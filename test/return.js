print("===========test return statement==========");
var c = function(){
	return "a";
};
if(c() == "a")
	print("success");
else
	print("fail");