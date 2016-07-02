#include"JsAsync.h"
#include"JsObject.h"
#include"JsContext.h"
#include"JsEngine.h"
#include"JsVm.h"
#include"JsValue.h"
#include"JsList.h"
#include"JsSys.h"
#include"JsDebug.h"
#include"JsAst.h"
#include"JsException.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<setjmp.h>
struct JsAsyncData{
	JsThreadFn work;
	void* data;
	struct JsContext* context;
	struct JsObject* function;
};
struct JsAsyncTaskData{
	int argc;
	struct JsValue** argv;
	struct JsObject* function;
};


static void* JsAsyncWork(void* data);
static void JsAsyncTask(struct JsEngine* e,void* data);


static void JsGcMarkAsyncData(void* mp,int ms);
static void JsGcMarkAsyncTaskData(void* mp,int ms);

JsThread JsAsync(JsThreadFn work,void* data, struct JsObject* o, int openEngine){

	struct JsEngine* e = NULL;
	struct JsContext* c = NULL;
	c = JsGetTlsContext();
	e = c->engine;
	if(openEngine == TRUE){
		//开启新引擎 
		e = JsCreateEngine();
		if(o== NULL || o->Call == NULL){
			//开启新引擎, 缺没有一个回调函数, 则开启是错误的
			JsAssert(FALSE);
		}
	}
	//创建新上下文
	c = JsCreateContext(e,c,"Async Context");
	//统一为Global对象
	c->thisObj = JsGetVm()->Global;
	//配置传递数据
	struct JsAsyncData* p =( struct JsAsyncData* ) JsGcMalloc(sizeof(struct JsAsyncData),&JsGcMarkAsyncData,NULL);
	JsGcMountRoot(p,c);
	
	
	p->work = work;
	p->data = data;
	p->context = c;
	p->function = o;
	JsThread thread = JsStartThread(&JsAsyncWork,p);
	return thread;
}

static void* JsAsyncWork(void* data){
	struct JsValue* error = NULL;
	struct JsAsyncData* p = (struct JsAsyncData*)data;
	struct JsAsyncWR* workRes = NULL;
	//设置本线程的JsContext
	JsSetTlsContext( p->context);
	//填充当前线程信息
	p->context->thread = JsCurThread();
	
	JS_TRY(0){
		//DO Async WORK
		if(p->work != NULL)
			workRes = (struct JsAsyncWR*)(*p->work)(p->data);
	}
	JS_CATCH(error){
		JsPrintValue(error);
		JsPrintStack(JsGetExceptionStack());
		//删除该Context
		JsBurnContext(p->context);
		p->context->thread = NULL;
		return NULL;
	}
	//Finish
	if(p->function != NULL && p->function->Call != NULL){

		struct JsAsyncTaskData* taskData = (struct JsAsyncTaskData*)JsGcMalloc(
				sizeof(struct JsAsyncTaskData),&JsGcMarkAsyncTaskData,NULL);
		//挂载到Context上
		JsGcMountRoot(taskData,p->context);
		taskData->function = p->function;
		taskData->argc = 0;
		taskData->argv = NULL;
		if(workRes != NULL){
			taskData->argc = workRes->argc;
			taskData->argv = workRes->argv;
		}
		//调入Engine中轮转
		JsDispatch(p->context,&JsAsyncTask,taskData);
	}
	else
		JsBurnContext(p->context);
	p->context->thread = NULL;
	return NULL;
}
//被Dispatch 调用的task
static void JsAsyncTask(struct JsEngine* e,void* data){
	struct JsValue res;
	struct JsContext* c = JsGetTlsContext();
	struct JsAsyncTaskData* p =(struct JsAsyncTaskData*)data;
	
	(*p->function->Call)(p->function,c->thisObj,p->argc,p->argv,&res);
}

static void JsGcMarkAsyncData(void* mp,int ms){
	struct JsAsyncData* p = (struct JsAsyncData*)mp;
	JsGcMark(p->data);
	JsGcMark(p->context);
	JsGcMark(p->function);
}
static void JsGcMarkAsyncTaskData(void* mp,int ms){
	struct JsAsyncTaskData* t =(struct JsAsyncTaskData*)mp;
	JsGcMark(t->argv);
	JsGcMark(t->function);
}