print("===========test switch statement==========");

switch(1){
case 0: print("fail");break;
case 1: print("success");
case 2: print("success");break;
case 3: print("fail");
default:
	print("fail");
}

switch(3){
case 0: print("fail");break;
case 1: print("fail");
case 2: print("fail");break;
case 3: print("success");
default:
	print("success");
}
switch(21){
case 0: print("fail");break;
case 1: print("fail");
case 2: print("fail");break;
case 3: print("fail");
default:
	print("success");
}

switch(21){
case 0: print("fail");break;
case 1: print("fail");
case 2: print("fail");break;
default:
	print("success");
case 3: print("success");

}