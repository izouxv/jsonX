//
//  Util.h
//  X_V
//
//  Created by zouxu on 7/7/13.
//  Copyright (c) 2013 zouxu. All rights reserved.
//

#include "UtilPlatform.h"
#include <sys/sysctl.h>
#include <mach/mach.h>
#include "FacadeX.h"

void timerAfter(CFacadeX* facade,  int afterTime){
    if(afterTime<=0){
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT,
                                                (unsigned long)NULL), ^(void) {
             facade->TimerExpired();
      });
    }else{
    double delayInSeconds =afterTime / 1000000 ;
    dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, (int64_t)(delayInSeconds * NSEC_PER_SEC));
    dispatch_after(popTime, dispatch_get_main_queue(), ^(void){
        facade->TimerExpired();
    });
    }
}

int64_t curTimeGen(){//by_zouxu
    return [[NSDate date] timeIntervalSince1970]*1000000;
}

double curTime(){
	return [[NSDate date] timeIntervalSince1970];
}
void printLog(const char* log){
	NSLog(@"%s", log);
}


double currentMemoryUsage() {
    vm_statistics_data_t vmStats;
    mach_msg_type_number_t infoCount = HOST_VM_INFO_COUNT;
    kern_return_t kernReturn = host_statistics(mach_host_self(), HOST_VM_INFO, (host_info_t)&vmStats, &infoCount);
    
    if(kernReturn == KERN_SUCCESS)
        return vmStats.wire_count;///1024.0;
    else return 0;
}



//FILE
extern void myselfDir(const char** dir){
	
}
extern void fileWrite_Append(const char* fileName, const char* content){
	
}
NSString* getFilePath(const char* fileName){
	NSString *dir = @"XV";
	NSString *ext = @".json";
	
	NSString *file_Name = [NSString stringWithUTF8String:fileName];
	
	//create base directory
	NSError* error= nil;
	NSString *baseDir = [NSHomeDirectory() stringByAppendingPathComponent:@"JSON"];
	if (![[NSFileManager defaultManager] fileExistsAtPath:baseDir])
		[[NSFileManager defaultManager] createDirectoryAtPath:baseDir withIntermediateDirectories:NO attributes:nil error:&error];
	
	NSString* targetDir= [baseDir stringByAppendingPathComponent:dir];
	if (![[NSFileManager defaultManager] fileExistsAtPath:targetDir])
		[[NSFileManager defaultManager] createDirectoryAtPath:targetDir withIntermediateDirectories:NO attributes:nil error:&error];
	
	NSString * filePath = [NSString stringWithFormat:@"%@/%@%@",targetDir, file_Name, ext ];
//	NSLog(@"fileName: %@", file_Name);
//	NSLog(@"%@", filePath);
	return filePath;
}
extern void fileWrite_Replace(const char* fileName, const char* content, int length){
	NSData* data = [NSData dataWithBytes:content length:length];
	@synchronized(data){
		NSString* filePath = getFilePath(fileName);
		@try
		{
			NSError *error = nil;
			if ([data writeToFile:filePath options:NSDataWritingAtomic error:&error])return;// return filePath;
			if (error.code == NSFileNoSuchFileError || error.code == NSFileWriteFileExistsError)
			{
				if ([data writeToFile:filePath options:NSDataWritingAtomic error:&error])return;// return filePath;
			}
			NSLog(@"Error writing file at %@", filePath);
		}
		@catch (NSException *e)
		{
			NSLog(@"Error writing Exception file at %@", filePath);
		} 
		 
	}
}

extern bool fileRead(const char* fileName, char** content, int* length ){
	NSString* filePath = getFilePath(fileName);
	NSData* data = [NSData dataWithContentsOfFile:filePath];
	*content = (char*)data.bytes;
    *length=data.length;
    if(data.length>0)return true;
    else return false;
}
