//
//  xdefines.h
//  X_V
//
//  Created by zouxu on 28/6/13.
//  Copyright (c) 2013 zouxu. All rights reserved.
//

#ifndef X_V_xdefines_h
#define X_V_xdefines_h


//#define SYMBIAN 1
#define OBJC 0
#define CPP 0

#undef OBJC
#define OBJC 1
//#define CPP 1

#define INLINE inline 
#undef __WINS__



typedef void TAny;
typedef signed char TInt8;
typedef unsigned char TUint8;
typedef short int TInt16;
typedef unsigned short int TUint16;
typedef long int TInt32;
typedef unsigned long int TUint32;
typedef signed int TInt;
typedef unsigned int TUint;
typedef float TReal32;
typedef double TReal64;
typedef double TReal;
typedef unsigned char TText8;
typedef unsigned short int TText16;
typedef int TBool;
typedef long long TInt64 ;

typedef	long long		int64_t;

#define DOUBLE_EQUAL(a,b) (fabs(a-b) <= 1e-6)


#define _DEBUG
#define LEAVE(a)
#undef NULL
#define NULL 0
#define ELeave 0
#define KErrNotFound -1
#define null 0
#define ETrue 1
#define EFalse 0
#define SAFE_DEL(aPtr) if(aPtr) { delete aPtr; aPtr = NULL;}
//#define NOTFOUND(err)		LEAVE(err);\
//							OBJECTNOTFOUND.deRef();\
//                            return OBJECTNOTFOUND;
//OBJECTNOTFOUND.clean();\

#define NOTFOUND(err)		LEAVE(err);\
							OBJECTNOTFOUND.clean();\
							return OBJECTNOTFOUND;




#if SYMBIAN
#include <e32std.h>
#include <e32base.h>
#endif


#if OBJC
#undef INLINE
#define INLINE
//#undef LEAVE
//#define LEAVE(err) printf("SP err:%d\n",err);
#import <stdio.h>
#import "ObjBase.h"
#define STR(str)  static NSString* const K_##str=@#str;
#endif

#if CPP
#define STR(str)  static const String K_##str(#str);//C++
#endif


#define PRINT_NODE(node) printf("\nNODE :%d\n%s\n", __LINE__, node->__Print().toString().getBytes().des().source);
#define PRINT_DYNA(node) printf("\nDYNA :%d\n%s\n", __LINE__, node->__Print().toString().getBytes().des().source);
#define PRINT_LIST(node) printf("\nLIST :%d\n%s\n", __LINE__, node->__Print().toString().getBytes().des().source);
#define PRINT_OBJ(obj)   printf("\nOBJ :%d\n%s\n", __LINE__, obj.toString().getBytes().des().source);














































#endif
