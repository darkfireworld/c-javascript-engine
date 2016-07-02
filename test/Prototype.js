print("============= Test Prototype============");
var Class = function(name){
	this["name"] = name;
};
Class.prototype = {soup : 21};
var C1 = new Class("C1");
var C2 = new Class("C2");
print(C1.name == "C1" ? "success" : "fail");
print(C2.name == "C2" ? "success" : "fail");

print(C1.soup == 21 ? "success" : "fail");
print(C2.soup == 21 ? "success" : "fail");

Class.prototype.soup = 12;
print(C1.soup == 12 ? "success" : "fail");
print(C2.soup == 12 ? "success" : "fail");

C1.soup = "c1";
print(C1.soup == "c1" ? "success" : "fail");
print(C2.soup == 12 ? "success" : "fail");

	