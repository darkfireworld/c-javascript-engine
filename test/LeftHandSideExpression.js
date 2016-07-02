print("===========LeftHandSide Expression ==========");
function a(){
	print("success");
};
a();
var b = function(){
	print("success");
};
b();
var mem = function(){
	return "mem";
};
var c = {mem: "success"}
print(c[mem()]);
print(c.mem);

(function(a,b,c,d){
	print(a == 1 ? "success" : "fail") ;
	print(b == true ? "success" : "fail") ;
	print(c == "string" ? "success" : "fail") ;
	print(d.toString() == "[object Object]" ? "success" : "fail") ;
	
})(1,true,"string",{});