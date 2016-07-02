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
#include <microhttpd.h>

#define JS_REQUEST_FLOOR 1

struct JsRequest{
	struct MHD_Connection *connection;
	const char *url;
    const char *method;
};
static void JsInitRequest(struct JsObject* req);



static void getParameter(struct JsObject *self, struct JsObject *thisobj, int argc, struct JsValue **argv, struct JsValue *res);
static void getServerName(struct JsObject *self, struct JsObject *thisobj, int argc, struct JsValue **argv, struct JsValue *res);
static void getMethod(struct JsObject *self, struct JsObject *thisobj, int argc, struct JsValue **argv, struct JsValue *res);
static void getServerPort(struct JsObject *self, struct JsObject *thisobj, int argc, struct JsValue **argv, struct JsValue *res);
static void getRequestURI(struct JsObject *self, struct JsObject *thisobj, int argc, struct JsValue **argv, struct JsValue *res);


/*

	getParameter(): String 						可以让您指定请求参数名称，以取得对应的设定值.
	getServerName():String						请求的服务器.
	getMethod(): "POST" | "GET" | "DELETE" ...	请求方法.
	getServerPort(): Number						请求端口号.
	getRequestURI():URI路径.-> '/main/list.do'	除去http://localhost:8080/部分的地址

*/

struct JsObject* JsCreateRequest(struct MHD_Connection *connection,
                                const char *url,
                                const char *method){
								
	struct JsObject* b = JsAllocObject(JS_REQUEST_FLOOR);
	JsCreateStandardObject(b);
	b->Class = "Request";
	
	//初始化自己的Sb
	struct JsRequest* p = (struct JsRequest*)JsGcMalloc(sizeof(struct JsRequest),NULL,NULL);
	p->connection = connection;
	p->url = url;
	p->method = method;
	b->sb[JS_REQUEST_FLOOR] = p;
	//初始化Object方法函数
	JsInitRequest(b);
	return b;
	
}

static void JsInitRequest(struct JsObject* req){
	
	
	struct JsValue* p= NULL;
	p= JsCreateValue();
	p->type = JS_OBJECT;
	p->u.object = JsCreateStandardFunctionObject(NULL,NULL,FALSE);
	p->u.object->Call = &getParameter;
	(*req->Put)(req,"getParameter",p,JS_OBJECT_ATTR_STRICT);
	
	p= JsCreateValue();
	p->type = JS_OBJECT;
	p->u.object = JsCreateStandardFunctionObject(NULL,NULL,FALSE);
	p->u.object->Call = &getServerName;
	(*req->Put)(req,"getServerName",p,JS_OBJECT_ATTR_STRICT);
	
	p= JsCreateValue();
	p->type = JS_OBJECT;
	p->u.object = JsCreateStandardFunctionObject(NULL,NULL,FALSE);
	p->u.object->Call = &getMethod;
	(*req->Put)(req,"getMethod",p,JS_OBJECT_ATTR_STRICT);
	
	p= JsCreateValue();
	p->type = JS_OBJECT;
	p->u.object = JsCreateStandardFunctionObject(NULL,NULL,FALSE);
	p->u.object->Call = &getServerPort;
	(*req->Put)(req,"getServerPort",p,JS_OBJECT_ATTR_STRICT);
	
	p= JsCreateValue();
	p->type = JS_OBJECT;
	p->u.object = JsCreateStandardFunctionObject(NULL,NULL,FALSE);
	p->u.object->Call = &getRequestURI;
	(*req->Put)(req,"getRequestURI",p,JS_OBJECT_ATTR_STRICT);

}
static void getParameter(struct JsObject *self, struct JsObject *thisobj, int argc, struct JsValue **argv, struct JsValue *res){
	if(strcmp(thisobj->Class,"Request")!=0)
		JsThrowString("The Object Is't Requset");
	if(argc >= 1 && argv[0]->type == JS_STRING && argv[0]->u.string != NULL){
		struct JsRequest* req = (struct JsRequest*)thisobj->sb[JS_REQUEST_FLOOR];
		const char* v0 = MHD_lookup_connection_value (req->connection, MHD_POSTDATA_KIND|MHD_GET_ARGUMENT_KIND, argv[0]->u.string);
		if(v0 == NULL){
			res->type = JS_NULL;
		}else{
			char* v1 = (char*)JsGcMalloc(strlen(v0)+4,NULL,NULL);
			strcpy(v1,v0);
			res->type = JS_STRING;
			res->u.string = v1;
		}
	}else{
		JsThrowString("getParameter Args Is Error");
	}
}
static void getServerName(struct JsObject *self, struct JsObject *thisobj, int argc, struct JsValue **argv, struct JsValue *res){
	res->type = JS_NULL;
}
static void getMethod(struct JsObject *self, struct JsObject *thisobj, int argc, struct JsValue **argv, struct JsValue *res){
	if(strcmp(thisobj->Class,"Request")!=0)
		JsThrowString("The Object Is't Requset");
	struct JsRequest* req = (struct JsRequest*)thisobj->sb[JS_REQUEST_FLOOR];
	char* m = (char*)JsGcMalloc(strlen(req->method) + 4,NULL,NULL);
	strcpy(m,req->method);
	res->type = JS_STRING;
	res->u.string = m;
}
static void getServerPort(struct JsObject *self, struct JsObject *thisobj, int argc, struct JsValue **argv, struct JsValue *res){
	res->type = JS_NULL;

}
static void getRequestURI(struct JsObject *self, struct JsObject *thisobj, int argc, struct JsValue **argv, struct JsValue *res){
	if(strcmp(thisobj->Class,"Request")!=0)
		JsThrowString("The Object Is't Requset");
	struct JsRequest* req = (struct JsRequest*)thisobj->sb[JS_REQUEST_FLOOR];
	char* m = (char*)JsGcMalloc(strlen(req->url) + 4,NULL,NULL);
	strcpy(m,req->url);
	res->type = JS_STRING;
	res->u.string = m;

}