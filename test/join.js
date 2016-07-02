var a = 0;
synchronized function inc() {
  a++;
}
function run(){
	for (var i = 0; i < 5; i++) {
		inc();
	}
}
var t1 = Thread.start(run);
t1.join();
Thread.yield();
print(a);