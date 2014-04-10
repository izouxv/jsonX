/*
 ============================================================================
 Name		: Handle.cpp
 Author	  : 
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CHandle implementation
 ============================================================================
 */

#include "Handle.h" 
#include "Object.h"
#include "String.h"

//	char type4test;
namespace XV {
Handle::Handle() {
	refCount = 1;
#ifdef _DEBUG
	handleCount++;
#endif
}

Handle::~Handle() {
#ifdef _DEBUG
	handleCount--;
#endif
}

void Handle::incRef() {
	refCount++;
}

int Handle::deRef() {
	refCount--;
	return refCount;

}

ObjectType Handle::getType() {
	return OT_Object;
	//	return type;
}
int Handle::getRefCount() {
	return refCount;
}
bool Handle::equals(Handle* handle) {
	bool result = (this == handle) || (handle && (this->getType()
			== handle->getType()) && compare(handle));
	return result;
}

//bool Handle::compare(Handle* handle) {
//	return false;
//}

int Handle::sizeOf(){
    return sizeof(this);
}
    
int Handle::hash() {
    
    int64_t add =(int64_t) this;
    return add;
//     return static_cast<int>( this);
//	return reinterpret_cast<int> (this);
    return 0;
}

//String Handle::toString() {
//	return String("handle");
//}
#ifdef _DEBUG
TInt Handle::handleCount = 0;
#endif
};
