//
//  Test.m
//  X_V
//
//  Created by zouxu on 24/7/13.
//  Copyright (c) 2013 zouxu. All rights reserved.
//

#import "Test.h"
#import "ViewController.h"
#import "jsonXV.h"
#import "NSObject+jsonXV.h"
//#import "crc32.h"
#import <stdio.h>
#include "Util.h"
#include "jsonXV_OjbC_Test.h"

unsigned char * getChar()
{
    new char[10000];
    [NSMutableArray new];
    malloc(10000);
    unsigned char decrypt[16];
    return decrypt;
}

@implementation clss
- (void)print
{
    NSLog(@"print 1");
}

@end
@implementation clss2
- (void)print
{
    NSLog(@"print 2");
}

@end

@implementation Test

List  getList()
{
    List list;
    list.add(1);
    list.add(2);
    return list;
}

#if 0
{
    long a;
    a=0xffffffff;
    if(a>1)
        printf("64bit");
    else
        printf("32bit");
}
#endif

- (void)TEST
{
        JsonXV_OjbC_Test* test = [[JsonXV_OjbC_Test alloc]init];
 //   [test Test_facade];
   // return;
    for (int i=0; i<100; i++) {
        DEBUG<<"TEST loop: "<<i<<"\n\n";
        @autoreleasepool {
        [test Test_jsonX];
        }
       // DEBUG<<"\n\n\n\n";
    }
        [test release]; 
	return;
	{
		DEBUG<<"123";
		DEBUG<<"456";
		DEBUG<<"abc";
	}
	Integer num2 = 1234567.1234567;
	num2 = 1000;
	NSLog(@"%@", num2.toString().ID());
	
	
	
	Integer num;
	num++;
	++num;
	
	
	
	//[NSMutableDictionary new];
	//return;
	
    {
        NSLog(@" !!! START TESTING !!! \n\n");
        const int KLoopTimes = 10;
        for (int i = 0; i < KLoopTimes; i++)
        {
            NSAutoreleasePool *loopPool = [[NSAutoreleasePool alloc]init];
            [self stressTest];
			[self testObjectObjC];
            [self testOther];
            [loopPool drain];
            NSLog(@"\n\nPassed  %d / %d\n\n", i + 1, KLoopTimes);
        }
        NSLog(@" !!! END TESTING !!! \n\n");
        return;
    }
}


- (BOOL)bigger:(int)a b:(int)b
{
    return a - b;
}

- (BOOL)bigger:(int)a c:(int)b
{
    return a - b;
}

- (void)testOther
{
    {
        long a;
        a = 0xffffffff;
        if (a > 1) printf("64bit\n");
        else printf("32bit\n");
    }
	
	
    {
        u.Long = 1;
        if (u.Char[0] == 1)
        {
            printf("Little Endian!\n");
        }
        else if (u.Char[sizeof(long) - 1] == 1)
        {
            printf("Big Endian!\n");
        }
        else
        {
            printf("Unknown Addressing!\n");
        }
    }
}

@end
