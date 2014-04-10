//
//  Util.h
//  X_V
//
//  Created by zouxu on 7/7/13.
//  Copyright (c) 2013 zouxu. All rights reserved.
//

#ifndef __X_V__Logger__
#define __X_V__Logger__

#include "StringBuilder.h"

enum LOG_LEVEL {
	logFatal = 0,
	logWarning = 1,
	logInfo = 2,
	logDebug = 3,
	logDebug1 = 4,
	logDebug2 = 5,
	logDIYName = 7,
	
	logPerformance = 1<<4,
};
#define MAX_LOG_LEVEL  logPerformance //logFatal

#define FILELINE <<"#### FUN: "<<String(__FUNCTION__)<<"  LINE: "<<Integer(__LINE__).toString()<<"\r\n"//<<"  File: "<<String(__FILE__)
#undef FILELINE
#define FILELINE <<"  File: "<<String(__FILE__)<<"  LINE: "<<Integer(__LINE__).toString()<<"   "

#define DIYLOG(FILENAME)	if (logDIYName <= MAX_LOG_LEVEL)Logger(FILENAME)FILELINE
#define DEBUG	if (logDebug <= MAX_LOG_LEVEL)Logger(logDebug)FILELINE
#define INFO	if (logInfo <= MAX_LOG_LEVEL) Logger(logInfo)FILELINE
#define WARN	if (logWarning <= MAX_LOG_LEVEL) Logger(logWarning)FILELINE
#define FATAL	if (logFatal <= MAX_LOG_LEVEL)Logger(logFatal)FILELINE
#define LOGGER(level)		if (level <= MAX_LOG_LEVEL) Logger(level)FILELINE
#define PERFOR Logger log(logDebug);log


class Logger: public StringBuilder {
public:
	Logger(int level);
	//Logger(const String& aFileName);
	virtual ~Logger();
	int iLevel;
	double startTime;
	//String name;
};

#endif /* defined(__X_V__Logger__) */
