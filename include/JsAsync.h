#ifndef JsAsyncH
#define JsAsyncH
/**
	NIO线程  和 开启新Engine的时候, 调用的函数, 提供了一个统一的接口
	不必在配置context 和 engine, 并且添加默认异常处理.
	注: 新线程中的context的this为Gloabl
*/
#include"JsType.h"


struct JsAsyncWR{
	int argc;
	struct JsValue **argv;
};
/*
	work 		: 开启线程后, 调用该函数指针进行工作, 可以为NULL, 
					表示没有工作部分. 函数执行完毕返回的可以是
					一个 struct JsAsyncWR* 给等待调用函数作为参数
					或者NULL.
		
	data 		: 传递给work的数据包
	o    		: 完成工作后, 调用的Js函数, 可以为NULL
	openEngine	: 是否开启新引擎单元来完成该工作.
*/
JsThread JsAsync(JsThreadFn work,void* data, struct JsObject* o, int openEngine);

#endif