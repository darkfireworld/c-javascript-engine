print("==================test sync Print ==========");
synchronized function funa(id){
	for(var i = 0 ; i < 10;++i){
		print("sync function thread ["+id+"]: "+i);
	}
}
var i = 0;
var lock = {};
var fCreateNewThread = function(id,fun){
	Thread.start(function(){
		synchronized(lock){
			for(var i = 0 ; i < 10;++i)
				print("sync block thread ["+id+"]: "+i);
		}
		funa(id);
	});
};
for(var i = 0 ; i < 5 ;++i)
	fCreateNewThread(i);