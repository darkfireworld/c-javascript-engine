var count = 0;
function fun(){
	print("Go To Function");
	for(var i = 0 ; i < 1000;++i)
		var a = {};
	if(count<=10){
		count++;
		setTimeout(fun,100);
	}
	//每次会增加8byte的内存被使用, 其实是被stack占据
}
fun();