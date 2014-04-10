//
//  NSObject+xSP.m
//  X_V
//
//  Created by zouxu on 23/7/13.
//  Copyright (c) 2013 zouxu. All rights reserved.
//

#import "NSObject+jsonXV.h"
//#include "xSP.h"
#include "Integer.h"
#include "Handle.h"
#include "List.h"
#include "Pair.h"
#include "Bytes.h"
#include "String.h"
#include "Map.h"
#include "Ptr.h"



@implementation NSObject (xSP)
+ (id)fromObj:(Object)obj
{
    ObjectType type = obj.getType();
    switch (type){
        case OT_Integer:{//by_zouxu int64 not consider
            if(Integer(obj)._TInt64()<Integer(obj)._double())
                return [NSNumber numberWithDouble:Integer(obj)._double()];
			return [NSNumber numberWithInt:Integer(obj)._TInt64()];
			break;
		}
        case OT_String:{
			String str(obj);
			TUint16*source = str.des().source;
			int length=str.des().length;
			return [NSString stringWithCharacters:source length:length];
			break;
		}
        case OT_Bytes:{
			Bytes bytes(obj);
			TUint8*source = bytes.des().source;
			int length=bytes.des().length;
			return [NSData dataWithBytes:source length:length];
			break;
		}
        case OT_List:{
			NSMutableArray* array = [NSMutableArray new];
			List list(obj);
			for(int i=0; i<list.size(); i++){
				Object tmpObj = list.get(i);
				id tmpObjC = [NSObject fromObj:tmpObj];
				[array addObject:tmpObjC];
			}
            return array;
//			return [array autorelease];
			break;
		}
        case OT_Map:{
			Map map(obj);
			List keyList = map.getKeys();
			
			NSMutableDictionary* dic = [NSMutableDictionary new];
			for(int i=0; i<keyList.size(); i++){
				Object keyObj = keyList.get(i);
				Object valueObj = map.get(keyObj);
				id keyObjC = [NSObject fromObj:keyObj];
				id valueObjC = [NSObject fromObj:valueObj];
				[dic setObject:valueObjC forKey:keyObjC];
			}
            return dic;
//			return [dic autorelease];
			break;
		}
		case OT_Object:
        default:{
			//NSLog(@"123 %@", [NSThread callStackSymbols] );
			//NSLog(@"toObjC Error: type not available");
			//return @"";
            LEAVE(ET_TypeError_NotFound); 
            break;
		}
    }
	return nil;
}
-(Object)obj
{
	if([self isKindOfClass:[NSArray class]]){
		NSArray* tmp = (NSArray*)self; 
		List list;
		for(int i=0; i<tmp.count;i++){
			id itemId = [tmp objectAtIndex:i];
			Object item = [itemId obj]; 
			list.add(item);
		}
		return list;
	}
	else if([self isKindOfClass:[NSDictionary class]]){
		NSDictionary* tmp = (NSDictionary*)self;
		NSArray * allKeys = [tmp allKeys];
		Map map;
		for(int i=0; i<allKeys.count;i++){
			id keyId = [allKeys objectAtIndex:i];
			id valueId = [tmp objectForKey:keyId];
			Object key = [keyId obj];
			Object value = [valueId obj];
			map.put(key, value);
		}
		return map;
	}
	else if([self isKindOfClass:[NSString class]]){
		NSString* tmp =(NSString*) self;
		const char * typein = [tmp UTF8String];
		return String(typein);
	}
	else if([self isKindOfClass:[NSData class]]){
		NSData* tmp = (NSData*)self;
	//	int length = tmp.length;
		const void* buffer = tmp.bytes;
		return Bytes((const char* )buffer, tmp.length);
	}
	else if([self isKindOfClass:[NSNumber class]]){
		NSNumber* tmp = (NSNumber*)self;
        //by_zouxu, need to test
		return Integer([tmp doubleValue]);
	}else{
		//NSLog(@"toObject Error: type not available");
	}
	//return Object();
	NOTFOUND(ET_NotFound_ObjC)
}
@end
