print("============= Test Clouse============");
var test1 = {
	hello:'hello',
	good:'good'
};
var test2 = {};
for(var name in test1){
	test2[name] = (function(name){
		return function(){
			return name;
		}
	})(name);
}
print(test2['hello']() == "hello" ? "success":"fail");
print(test2['good']() == "good" ? "success":"fail");
	