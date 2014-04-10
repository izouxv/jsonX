/*
 ============================================================================
 Name		: String.h
 Author	  : 
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CString declaration
 ============================================================================
 */

#ifndef GENPTR_H
#define GENPTR_H

#include "Object.h"
class GenPtrHandle;
class String;

class GenPtrBase;
class GenPtr: public Object {
public:
	GenPtr(GenPtrBase * ptr, bool autoDel);
	GenPtr(const Object& object);
	GenPtrBase* getPtr() const;
#ifdef __WINS__
	void updateHandle(Handle* value);
	GenPtrHandle* internal;
#endif
};

class GenPtrBase {
public:
	GenPtrBase();
	virtual ~GenPtrBase();
	GenPtr genPtr;
};
#endif // STRING_H
