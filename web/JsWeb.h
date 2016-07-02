#ifndef JsWebH
#define JsWebH
#include"JsType.h"

struct MHD_Connection;

/*
	加载web模块
*/
void JsLoadWebModule();
/*
JS:
	getParameter(): String 						可以让您指定请求参数名称，以取得对应的设定值.
	getServerName():String						请求的服务器.
	getMethod(): "POST" | "GET" | "DELETE" ...	请求方法.
	getServerPort(): Number						请求端口号.
	getRequestURI():URI路径.-> '/main/list.do'	除去http://localhost:8080/部分的地址
*/
struct JsObject* JsCreateRequest(struct MHD_Connection *connection,
                                const char *url,
                                const char *method);
/*
Js:
	setHeader(String,String): 	是一个通用的标头设定方法，您可以用它来设定任何「名称/值」的标头.
	setStatus(Number):			状态码
	write(String) : 			写入返回文本
	clear():					清空write内容
*/
struct JsObject* JsCreateResponse(struct MHD_Connection *connection);
/*
	将response提交到queue中, 并且销毁
	@return 提交后得到的代码
*/
int JsSubmitResponse(struct JsObject* response);
#endif