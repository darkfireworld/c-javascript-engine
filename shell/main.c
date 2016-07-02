#include"JsObject.h"
#include"JsContext.h"
#include"JsEngine.h"
#include"JsVm.h"
#include"JsList.h"
#include"JsValue.h"
#include"JsSys.h"
#include"JsDebug.h"
#include"JsException.h"
#include"JsAst.h"
#include"JsParser.h"
#include"JsEval.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<setjmp.h>
#include<unistd.h>
#include <sys/dir.h>
#include <sys/stat.h>

/**************************Print Function***************************/
//一个参数value
static void JsPrintFn(struct JsEngine* e,void* data,struct JsValue* res){
	struct JsValue v;
	struct JsContext* c = JsGetTlsContext();
	if(c ==  NULL)
		return;
	//获得参数
	JsFindValue(c,"value",&v);
	JsPrintValue(&v);
}
static void CreatePrintFn(){
	char** argv = JsGcMalloc(sizeof(char*) * 1,NULL,NULL);
	argv[0] = "value";
	//创建PrintFunction
	struct JsObject* print = JsCreateStandardSpecFunction(NULL,NULL,1,argv,
		&JsPrintFn,NULL,"print",FALSE);
	struct JsValue* vPrint = JsCreateValue();
	vPrint->type = JS_OBJECT;
	vPrint->u.object = print;
	(*JsGetVm()->Global->Put)(JsGetVm()->Global,"print",vPrint,JS_OBJECT_ATTR_STRICT);
}

/*****************************Run First Task*********************************/
static void JsContextTask(struct JsEngine* e,void* data){

	struct JsAstNode* ast = NULL;
	struct JsValue v;
	ast = JsParseFile(JS_PARSER_DEBUG_ERROR,NULL);
	if(ast == NULL)
		printf("language error\n");
	else
		JsEval(e,ast,&v);
}
int main(){
	JsCreateVm(TRUE,0,NULL, NULL);
	CreatePrintFn();
	struct JsEngine* e = JsCreateEngine();
	while(1){
		struct JsContext* c = JsCreateContext(e, NULL,"Main Context");
		JsDispatch(c,&JsContextTask,NULL);
	}
	
	
	//安全推出主线程
	JsCloseSelf();
	return 0;
}