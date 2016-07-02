#include"JsObject.h"
#include"JsContext.h"
#include"JsEngine.h"
#include"JsVm.h"
#include"JsValue.h"
#include"JsList.h"
#include"JsSys.h"
#include"JsInit.h"
#include"JsException.h"
#include"JsDebug.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

//全局唯一变量
static struct JsVm* g_JsVm = NULL;

//初始化各种优先于VM模块的模块单位
static void JsPrevInitModules();
//加载DLL, SO 类型的模块
static void JsLoadShareModule(struct JsVm* vm);
//Vm初始化完成后, 再初始化的资源模块
static void JsPostInitModules();

static void JsGcMarkVm(void* mp, int ms);
//------------------------------------------------
//初始化一个新的Vm
struct JsVm* JsCreateVm(int debug,int mSize, char** mPath,
					JsVmTraceFn traceFn){
	if(g_JsVm)
		return g_JsVm;
	//初始化优先于VM的模块
	JsPrevInitModules();
	g_JsVm = (struct JsVm*)JsGcMalloc(sizeof(struct JsVm),&JsGcMarkVm,NULL);
	//挂载
	JsGcRegistKey(g_JsVm,"VM");
	JsGcMountRoot(g_JsVm,g_JsVm);
	
	g_JsVm->state = JS_VM_START;
	g_JsVm->debug = debug;
	g_JsVm->trace = traceFn;
	
	g_JsVm->engines = JsCreateList();
	//初始化Ecmascript的对象
	JsECMAScriptObjectInit(g_JsVm);
	JsAssert(g_JsVm->Global != NULL);
	
	g_JsVm->mSize = mSize;
	g_JsVm->mPath = mPath;
	JsLoadShareModule(g_JsVm);
	
	g_JsVm->lock = JsCreateLock();
	JsPostInitModules();
	
	return g_JsVm;
}
//关闭Vm
void JsHaltVm(){
	JsHalt();
}
void JsEnginePiVm(struct JsEngine* e){

	JsAssert(g_JsVm != NULL && e != NULL);
	JsLockup(g_JsVm->lock);
	JsListPush(g_JsVm->engines,e);
	JsUnlock(g_JsVm->lock);
}
struct JsVm* JsGetVm(){
	JsAssert( g_JsVm != NULL);
	return g_JsVm;
}

/*锁住VM*/
void JsLockVm(){
	JsLockup(g_JsVm->lock);
	int size = JsListSize(g_JsVm->engines);
	int i;
	for(i=0;i<size;++i){
		struct JsEngine* e = JsListGet(g_JsVm->engines,i);
		JsLockup(e->lock);
	}
}
void JsUnlockVm(){
	int size = JsListSize(g_JsVm->engines);
	int i;
	for(i=0;i<size;++i){
		struct JsEngine* e = JsListGet(g_JsVm->engines,i);
		JsUnlock(e->lock);
	}
	JsUnlock(g_JsVm->lock);
}
static void JsPrevInitModules(){
	//.h中声明为 JsPrevInitializeModule 的API
	JsPrevInitSys();
	JsPrevInitException();
	JsPrevInitEngine();
	JsPrevInitContext();
}

static void JsLoadShareModule(struct JsVm* vm){


}

static void JsPostInitModules(){
	//.h中声明为 JsPostInitializeModule 的API
	JsPostInitSys();
	JsPostInitException();
	JsPostInitEngine();
	JsPostInitContext();
}

static void JsGcMarkVm(void* mp, int ms){
	struct JsVm* vm = (struct JsVm*)mp;
	JsGcMark(vm->engines);
	JsGcMark(vm->Global);
	JsGcMark(vm->mPath);
	JsGcMark(vm->lock);
}
