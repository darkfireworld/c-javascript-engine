print("===========ArrayLiteral==========");
var array = [1,2,3,4];
for(var i = 0 ; i < array.length ; ++i)
	if(array[i] != i+1)
		print("fail");
	else
		print("success");
		
