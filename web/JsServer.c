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
#include"JsAsync.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<setjmp.h>
#include<unistd.h>
#include <sys/dir.h>
#include <sys/stat.h>
#include <microhttpd.h>


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



/**************************startServer Function***************************/
struct JsRequestData{
	struct JsObject* request;
	struct JsObject* response;
	struct JsObject* function;
};
/*
	开启Daemon的时候, 环境参数
	交给连接处理线程使用
*/
struct JsStartDaemon{
	struct JsObject* function;
	struct JsContext* context;

};

struct JsServerData{
	struct JsObject* function;
	int port;
};
static void JsGcMarkServerData(void* mp,int ms){
	struct JsServerData* p = (struct JsServerData*)mp;
	JsGcMark(p->function);
}

static void JsRequestTaskFn(struct JsEngine* e,void* data){
	struct JsRequestData* rd = (struct JsRequestData*)data;
	struct JsValue* argv[2];
	struct JsValue v0,v1,res;
	v0.type = JS_OBJECT;
	v1.type = JS_OBJECT;
	v0.u.object = rd->request;
	v1.u.object = rd->response;
	argv[0] = &v0;
	argv[1] = &v1;
	rd->function->Call(rd->function,JsGetVm()->Global,2,argv,&res);
}

static int
answer_to_connection(void *cls,
          struct MHD_Connection *connection,
          const char *url,
          const char *method,
          const char *version,
          const char *upload_data, size_t *upload_data_size, void **ptr){ 
	struct JsStartDaemon* sdp = (struct JsStartDaemon*)cls;
	
	//构建requset
	struct JsObject* request = JsCreateRequest(connection,url,method);
	//构建response
	struct JsObject* response = JsCreateResponse(connection);
	//开启Engine进行工作
	struct JsEngine* engine = JsCreateEngine();
	struct JsContext* context = JsCreateContext(engine, sdp->context,"RequestContext");
	
	struct JsRequestData* rd = (struct JsRequestData*)malloc(sizeof(struct JsRequestData));
	rd->request = request;
	rd->response = response;
	rd->function = sdp->function;
	//立马执行该task
	JsDispatch(context,&JsRequestTaskFn,rd);
	
	free(rd);
	//提交Response
	return JsSubmitResponse(response);
}
static void* JsStartServerWork(void* data){
	
	struct JsServerData* sd = (struct JsServerData* )data;
	struct JsStartDaemon* sdp = (struct JsStartDaemon*)malloc(sizeof(struct JsStartDaemon));
	sdp->function = sd->function;
	sdp->context = JsGetTlsContext();
	//开启守护线程
	struct MHD_Daemon *daemon;
	//使用epoll方式
	daemon = MHD_start_daemon (MHD_USE_THREAD_PER_CONNECTION
								|MHD_USE_DEBUG, sd->port, NULL, NULL,
						 &answer_to_connection, sdp, MHD_OPTION_END);
						 
	if(daemon == NULL){
		JsThrowString("StartServer Fail");
	}
	//陷入永远sleep, 防止对sd对象的回收
	while(1){
		sleep(3600*24);
	}
	//结束的时候, 释放该内存
	free(sdp);
	return NULL;
}

static void JsStartServerFn(struct JsObject *self, struct JsObject *thisobj,
			int argc, struct JsValue **argv, struct JsValue *res){
			
	if(argc >= 2 && argv[0]->type == JS_OBJECT &&argv[0]->u.object != NULL && argv[0]->u.object->Call != NULL
		&& argv[1]->type == JS_NUMBER &&argv[1]->u.number >=0 ){
		struct JsServerData* data = (struct JsServerData*)JsGcMalloc(sizeof(struct JsServerData),&JsGcMarkServerData,NULL);
		data->function  = argv[0]->u.object;
		data->port =  argv[1]->u.number;
		
		JsAsync(&JsStartServerWork,data,NULL,FALSE);
	}else{
		JsThrowString("TypeError");
	}
}

static void CreateStartServer(){
	struct JsValue* vStartServer = JsCreateValue();
	vStartServer->type = JS_OBJECT;
	vStartServer->u.object  = JsCreateStandardFunctionObject(NULL,NULL,FALSE);
	vStartServer->u.object->Call = &JsStartServerFn;
	(*JsGetVm()->Global->Put)(JsGetVm()->Global,"startServer",vStartServer,JS_OBJECT_ATTR_STRICT);
}

void JsLoadWebModule(){
	//加载打印函数
	CreatePrintFn();
	//加载startServer();
	CreateStartServer();
}