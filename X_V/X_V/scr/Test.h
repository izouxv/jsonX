//
//  Test.h
//  X_V
//
//  Created by zouxu on 24/7/13.
//  Copyright (c) 2013 zouxu. All rights reserved.
//

#import <Foundation/Foundation.h>
 

static const int KmaxCount = 1000000;

@interface clss : NSObject

@end
@interface clss2 : clss
-(void)print;
@end



union {
	long Long;
	char Char[sizeof(long)];
}u;



@interface Test : NSObject
-(void)TEST;
@end
