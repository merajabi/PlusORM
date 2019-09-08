#include <iostream>
#include "thread.h"
#include "mutex.h"
#include "atomic.h"

ModernCPP::Mutex m;
ModernCPP::Atomic<unsigned long> counter(0);

int inc(std::string x) {
	while(1){
		unsigned long tmp = counter.Load();
		if(tmp<10000ul) {
			ModernCPP::LockGuard lk(m);
			tmp = counter.Load();
			if(tmp<10000ul){
				tmp++;
				counter.Store(tmp);
			}
		} else {
			break;
		}
	}
	return 0;
}

class Test {
	public:
	void operator () (){
		ModernCPP::Thread t2(inc,"5");
		//inc("1");
	}
};

int main() {
	Test obj;
	int (*fp)(std::string) = inc;
	{
		ModernCPP::Thread t1(obj);

		ModernCPP::Thread t2(inc,"5");

		ModernCPP::Thread t3(fp,"5");
		//t3.Join();
		//t2.Join();
		//t1.Join();
	}
	    std::cout<<counter<<std::endl;

}

