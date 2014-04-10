/*
 ============================================================================
 Name		: StringBuilder.h
 Author	  : 
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CStringBuilder declaration
 ============================================================================
 */

#ifndef STRINGBUILDER_H
#define STRINGBUILDER_H

// INCLUDES 

//#include "String.h"
#include "Object.h"

class StringBuilder: public Object {
public:

	StringBuilder();

	StringBuilder& append(const String& value);

//	StringBuilder& append(const TChar value);
	
	StringBuilder& operator<<(const String& string);
	
	StringBuilder& operator<<(const Object& object);

	StringBuilder& operator<<(const char* things);

	StringBuilder& operator<<(int things);
	
	String toString();

};

#endif // STRINGBUILDER_H
