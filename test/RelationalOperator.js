print("===========Relational Operator==========");

if(undefined <= undefined )
	print("fail");
else
	print("success");
	
if(null < null )
	print("fail");
else
	print("success");

if(null <= null )
	print("success");
else
	print("fail");
	
if(NaN == 32 )
	print("fail");
else
	print("success");
	
if(NaN < 32 )
	print("fail");
else
	print("success");

if(NaN > 32 )
	print("fail");
else
	print("success");

if(NaN >= 32 )
	print("fail");
else
	print("success");

if(NaN <= 32 )
	print("fail");
else
	print("success");
		
if(0 < 0 )
	print("fail");
else
	print("success");

if(0 <= 0 )
	print("success");
else
	print("fail");

if(0 >= 0 )
	print("success");
else
	print("fail");
	
if(1 > 0 )
	print("success");
else
	print("fail");
if(1 >= 0 )
	print("success");
else
	print("fail");	
	
if(1 < 0 )
	print("fail");
else
	print("success");

if(1 <= 0 )
	print("fail");
else
	print("success");
	
if(10000.32 > 10000.32 )
	print("fail");
else
	print("success");

if(10000.32 >= 10000.32 )
	print("success");
else
	print("fail");
	
if("abcd" < "abcd")
	print("fail");
else
	print("success");
	
if("abcd" <= "abcd")
	print("success");
else
	print("fail");	
	
	
if("abcd" > "abc")
	print("success");
else
	print("fail");

if("abcd" >= "abc")
	print("success");
else
	print("fail");	
	
if(false >= false)
	print("success")
else
	print("fail");
	
if(null >= undefined)
	print("fail")
else
	print("success")
	
	
var str = new String("abcd");
if("abc" < str)
	print("success");
else
	print("fail");
	
var bool = new Boolean(false);

	
var num = new Number("12.21");
if(num > 12)
	print("success");
else	
	print("fail");
	
var c = "12.2132"
var d = 12.21
if(c > d)
	print("success");
else
	print("fail");
	
	
	
var  create = function(){};
var c1 = new create();
if(c1 instanceof create)
	print("success");
else
	print("fail");
var obj = {a:3,b:32};
if('a' in obj)
	print("success");
else
	print("fail");
	
if('c' in obj)
	print("fail");
else
	print("success");
