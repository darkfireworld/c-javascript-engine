
print("===========UnaryExpression Operator ==========");

var a = 1;
print((delete a) == false ? "success" : "fail");
uniona =1;
print((delete uniona) == true ? "success":"fail");
print(void a == undefined ? "success" : "fail");


print(typeof null == "null")
print(typeof undefined == "undefined")
print(typeof 1 == "number")
print(typeof "abcd" == "string")
print(typeof {} == "object")
print(typeof true == "boolean")

i  = 9;
print( ++i == 10 ? "success" : "fail");
print( --i == 9 ? "success" : "fail");


print(!true  == false ? "success" : "fail");