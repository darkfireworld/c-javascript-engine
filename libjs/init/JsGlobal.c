#include"JsObject.h"
#include"JsContext.h"
#include"JsEngine.h"
#include"JsVm.h"
#include"JsValue.h"
#include"JsList.h"
#include"JsSys.h"
#include"JsDebug.h"
#include"JsException.h"
#include"JsAst.h"
#include"JsParser.h"
#include"JsEval.h"
#include"JsAsync.h"
#include"JsECMAScript.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>

static	void JsGlobalEval(struct JsObject *self, struct JsObject *thisobj, int argc, struct JsValue **argv, struct JsValue *res);
static	void JsIsNaN(struct JsObject *self, struct JsObject *thisobj, int argc, struct JsValue **argv, struct JsValue *res);

static void JsSetTimeout(struct JsObject *self, struct JsObject *thisobj, int argc, struct JsValue **argv, struct JsValue *res);
static void* JsSetTimeoutWork(void* data);

/*vm的空间已经被声明好了, 在JsInit.c中*/
void JsGlobalInit(struct JsVm* vm){
	
	struct JsValue t ;
	struct JsValue* v;
	
	//配置Global
	//Gloabl->Prototype
	(*vm->Global->Get)(vm->Global,"Object",NULL,&t);
	if(t.type != JS_OBJECT)
		JsAssert(FALSE);
	(*t.u.object->Get)(t.u.object,"prototype",NULL,&t);
	vm->Global->Prototype = t.u.object;
	//Global->Class
	vm->Global->Class = "Global";
	
	//NaN
	v = JsCreateValue();
	v->type = JS_NUMBER;
	v->u.number = JS_VALUE_NUMBER_NAN;
	(*vm->Global->Put)(vm->Global,"NaN",v,JS_OBJECT_ATTR_STRICT);
	//undefined
	v = JsCreateValue();
	v->type = JS_UNDEFINED;
	(*vm->Global->Put)(vm->Global,"undefined",v,JS_OBJECT_ATTR_STRICT);
	//eval
	v = JsCreateValue();
	v->type = JS_OBJECT;
	v->u.object = JsCreateStandardFunctionObject(NULL,NULL,FALSE);
	v->u.object->Call = &JsGlobalEval;
	(*vm->Global->Put)(vm->Global,"eval",v,JS_OBJECT_ATTR_STRICT);
	//isNaN
	v = JsCreateValue();
	v->type = JS_OBJECT;
	v->u.object = JsCreateStandardFunctionObject(NULL,NULL,FALSE);
	v->u.object->Call = &JsIsNaN;
	(*vm->Global->Put)(vm->Global,"isNaN",v,JS_OBJECT_ATTR_STRICT);
	
	//setTimeout
	v = JsCreateValue();
	v->type = JS_OBJECT;
	v->u.object = JsCreateStandardFunctionObject(NULL,NULL,FALSE);
	v->u.object->Call = &JsSetTimeout;
	(*vm->Global->Put)(vm->Global,"setTimeout",v,JS_OBJECT_ATTR_STRICT);

}



static	void JsIsNaN(struct JsObject *self, struct JsObject *thisobj, int argc, struct JsValue **argv, struct JsValue *res){
	res->type = JS_BOOLEAN;
	res->u.boolean = FALSE;
	if(argc <= 0)
		return;
	if(argv[0]->type == JS_NUMBER && isnan(argv[0]->u.number)){
		res->u.boolean  = TRUE;
	}
}
/*
	相当于把string字符串放置在当前环境下执行.
*/
static	void JsGlobalEval(struct JsObject *self, struct JsObject *thisobj, int argc, struct JsValue **argv, struct JsValue *res){

	int saveVarattr;
	//检查参数是否正确
	if(argc <=0){
		res->type =JS_UNDEFINED;
		return;
	}else  if( argv[0]->type != JS_STRING){
		*res = *argv[0];
		return;
	}
	
	//从线程中获得JsEngine
	struct JsEngine* e = JsGetTlsEngine();
	
	//处理上下文
	/*Stack, Scope, This 都使用当前Context*/
	saveVarattr = e->exec->varattr;
	e->exec->varattr = JS_OBJECT_ATTR_DEFAULT;
	
	
	//执行函数
	res->type =JS_UNDEFINED;
	struct JsAstNode * ast;
	ast = JsParseString(e->vm->debug ? JS_PARSER_DEBUG_ERROR 
		: JS_PARSER_DEBUG_CLOSE,argv[0]->u.string);
	if(ast == NULL)
		JsThrowString("SytaxError");
	JsEval(e,ast,res);
	//还原环境
	e->exec->varattr = saveVarattr;
	
	//结果处理
	if(res->type == JS_COMPLETION){

		if(res->u.completion.type == JS_COMPLETION_RETURN){
			*res = *res->u.completion.value;
		}else if(res->u.completion.type == JS_COMPLETION_THROW){
			//不处理
		}else{
			//JS_COMPLETION_NORMAL, JS_COMPLETION_BREAK, JS_COMPLETION_CONTINUE
			res->type = JS_UNDEFINED;
		}
	}else if(res->type == JS_REFERENCE){
		res->type = JS_UNDEFINED;
	}//res->type = JS_OBJECT, JS_NULL, JS_BOOLEAN ...
	return;

}

//一个参数value
static void JsSetTimeout(struct JsObject *self, struct JsObject *thisobj, int argc,
		struct JsValue **argv, struct JsValue *res){

	if(argc >= 2 && argv[0]->type == JS_OBJECT &&argv[0]->u.object != NULL && argv[0]->u.object->Call != NULL
		&& argv[1]->type == JS_NUMBER &&argv[1]->u.number >=0 ){
		int* data = (int*)JsGcMalloc(sizeof(int),NULL,NULL);
		*data = argv[1]->u.number;
		JsAsync(&JsSetTimeoutWork,data,argv[0]->u.object,FALSE);
	}else{
		JsThrowString("TypeError");
	}
}
//NIO work
static void* JsSetTimeoutWork(void* data){
	int* time  = (int*)data;
	JsSleep(*time);
	return NULL;
}

