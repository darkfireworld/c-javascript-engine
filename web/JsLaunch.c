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
#include"JsWeb.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<setjmp.h>
#include<unistd.h>
#include <sys/dir.h>
#include <sys/stat.h>

//启动文件
static char* launch = NULL;

/*****************************Run First Task*********************************/
static void JsContextTask(struct JsEngine* e,void* data){

	JS_TRY(0){
		struct JsAstNode* ast = NULL;
		struct JsValue v;
		ast = JsParseFile(JS_PARSER_DEBUG_ERROR,launch);
		JsAssert(ast != NULL);
		JsEval(e,ast,&v);
	}
	struct JsValue* err;
	JS_CATCH(err){
		JsPrintValue(err);
		JsPrintStack(JsGetExceptionStack());
	}

}
int main(int argc,char* argv[]){
	if(argc != 2){
		printf("Format: command launch.js\n");
		return 0;
	}
	launch = argv[1];
	//创建VM环境
	JsCreateVm(TRUE,0,NULL, NULL);
	//加载各个模块
	JsLoadWebModule();
	//开启引擎
	struct JsEngine* e = JsCreateEngine();
	struct JsContext* c = JsCreateContext(e, NULL,"Main Context");
	JsDispatch(c,&JsContextTask,NULL);
	//安全推出主线程
	JsCloseSelf();
	return 0;
}