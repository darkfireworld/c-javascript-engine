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

#define JS_RESPONSE_FLOOR 1

/*以下的内存为手动管理*/
struct JsHeader{
	char* key;
	char* value;
};
struct JsResponse{
	//连接
	struct MHD_Connection *connection;
	//状态码
	int code;
	//body已经使用的大小
	int bUsed;
	//body大小
	int bTotal;
	//HTML BODY
	char* body;
	
	//head已经使用
	int hUsed;
	//head已经使用的大小, 空为NULL
	int hTotal;
	//HTML Header
	struct JsHeader* header;
	
};


static void JsInitResponse(struct JsObject* response);

static void setHeader(struct JsObject *self, struct JsObject *thisobj, int argc, struct JsValue **argv, struct JsValue *res);
static void setStatus(struct JsObject *self, struct JsObject *thisobj, int argc, struct JsValue **argv, struct JsValue *res);
/*unistd 重名修改为write0*/
static void write0(struct JsObject *self, struct JsObject *thisobj, int argc, struct JsValue **argv, struct JsValue *res);
static void clear(struct JsObject *self, struct JsObject *thisobj, int argc, struct JsValue **argv, struct JsValue *res);



/*
Js:
	setHeader(String,String): 	是一个通用的标头设定方法，您可以用它来设定任何「名称/值」的标头.
	setStatus(Number):			状态码
	write(String) : 			写入返回文本
*/
struct JsObject* JsCreateResponse(struct MHD_Connection *connection){
	struct JsObject* b = JsAllocObject(JS_RESPONSE_FLOOR);
	JsCreateStandardObject(b);
	b->Class = "Response";
	
	//初始化自己的Sb
	struct JsResponse* p = (struct JsResponse*)malloc(sizeof(struct JsResponse));
	p->connection = connection;
	p->code = 200;
	
	p->bUsed = 0;
	p->bTotal = 128;
	p->body = calloc(sizeof(char),p->bTotal);
	
	p->hTotal = 2;
	p->hUsed = 0;
	p->header = (struct JsHeader* )calloc(sizeof(struct JsHeader) ,p->hTotal);
	
	
	b->sb[JS_RESPONSE_FLOOR] = p;
	//初始化Object方法函数
	JsInitResponse(b);
	return b;

}
/*
	将response提交到queue中, 并且销毁
*/
int JsSubmitResponse(struct JsObject* obj){
	if(strcmp(obj->Class,"Response")!=0)
		JsThrowString("The Object Is't Response");
	struct JsResponse* response = (struct JsResponse*)obj->sb[JS_RESPONSE_FLOOR];
	
	if(response == NULL)
			JsThrowString("The Response Is Burned");
			
	struct MHD_Response *HTMLResponse = NULL;
	//构建HTML response
	HTMLResponse = MHD_create_response_from_buffer (response->bUsed, 
					 (void *) response->body, 
					 MHD_RESPMEM_MUST_COPY);
	//配置header
	int i;
	for(i=0;i<response->hUsed;++i){
		MHD_add_response_header (HTMLResponse, response->header[i].key,  response->header[i].value);
	}
	//配置状态码
	int ret = MHD_queue_response (response->connection, response->code, HTMLResponse);
	MHD_destroy_response (HTMLResponse);
	
	//释放内存
	free(response->body);
	for(i=0;i<response->hUsed;++i){
		free(response->header[i].key);
		free(response->header[i].value);
	}
	free(response->header);
	free(response);
	obj->sb[JS_RESPONSE_FLOOR] = NULL;
	return ret;
	
}


static void JsInitResponse(struct JsObject* response){
	
	
	struct JsValue* p= NULL;
	p= JsCreateValue();
	p->type = JS_OBJECT;
	p->u.object = JsCreateStandardFunctionObject(NULL,NULL,FALSE);
	p->u.object->Call = &setHeader;
	(*response->Put)(response,"setHeader",p,JS_OBJECT_ATTR_STRICT);
	
	p= JsCreateValue();
	p->type = JS_OBJECT;
	p->u.object = JsCreateStandardFunctionObject(NULL,NULL,FALSE);
	p->u.object->Call = &setStatus;
	(*response->Put)(response,"setStatus",p,JS_OBJECT_ATTR_STRICT);
	
	p= JsCreateValue();
	p->type = JS_OBJECT;
	p->u.object = JsCreateStandardFunctionObject(NULL,NULL,FALSE);
	p->u.object->Call = &write0;
	(*response->Put)(response,"write",p,JS_OBJECT_ATTR_STRICT);
	
	p= JsCreateValue();
	p->type = JS_OBJECT;
	p->u.object = JsCreateStandardFunctionObject(NULL,NULL,FALSE);
	p->u.object->Call = &clear;
	(*response->Put)(response,"clear",p,JS_OBJECT_ATTR_STRICT);
}


static void setHeader(struct JsObject *self, struct JsObject *thisobj, int argc, struct JsValue **argv, struct JsValue *res){
	if(strcmp(thisobj->Class,"Response")!=0)
		JsThrowString("The Object Is't Response");
	if(argc >= 2 && argv[0]->type == JS_STRING && argv[0]->u.string != NULL
		&& argv[1]->type == JS_STRING && argv[1]->u.string != NULL){
		struct JsResponse* response = (struct JsResponse*)thisobj->sb[JS_RESPONSE_FLOOR];
		if(response == NULL)
			JsThrowString("The Response Is Burned");
		int i;
		int flag = TRUE;
		//判断是否已经存在该Header Key
		for(i=0;i<response->hUsed;++i){
			if(strcmp(response->header[i].key,argv[0]->u.string) == 0){
			
				//已经存在Key在Head中, 仅仅使用覆盖的方式
				flag = FALSE;
				//释放先前的内存
				free(response->header[i].value);
				char* value = (char*)malloc(strlen(argv[1]->u.string)+4);
				strcpy(value,argv[1]->u.string);
				response->header[i].value = value;
				break;
			}
		}
		if(flag == TRUE){
			//先前不存在Key对应的Header
			//检验空间是否足够
			if(response->hUsed == response->hTotal){
				response->hTotal += 2;
				//申请新空间
				response->header = realloc(response->header,sizeof(struct JsHeader)*response->hTotal);
			}
			response->header[response->hUsed].key = (char*)malloc(strlen(argv[0]->u.string)+4);
			response->header[response->hUsed].value = (char*)malloc(strlen(argv[1]->u.string)+4);
			strcpy(response->header[response->hUsed].key,argv[0]->u.string);
			strcpy(response->header[response->hUsed].value,argv[1]->u.string);
			
			response->hUsed++;
		}
	}else{
		JsThrowString("Response Args Is Error");
	}
}

static void setStatus(struct JsObject *self, struct JsObject *thisobj, int argc, struct JsValue **argv, struct JsValue *res){
	if(strcmp(thisobj->Class,"Response")!=0)
		JsThrowString("The Object Is't Response");
	if(argc >= 1 && argv[0]->type == JS_NUMBER && argv[0]->u.number >=0){
		struct JsResponse* response = (struct JsResponse*)thisobj->sb[JS_RESPONSE_FLOOR];
		if(response == NULL)
			JsThrowString("The Response Is Burned");
		response->code = argv[0]->u.number;
	}else{
		JsThrowString("setStatus Args Is Error");
	}
}
static void write0(struct JsObject *self, struct JsObject *thisobj, int argc, struct JsValue **argv, struct JsValue *res){
	if(strcmp(thisobj->Class,"Response")!=0)
		JsThrowString("The Object Is't Response");
	if(argc >= 1 && argv[0]->type == JS_STRING && argv[0]->u.string != NULL){
		struct JsResponse* response = (struct JsResponse*)thisobj->sb[JS_RESPONSE_FLOOR];
		if(response == NULL)
			JsThrowString("The Response Is Burned");
		int sizeOfString = strlen(argv[0]->u.string);
		//检查空间大小
		if(response->bUsed + sizeOfString >  response->bTotal){
			//增加空间
			response->bTotal += response->bUsed*2 + sizeOfString + 4;
			response->body = realloc(response->body,response->bTotal);
		}
		memcpy(&response->body[response->bUsed],argv[0]->u.string,sizeOfString);
		//大小变大
		response->bUsed+=sizeOfString;
	}else{
		JsThrowString("setStatus Args Is Error");
	}

}
static void clear(struct JsObject *self, struct JsObject *thisobj, int argc, struct JsValue **argv, struct JsValue *res){
	if(strcmp(thisobj->Class,"Response")!=0)
		JsThrowString("The Object Is't Response");
	struct JsResponse* response = (struct JsResponse*)thisobj->sb[JS_RESPONSE_FLOOR];
	if(response == NULL)
			JsThrowString("The Response Is Burned");
	response->bUsed = 0;
}