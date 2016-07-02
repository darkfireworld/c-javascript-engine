print("===========test var statement==========");
var a = 1, b = {}, c = 2;
print( (a == 1 && b == b && c == 2) == true ? "success" : "fail");
try{
	print( varSepcDiefineVar032 == undefined);
	print("fail");
}catch(e){
	print("success");
}
cc = 21;
function hello(){
	var a = "string";
	if(a == "string")
		print("success");
	else
		print("fail");
	if(cc == 21)
		print("success");
}
hello();
var abcd = 32;
delete abcd;
print( abcd == 32 ? "success" : "fail");
bcd = 32;
print((delete bcd) == true ? "success" : "fail");
