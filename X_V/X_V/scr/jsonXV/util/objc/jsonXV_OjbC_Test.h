//
//  jsonXV_OjbC_Test.h
//  X_V
//
//  Created by zouxu on 23/7/13.
//  Copyright (c) 2013 zouxu. All rights reserved.
//



#if 0
static const int K_Loop_Huge = 10000000;
static const int K_Loop_Small =100000;
static const int K_OCOBJC_CMP =5000;
#else
static const int K_Loop_Huge = 100000;
static const int K_Loop_Small =10000;
static const int K_OCOBJC_CMP =1000;
#endif



@interface JsonXV_OjbC_Test : NSObject
-(void)Test_jsonX;
-(void)Test_facade;
@end
