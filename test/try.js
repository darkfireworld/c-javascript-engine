print("===========test try statement==========");
var fun = function(){
	try{
		throw "e";
	}catch(e){
		if(e == "e")
		print("success");
	else
		print("fail");
		return 0;
	}finally{
		throw "finish";
	}
}
try{
	fun()
}catch(e){
	if(e == "finish")
		print("success");
	else
		print("fail");
}


