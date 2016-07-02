print("===========Equality Expression==========");
if(undefined == undefined )
	print("success");
else
	print("fail");
	
if(null == null )
	print("success");
else
	print("fail");

if(NaN == 32 )
	print("fail");
else
	print("success");
	
if(32 == NaN )
	print("fail");
else
	print("success");
	
if(0 == 0 )
	print("success");
else
	print("fail");
	
if(1 == 0 )
	print("fail");
else
	print("success");

if(10000.32 == 10000.32 )
	print("success");
else
	print("fail");
	
if("abcd" == "abcd")
	print("success");
else
	print("fail");
	
if("abcd" == "abc")
	print("fail");
else
	print("success");

if(false == false)
	print("success")
else
	print("fail");
	
if(null == undefined)
	print("success")
else
	print("fail")
	
	
	
if(undefined == null)
	print("success")
else
	print("fail")
	
	
var str = new String("abcd");
if("abcd" == str)
	print("success");
else
	print("fail");
	
var bool = new Boolean(false);

if(bool == true)
	print("Mock fail");
else
	print("success");
	
var num = new Number("12.21");
if(num == 12.21)
	print("success");
else	
	print("fail");
	
var c = "12.21"
var d = 12.21
if(c == d)
	print("success");
else
	print("fail");