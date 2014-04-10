//
//  utilPF.h
//  TestUDX
//
//  Created by zouxu on 10/1/14.
//  Copyright (c) 2014 zouxu. All rights reserved.
//

#ifndef TestUDX_utilPF_h
#define TestUDX_utilPF_h

#include "jsonXVdefines.h"
class CFacadeX;
//typedef  void (*FunPtr)(int);// (*FunP)(20);
extern void timerAfter(CFacadeX* facade, int afterTime);// 1/1000000 second


extern int64_t curTimeGen();//from 1970//  1/1000000 second
extern double curTime();//from 1970
extern void printLog(const char* log);

//MEM
extern double currentMemoryUsage();

//FILE
extern char* pwd();//new
extern bool fileRead(const char* fileName, char** content, int* length );//malloc
extern void myselfDir(const char** dir);
extern void fileWrite_Append(const char* fileName,  const char* content);
extern void fileWrite_Replace(const char* fileName,  const char* content, int length);
//extern void fileRead(const char* fileName, char** content);

extern void* thread_timer(void* arg);
#endif
