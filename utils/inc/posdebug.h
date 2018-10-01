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
	#ifdef POS_DEBUG
		#ifdef HWX86QT
			#include <QDebug>
			#define posDebug(args...) qDebug(args)
		#else
	// __VA_ARGV__
			#include <stdio.h>
			#define posDebug(args...) printf(args)
		#endif
	#else // DEBUG
		#define posDebug(args...) MyPosDebug(args)
	#endif // DEBUG
#endif //#ifndef debugf

void MyPosDebug(const char* format, ...);

#endif //_POS_DEBUG_H

