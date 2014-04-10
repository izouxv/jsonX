//
//  Util.cpp
//  X_V
//
//  Created by zouxu on 7/7/13.
//  Copyright (c) 2013 zouxu. All rights reserved.
//

#include "Logger.h"
#include "UtilPlatform.h"
#include "Integer.h"
#include "Bytes.h" 
#include "String.h"
#include "jsonXVdefines.h"
#include "Ptr.h"
#include <math.h>
//#include "jsonXV.h"

//Logger::Logger(const String& aFileName) {
//	iLevel = logDIYName;
//	name = aFileName;
//	(*this) << _L("\t\t\t\t\t\t\t\t\t\t\t\t\tDIY:\t") << aFileName << "\t"
//	<< curTime() << "\t";
//}
Logger::Logger(int level) {
	iLevel = level;
	switch (level) {
		case logFatal: {
			(*this) << "\tFatal:\t";
			break;
		}
		case logWarning: {
			(*this) << "\tWarning:\t";
			break;
		}
		case logInfo: {
			(*this) << "\tInfo:\t";
			break;
		}
		case logDebug: {
			//(*this) << "\tDebug:\t";
			break;
		}
		case logDebug1: {
			(*this) << "\tDebug1:\t";
			break;
		}
		case logDebug2: {
			(*this) << "\tDebug2:\t";
			break;
		}
		default: {
			//(*this) << "\tUnknown:\t";
			break;
		}
	}
	//(*this) << curTime() << "\t";
	startTime = curTime();
}


Logger::~Logger() {
	double endTime = curTime();
	//printf("%f    %f", startTime, endTime);
	double spendTime = endTime-startTime;
	

	
	(*this) << "\r\n";
	Integer time = spendTime;
	String temp = "";
	if(DOUBLE_EQUAL(spendTime,0.0))
		temp=toString();
	else
		temp=String("TIME:")+time.toString()+" S.     "+ toString();
	Bytes bytes = temp.toBytes();
	printLog((const char*)bytes.des().source);
	
	deRef();
}


