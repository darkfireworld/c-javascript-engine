#include"JsObject.h"
#include"JsContext.h"
#include"JsEngine.h"
#include"JsVm.h"
#include"JsValue.h"
#include"JsList.h"
#include"JsSys.h"
#include"JsDebug.h"
#include"JsException.h"
#include<setjmp.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

//wait 队列的数据结构
struct JsTaskComp{
	struct JsContext* context;
	JsTaskFn task;
	void* data;
};
//当前线程的engine对象的key
static JsTlsKey JsEngineKey = NULL;
static void JsInitEngineKey();

static void JsGcMarkEngine(void* mp, int ms);
static void JsGcFreeEngine(void* mp,int size);
static void JsGcMarkTaskComp(void* mp, int ms);

void JsPrevInitEngine(){
	JsInitEngineKey();
}
void JsPostInitEngine(){

}

struct JsEngine* JsCreateEngine(){
	struct JsVm* vm = JsGetVm();
	struct JsEngine* e = (struct JsEngine*)JsGcMalloc(sizeof(struct JsEngine),&JsGcMarkEngine,&JsGcFreeEngine);
	e->vm = vm;
	e->state = JS_ENGINE_NEW;
	
	e->exec = NULL;
	e->waits = JsCreateList();
	e->pools = JsCreateList();
	
	e->lock = JsCreateLock();
	
	JsEnginePiVm(e);
	return e;
}

void JsDispatch(struct JsContext* c,JsTaskFn task0,void* data){
	struct JsTaskComp* task;
//验证
	JsAssert(c != NULL && task0 != NULL);
	
	struct JsEngine* e = c->engine;
	if(e == NULL || e->vm == NULL 
		|| e->state ==JS_ENGINE_STOPPED || e->vm->state == JS_VM_HALT)
		return;
//添加到wait队列中
	JsLockup(e->lock);
	task = (struct JsTaskComp*) JsGcMalloc(sizeof(struct JsTaskComp),&JsGcMarkTaskComp,NULL);
	task->context = c;
	task->task = task0;
	task->data = data;
	
	JsListPush(e->waits,task);
	c->thread = NULL;
	c = NULL;
	task = NULL;
	JsUnlock(e->lock);
//循环执行waits队列
	while(TRUE){
		//提交本次Tls内存到主存中
		JsGcCommit();
		
		JsLockup(e->lock);
		if(e->exec != NULL){
			JsUnlock(e->lock);
			return;
		}
		//获得waits第一个Context, 并且删除它
		task = (struct JsTaskComp*)JsListGet(e->waits,JS_LIST_FIRST);
		if(task !=NULL){
			//存在task
			e->exec = task->context ;
		}
		if(e->exec == NULL){
			//printf("Engine Goto IDLE\n");
			//修改为空闲状态
			e->state = JS_ENGINE_IDLE;
			//waits队列中不存在等待的context
			JsUnlock(e->lock);
			
			return;
		}
		//记录当前线程信息
		e->exec->thread = JsCurThread();
		JsUnlock(e->lock);
		
		
		//配置环境
		struct JsEngine* tlsEngine = JsGetTlsEngine();
		struct JsContext* tlsContext = JsGetTlsContext();
		JsSetTlsEngine(e);
		JsSetTlsContext(e->exec);
		//修改状态
		e->state = JS_ENGINE_RUNNING;
		
		JS_TRY(0){
			(*task->task)(e,task->data);
		}
		struct JsValue* error = NULL;
		JS_CATCH(error){
			JsPrintString("\n\nCatch Exception: ");
			JsPrintValue(error);
			JsPrintStack(JsGetExceptionStack());
			JsPrintString("\n\n");
		}
		//还原环境
		JsSetTlsEngine(tlsEngine);
		JsSetTlsContext(tlsContext);
		
		JsLockup(e->lock);
		//删除第一个等待task
		JsListRemove(e->waits,JS_LIST_FIRST);
		//因为执行完该Context之后, 就失效了,所以从Vm中消去该Context
		JsBurnContext(e->exec);
		e->exec = NULL;
		JsUnlock(e->lock);
		
		//提交本次Tls内存到主存中
		JsGcCommit();
	}

}
//注册一个新的context到reg中, lock
void JsContextPiEngine(struct JsEngine* e, struct JsContext* c){
	JsAssert(e != NULL && c != NULL);
	JsLockup(e->lock);
	JsListPush(e->pools,c);
	JsUnlock(e->lock);
}
void JsContextPoEngine(struct JsEngine* e, struct JsContext* c){
	JsAssert(e != NULL && c != NULL);
	JsLockup(e->lock);
	//从pools中剔除exec指向的context
	int size,i;
	size = JsListSize(e->pools);
	for(i = 0 ; i < size; ++i){
		if(JsListGet(e->pools,i) == c){
			JsListRemove(e->pools,i);
			break;
		}
	}
	JsUnlock(e->lock);

}
void JsStopEngine(struct JsEngine* e){
	
	struct JsContext* c;
	int size ,i;
	if(e ==NULL)
		return;
	JsLockup(e->lock);
	
	if(e->state == JS_ENGINE_STOPPED){
		JsUnlock(e->lock);
		return;
	}
	//Mark
	e->state = JS_ENGINE_STOPPED;
	
	size = JsListSize(e->pools);
	for( i = 0 ; i < size ; ++i){
		c = (struct JsContext*)JsListGet(e->pools,i);
		if(c->thread){
			//停止从属于该Engine的async线程
			JsCloseThread(c->thread);
			c->thread = NULL;
		}
	}
	JsUnlock(e->lock);
}
/*获得当前线程Engine*对象*/
void JsSetTlsEngine(struct JsEngine* e){
	JsSetTlsValue(JsEngineKey,e);
}
struct JsEngine* JsGetTlsEngine(){
	struct JsEngine* e;
	e = (struct JsEngine*)JsGetTlsValue(JsEngineKey);
	return e;
}
static void JsInitEngineKey(){
	JsEngineKey = JsCreateTlsKey(NULL);
	//挂在为Root
	JsGcRegistKey(JsEngineKey,"JsEngineKey");
	JsGcMountRoot(JsEngineKey,JsEngineKey);
}
static void JsGcMarkEngine(void* mp, int ms){
	struct JsEngine* e = (struct JsEngine*) mp;
	JsGcMark(e->vm);
	JsGcMark(e->exec);
	JsGcMark(e->waits);
	JsGcMark(e->pools);
	JsGcMark(e->lock);
}
static void JsGcFreeEngine(void* mp,int size){
	//JsAssert(FALSE);
}
static void JsGcMarkTaskComp(void* mp, int ms){
	struct JsTaskComp* task = (struct JsTaskComp*)mp;
	JsGcMark(task->context);
	JsGcMark(task->data);
}