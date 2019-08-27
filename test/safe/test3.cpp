#include <iostream>
#include "thread.h"
#include "mutex.h"
#include "atomic.h"

ModernCPP::Mutex m;
ModernCPP::Atomic<unsigned long> counter(0);

void inc(int x) {
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
}

class Test {
	public:
	void operator () (){
		inc(1);
	}
};

int main() {
	Test obj;
	void (*fp)(int) = inc;
	ModernCPP::Thread<Test&> t1(obj);
    ModernCPP::Thread<void(*)(int),int> t2(inc,5);
    ModernCPP::Thread<typeof(fp),int> t3(fp,5);		// only with gnu++98
    t3.Join();
    t2.Join();
    t1.Join();

    std::cout<<counter<<std::endl;
}

