namespace ModernCPP {
	struct ThreadDataBase {
		virtual ~ThreadDataBase(){};
	};

	template<typename FuncType,typename ParamType=void>
	struct ThreadData : public ThreadDataBase {
		FuncType func;
		ParamType param;
		ThreadData(FuncType func,ParamType param):func(func),param(param){};
		static void *function(void *data){
			((ThreadData*)data)->func( ((ThreadData*)data)->param );
		}
	};

	template<typename FuncType>
	struct ThreadData<FuncType,void> : public ThreadDataBase {
		FuncType func;
		ThreadData(FuncType func):func(func){};
		static void *function(void *data){
			((ThreadData*)data)->func( );
		}
	};

	class Thread {
		bool status;
		pthread_t threadId;
		ThreadDataBase* data ;
		public:
			template<typename FuncType,typename ParamType>
			Thread(FuncType functor,ParamType param):data( new ThreadData<FuncType,ParamType>(functor,param) ){
				pthread_create( &threadId, NULL, &ThreadData<FuncType,ParamType>::function, data);
			}

			template<typename FuncType>
			Thread(FuncType functor):data( new ThreadData<FuncType>(functor)){
				pthread_create( &threadId, NULL, &ThreadData<FuncType>::function, data);
			}

			~Thread(){
				if(status){
					Join();
				}else{
					Detach();
				}
				delete data;
			}
			void Join(){
				pthread_join( threadId, NULL);
			}
			void Detach() {
				pthread_detach(threadId);
			}					
	};

};
