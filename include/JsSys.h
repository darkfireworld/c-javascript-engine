#ifndef JsSysH
#define JsSysH
#include"JsType.h"
/*该模块 占用了USR1的信号位, 其他模块不能再占用!*/
struct JsVm;
struct JsEngine;
struct JsContext;

/****************************************************************************
									通用API
*****************************************************************************/

//模块初始化API
void JsPrevInitSys();
void JsPostInitSys();

/***********************************内存管理模块 API*******************************/

/*
	委托到GC管理的内存中
		size must > 0
		markFm = NULL 的时候, 表示配置的内存空间为原生型 如int*,double* 等
		freeFn 为在回收该内存的时候, 执行的析构动作
*/
void* JsGcMalloc(int size,JsGcMarkFn markFn,JsGcFreeFn freeFn);

/*Gc空间realloc大小*/
void* JsGcReAlloc(void* mp,int newSize);
/*
	标记指向内存空间为可用, 并且调用申请该内存空间时候, 配置的fn
	返回
		0 	: 先前没有标记过
		1 	: 先前标记过
		-1	: 不在Gc管理的内存中
	*一个属性是否被Mark到, 在于父容器是否对该属性所在的MP(内存地址)进行Mark
*/
int JsGcMark(void* mp);
/*
	Gc中添加一个有效的Key类型(指针类型),Key != 0, 
	当该Key指向的内存空间被回收的时候, 会导致该Key失效
	也可以手动调用JsGcBurnKey()来销毁该Key
	返回
		0 : 已经存在该key
		1 : 插入成功
*/
int JsGcRegistKey(void* key,const char* desc);
/*Gc中删除一个Key , Key != 0, 并且RootTable中相关的点*/
void JsGcBurnKey(void* key);
/*Gc扫描的时候, Root节点配置*/
void JsGcMountRoot(void* mp,void* key);
/*
	关系图完整性API:
		*正在处理A->B 关系图为 A->C->B的时候, 
	变成A , C , B 的引用方式, 而导致B被回收.
	
	例如:
		在NIO中, 处理 Context->scope.push(object), 的操作的时候
	需要结冻GC, 避免被错误回收
*/
void JsGcFreeze();
void JsGcUnfreeze();

/*	
	将当前TLS中的内存刷入主存中, 让GC可以进行回收, 
	这时候, 需要确定关系图的完整性(一般不调用)
*/
void JsGcCommit();
/****************************锁模块 API**********************************/
/*锁 API*/
JsLock JsCreateLock();
void JsLockup(JsLock lock);
void JsUnlock(JsLock lock);

/*****************************线程模块API********************************/

/*线程 API*/
//返回线程相关信息
JsThread JsCurThread();
/*通过该API启动的线程, 会添加GC特性*/
JsThread JsStartThread(JsThreadFn fn,void* data);
//自身线程安全退出
void JsCloseSelf();
//终止非当前线程
void JsCloseThread(JsThread thread);
//sleep time = ms
void JsSleep(long time);
//yield
void JsYield();
//thread join
void JsJoin(JsThread thread);
//马上退出
void JsHalt();

/*********************************TLS  模块 API******************************/

/*把数据存储在当前TLS中*/
JsTlsKey JsCreateTlsKey(JsTlsFn fn);
void JsSetTlsValue(JsTlsKey key, void* value);
void* JsGetTlsValue(JsTlsKey key);

#endif