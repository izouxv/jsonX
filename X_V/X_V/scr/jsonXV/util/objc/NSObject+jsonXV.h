//
//  NSObject+jsonXV.h
//  X_V
//
//  Created by zouxu on 23/7/13.
//  Copyright (c) 2013 zouxu. All rights reserved.
//

#import <Foundation/Foundation.h>

class Object;
@interface NSObject (jsonXV)//only for NSArray NSDictionary NSString NSNumber NSData 
+(id)fromObj:(Object)obj;
-(Object)obj;
@end
