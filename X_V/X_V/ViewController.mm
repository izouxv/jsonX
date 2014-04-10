//
//  ViewController.m
//  X_V
//
//  Created by zouxu on 14/6/13.
//  Copyright (c) 2013 zouxu. All rights reserved.
// 
#import "ViewController.h" 
#import "Test.h"
#import <sys/sysctl.h>


@interface ViewController ()

@end

@implementation ViewController




#import "mach/mach.h"

vm_size_t usedMemory(void) {
    int mib[2];
    uint64_t physicalMemorySize;
    uint64_t userMemorySize;
     size_t length;
    sysctl(mib, 2, &userMemorySize, &length, NULL, 0);
    return length;
    struct task_basic_info info;
    mach_msg_type_number_t size = sizeof(info);
    kern_return_t kerr = task_info(mach_task_self(), TASK_BASIC_INFO, (task_info_t)&info, &size);
    return (kerr == KERN_SUCCESS) ? info.resident_size : 0; // size in bytes
}

vm_size_t freeMemory1(void) {
    mach_port_t host_port = mach_host_self();
    mach_msg_type_number_t host_size = sizeof(vm_statistics_data_t) / sizeof(integer_t);
    vm_size_t pagesize;
    vm_statistics_data_t vm_stat;
    
    host_page_size(host_port, &pagesize);
    (void) host_statistics(host_port, HOST_VM_INFO, (host_info_t)&vm_stat, &host_size);
    return vm_stat.free_count * pagesize;
}

natural_t  freeMemory(void) {
    mach_port_t           host_port = mach_host_self();
    mach_msg_type_number_t   host_size = sizeof(vm_statistics_data_t) / sizeof(integer_t);
    vm_size_t               pagesize;
    vm_statistics_data_t     vm_stat;
    
    host_page_size(host_port, &pagesize);
    
    if (host_statistics(host_port, HOST_VM_INFO, (host_info_t)&vm_stat, &host_size) != KERN_SUCCESS) NSLog(@"Failed to fetch vm statistics");
    
    natural_t   mem_used = (vm_stat.active_count + vm_stat.inactive_count + vm_stat.wire_count) * pagesize;
    natural_t   mem_free = vm_stat.free_count * pagesize;
    natural_t   mem_total = mem_used + mem_free;
    
    return mem_free;
}

void logMemUsage(void) {
    // compute memory usage and log if different by >= 100k
    static long prevMemUsage = 0;
    long curMemUsage = usedMemory();
    long memUsageDiff = curMemUsage - prevMemUsage;
    
   // if (memUsageDiff > 100000 || memUsageDiff < -100000)
    {
        prevMemUsage = curMemUsage;
        NSLog(@"Memory used %7.1f (%+5.0f), free %7.1f kb", curMemUsage/1000.0f, memUsageDiff/1000.0f, freeMemory()/1000.0f);
   //      NSLog(@"Memory used %ld (%+5.0f), free %7.1f kb", curMemUsage, memUsageDiff/1000.0f, freeMemory()/1000.0f);
    }
}

-(void)TEST{
    
//    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT,
//                                             (unsigned long)NULL), ^(void) {
//        [self getResultSetFromDB:docids];
//    });
	Test* test = [Test new];
	[test TEST];
	[test release];
}

int getInt(){
	return 100;
}

- (void)viewDidLoad
{
	NSLog(@"%d", getInt());
    
    
    [super viewDidLoad];

    [self performSelectorInBackground:@selector(TEST) withObject:nil];
    
	
	//return;
	
#if 0
	self.progressView.progress = 0.83;
    self.unicode.text = String("this is object str");
	
	String str = self.unicode.text.obj;
	
	Bytes bye = str.getBytes();
	NSData * data = bye;
		NSLog(@"%@", data);
	Bytes bye2 = data.obj;
	String str2 = bye2.toString();
	self.unicode.text = str2;
#endif
	
	
	
}


- (IBAction)openCamera:(id)sender {
	
	 

}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
 
- (void)dealloc {
	[_img release];
	[_progressView release];
	[_choiced release];
    [_text release];
    [_unicode release];
	[super dealloc];
}
- (IBAction)choice:(id)sender {
}
@end
