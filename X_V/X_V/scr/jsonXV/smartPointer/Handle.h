/*
 ============================================================================
 Name		: Handle.h
 Author	  : 
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CHandle declaration
 ============================================================================
 */

#ifndef HANDLE_H
#define HANDLE_H

// INCLUDES
#include "Object.h"
class String;
class Bytes;

// CLASS DECLARATION
namespace XV {

class Handle {
	friend class Object;
public:

	virtual ~Handle();

	Handle();

	bool equals(Handle* handle);

	int getRefCount();

//protected://xcode 5, error

	virtual ObjectType getType()=0;

	virtual void destroy()=0;

	virtual bool compare(Handle* handle)=0;

	virtual int hash();
    
   virtual int sizeOf();

	int refCount;
	virtual String toString()=0;
	virtual Bytes toBytes()=0;
	
private:
	void incRef();

	int deRef();

#ifdef _DEBUG
public:
	static TInt handleCount;
#endif
};
};



#endif // HANDLE_H
