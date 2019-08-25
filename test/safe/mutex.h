namespace ModernCPP {
	class Mutex {
		typedef enum {INIT,LOCKED,UNLOCKED} LockStatus;
		LockStatus status;
		pthread_mutex_t mutex;// = PTHREAD_MUTEX_INITIALIZER;
		public:
			Mutex():status(INIT){
				if (pthread_mutex_init(&mutex, NULL) == 0) { 
					status = UNLOCKED;
				} 
			}
			~Mutex(){
			    pthread_mutex_destroy(&mutex); 
			}
			void Lock(){
				if(pthread_mutex_lock( &mutex )==0){
					status = LOCKED;
				}
			}
			void Unlock(){
				if(pthread_mutex_unlock( &mutex )==0){
					status = UNLOCKED;
				}
			}
			LockStatus GetStatus() {return status;}
	};
	class LockGuard {
		Mutex &m;
		public:
		LockGuard(Mutex& m):m(m){
			m.Lock();
		}
		~LockGuard(){
			m.Unlock();
		}
	};
};




