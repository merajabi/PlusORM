#ifndef _POS_DEBUG_H
#define _POS_DEBUG_H

#ifndef NULL
	#define NULL 0
#endif 

#if (__cplusplus < 201103L)
	#ifndef nullptr
		#define nullptr NULL
	#endif
#endif 

#ifndef posDebug
		#define posDebug(args...) MyPosDebug(args)
//		#define posDebug(args...) printf(args)
#endif //#ifndef debugf

#ifdef __cplusplus
        extern "C" {
#endif

void MyPosDebug(const char* format, ...);
int OpenLog();
int CloseLog();

#ifdef __cplusplus
        };
#endif

#endif //_POS_DEBUG_H

