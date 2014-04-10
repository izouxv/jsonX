//
//  Util.h
//  X_V
//
//  Created by zouxu on 7/7/13.
//  Copyright (c) 2013 zouxu. All rights reserved.
//

#include "jsonXVdefines.h"
#if CPP
#include <sys/time.h>
#include "UtilPlatform.h"
#include <sys/sysctl.h>
#include <mach/mach.h>
#include "FacadeX.h"
#include <iostream>

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
//CFacadeX*facade;
//timespec curTimeAdd(int asubs){
//    struct timeval tv;
//    gettimeofday(&tv,NULL);
//    timespec ts; 
//    ts.tv_sec = tv.tv_sec+ asubs/1000000;
//    ts.tv_nsec = tv.tv_usec+ asubs%1000000;
//    return ts;
//}
//void* thread_timer(void* arg){
//    while (true) {
//        usleep(100000);
//        if(facade)facade->TimerExpired();
//    }
//    pthread_exit(0);
//}
// 
//void timerAfter(CFacadeX* facad,  int afterT){
//    facade=facad;
//}
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

int64_t curTimeGen(){//by_zouxu
    return curTime()*1000000;
//    return [[NSDate date] timeIntervalSince1970]*1000000;
}

double curTime(){
//    time_t mytime;
//    mytime = time(NULL);
//   // printf(ctime(&mytime));
//    return mytime;
//    
    struct timeval tv;
    gettimeofday(&tv,NULL);
 //  printf("time %u:%u\n",tv.tv_sec,tv.tv_usec);
    double curT =tv.tv_usec;
    curT/=1000000;
    curT+=tv.tv_sec;
    return curT;
}
void printLog(const char* log){
	printf("%s", log);
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

extern char* pwd(){
//    char the_path[256];
    char* the_path=new char[256];
    getcwd(the_path, 255);
    strcat(the_path, "/");
   // printf("%s\n", the_path);
    return the_path;
}
extern bool fileRead(const char* fileName, char** content, int* length ){
//    char *source = NULL;
    FILE *fp = fopen(fileName, "r");
    bool ret = false;
    if (fp != NULL) {
        /* Go to the end of the file. */
        if (fseek(fp, 0L, SEEK_END) == 0) {
            /* Get the size of the file. */
            long bufsize = ftell(fp);
            (*length) = bufsize;
            if (bufsize == -1) { /* Error */
                fclose(fp);
                return false;
            }
            
            /* Allocate our buffer to that size. */
            *content =(char *) malloc(sizeof(char) * (bufsize + 1));
            
            /* Go back to the start of the file. */
            if (fseek(fp, 0L, SEEK_SET) == 0) { /* Error */ }
            
            /* Read the entire file into memory. */
            size_t newLen = fread(*content, sizeof(char), bufsize, fp);
            if (newLen == 0) {
                fputs("Error reading file", stderr);
            } else {
                (*content)[++newLen] = '\0'; /* Just to be safe. */
                ret=true;
            }
            
        }
        fclose(fp);
        return ret;
    }
    return ret;
   // free(source); /* Don't forget to call free() later! */
 
}
extern void myselfDir(const char** dir){
    char the_path[256];
    getcwd(the_path, 255);
    strcat(the_path, "/");
//    strcat(the_path, argv[0]); 
    printf("%s\n", the_path);
}
extern void fileWrite_Append(const char* fileName, const char* content){
	
}

extern void fileWrite_Replace(const char* fileName, const char* content, int length){
 
}
//extern void fileRead(const char* fileName,  char** content){
//    FILE *fp;
//    char hc1,hc2,mc1,mc2;
//    int hi1,hi2,mi1,mi2,hour,minute;
//    system("echo %time% >time.txt");
//    fp=fopen("time.txt","r");
//    if(fp==NULL)
//        exit(1) ;
//    hc1=fgetc(fp);
//    hc2=fgetc(fp);
//    fgetc(fp);
//    mc1=fgetc(fp);
//    mc2=fgetc(fp);
//    fclose(fp);
//    remove("time.txt");
//    hi1=hc1;
//    hi2=hc2;
//    mi1=mc1;
//    mi2=mc2;
//    hi1-=48;
//    hi2-=48;
//    mi1-=48;
//    mi2-=48;
//    hour=hi1*10+hi2;
//    minute=mi1*10+mi2;
//    printf("Current time is %d:%d\n",hour,minute);
//   // return 0;
//}
#endif
