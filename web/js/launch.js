
//开启Web服务器, 该API 是开发者提供的.
startServer(function(request,response){
	//设置minetype = json
	response.setHeader('Content-Type',"application/json; charset=utf-8");
	//如果用户名和密码相等, 则返回true
	if(request.getParameter("username") 
		== request.getParameter("passwd")){
		response.write('{"result": true}');
	}else{
		response.write('{"result": false}');
	}
	//监听在8080端口, 而lightppd 就把*.do的请求转发到本机的8080端口给js虚拟机处理
},8080);