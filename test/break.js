print("===========test break statement==========");
var i = 0
do{
	i++;
	break;
	print("fail");
}while(i != 10);

print("success");
i = 0;
while(i<10){
	break;
	print("fail");
}

	print("success");
for(var i = 0 ; i < 10 ; ++ i){
	break;
	print("fail");
}

print("success");

var obj = {a:1,b:3,d:3};
for(var name in obj){
	break;
	print("fail");
}
print("success");