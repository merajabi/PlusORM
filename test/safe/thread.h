namespace ModernCPP {
	template<typename FuncType,typename ParamType=void>
	class Thread {
		pthread_t threadId;
		struct ThreadData {
			FuncType func;
			ParamType param;
			ThreadData(FuncType func,ParamType param):func(func),param(param){};
		} data ;
		public:
			Thread(FuncType functor,ParamType param):data(functor,param){
				pthread_create( &threadId, NULL, &Thread::function, &data);
			}
			Thread(FuncType functor):data(functor,NULL){
				pthread_create( &threadId, NULL, &Thread::function, &data);
			}
			~Thread(){
				pthread_join( threadId, NULL);
			}
			void Join(){
				pthread_join( threadId, NULL);
			}
			static void *function(void *data){
				
				((ThreadData*)data)->func( ((ThreadData*)data)->param );
			}
	};
	template<typename FuncType>
	class Thread <FuncType,void> {
		pthread_t threadId;
		struct ThreadData {
			FuncType func;
			ThreadData(FuncType func):func(func){};
		} data ;
		public:
			Thread(FuncType functor):data(functor){
				pthread_create( &threadId, NULL, &Thread::function, &data);
			}
			~Thread(){
				pthread_join( threadId, NULL);
			}
			void Join(){
				pthread_join( threadId, NULL);
			}
			static void *function(void *data){
				((ThreadData*)data)->func( );
			}
	};

};
