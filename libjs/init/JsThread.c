#include"JsObject.h"
#include"JsContext.h"
#include"JsEngine.h"
#include"JsVm.h"
#include"JsValue.h"
#include"JsList.h"
#include"JsSys.h"
#include"JsDebug.h"
#include"JsAst.h"
#include"JsAsync.h"
#include"JsECMAScript.h"
#include"JsException.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#define JS_NEW_THREAD_OBJECT_FLOOR 1

static void JsThreadObjectInit(struct JsObject* thread);
//创建一个由于start函数构造处理来的对象
static struct JsObject* JsCreateMockThread(JsThread t);

static void JsStart(struct JsObject *self, struct JsObject *thisobj, 
		int argc, struct JsValue **argv, struct JsValue *res);

static void JsThreadSleep(struct JsObject *self, struct JsObject *thisobj, 
		int argc, struct JsValue **argv, struct JsValue *res);

static void JsThreadYield(struct JsObject *self, struct JsObject *thisobj, 
		int argc, struct JsValue **argv, struct JsValue *res);
		
static void JsThreadJoin(struct JsObject *self, struct JsObject *thisobj, 
		int argc, struct JsValue **argv, struct JsValue *res);
		
		
void JsThreadInit(struct JsVm* vm){
	
	struct JsObject* thread = JsCreateStandardObject(NULL);
	//设置Class
	thread->Class = "Thread";
	JsThreadObjectInit(thread);
	
	//添加到Global中
	struct JsValue* v = JsCreateValue();
	v->type = JS_OBJECT;
	v->u.object = thread;
	(*vm->Global->Put)(vm->Global,"Thread",v,JS_OBJECT_ATTR_STRICT);
	
	
}

static void JsThreadObjectInit(struct JsObject* thread){
	
	struct JsValue* vProperty  = NULL;
	struct JsObject* function = NULL;
	//添加start函数
	function  = JsCreateStandardFunctionObject(NULL,NULL,FALSE);
	vProperty = JsCreateValue();
	vProperty->type = JS_OBJECT;
	vProperty->u.object = function;
	function->Call = &JsStart;
	(*thread->Put)(thread,"start",vProperty,JS_OBJECT_ATTR_STRICT);
	//添加sleep函数
	function  = JsCreateStandardFunctionObject(NULL,NULL,FALSE);
	vProperty = JsCreateValue();
	vProperty->type = JS_OBJECT;
	vProperty->u.object = function;
	function->Call = &JsThreadSleep;
	(*thread->Put)(thread,"sleep",vProperty,JS_OBJECT_ATTR_STRICT);
	//添加yield函数
	function  = JsCreateStandardFunctionObject(NULL,NULL,FALSE);
	vProperty = JsCreateValue();
	vProperty->type = JS_OBJECT;
	vProperty->u.object = function;
	function->Call = &JsThreadYield;
	(*thread->Put)(thread,"yield",vProperty,JS_OBJECT_ATTR_STRICT);
}
/*
	特殊的Thread对象
*/
static struct JsObject* JsCreateMockThread(JsThread t){
	struct JsValue* vProperty  = NULL;
	struct JsObject* function = NULL;
	
	if(t == NULL)
		JsThrowString("Create Thread Error");
	struct JsObject* thread = JsAllocObject(JS_NEW_THREAD_OBJECT_FLOOR);
	//构建了standard
	JsCreateStandardObject(thread);
	thread->Class = "MockThread";
	thread->sb[JS_NEW_THREAD_OBJECT_FLOOR] = t;
	//添加sleep函数
	function  = JsCreateStandardFunctionObject(NULL,NULL,FALSE);
	vProperty = JsCreateValue();
	vProperty->type = JS_OBJECT;
	vProperty->u.object = function;
	function->Call = &JsThreadJoin;
	(*thread->Put)(thread,"join",vProperty,JS_OBJECT_ATTR_STRICT);
	return thread;
}
/*
	scope 为原先的Context的Scope
	thisobj 使用调用该函数的Global
*/
static void JsStart(struct JsObject *self, struct JsObject *thisobj, 
		int argc, struct JsValue **argv, struct JsValue *res){
	if(argc >=0 &&argv[0]->type == JS_OBJECT && 
		argv[0]->u.object != NULL && argv[0]->u.object->Call != NULL){
		//pass test
		JsThread thread = JsAsync(NULL,NULL,argv[0]->u.object,TRUE);
		res->type = JS_OBJECT;
		res->u.object = JsCreateMockThread(thread);
	}else{
		JsThrowString("TypeError");
	}
}

static void JsThreadSleep(struct JsObject *self, struct JsObject *thisobj, 
		int argc, struct JsValue **argv, struct JsValue *res){
		
	if(argc <= 0 || argv[0]->type != JS_NUMBER)
		JsThrowString("Args Error");
	JsSleep(argv[0]->u.number);
}

static void JsThreadYield(struct JsObject *self, struct JsObject *thisobj, 
		int argc, struct JsValue **argv, struct JsValue *res){
		
	JsYield();
		
}

static void JsThreadJoin(struct JsObject *self, struct JsObject *thisobj, 
		int argc, struct JsValue **argv, struct JsValue *res){
		
	if(strcmp(thisobj->Class,"MockThread") == 0 ){
		JsThread thread = (JsThread)thisobj->sb[JS_NEW_THREAD_OBJECT_FLOOR];
		JsJoin(thread);
	}
}
