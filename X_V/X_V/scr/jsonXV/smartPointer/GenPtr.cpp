/*
 ============================================================================
 Name		: String.cpp
 Author	  : 
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CString implementation
 ============================================================================
 */
//#include "defineUtil.h"
#include "GenPtr.h"
#include "Handle.h"
#include "String.h"
#include "Integer.h"
#include "Bytes.h"
//#include "UiHelperX.h"
GenPtrBase::GenPtrBase() :genPtr(this, false){
//	genPtr = GenPtr(this, false);
}
GenPtrBase::~GenPtrBase() {
}

//class String;
class GenPtrHandle: public XV::Handle {
public:
	bool autoDel;
	GenPtrBase * ptr;
	ObjectType getType() {
		return OT_GenPtr;
	}
    int sizeOf(){
        return sizeof(this);
    }
	GenPtrHandle() :
		Handle() {
		ptr = NULL;
		autoDel = false;
	}
	void destroy() {
		if (autoDel){
			if (ptr) {
				delete ptr, ptr = NULL;
			}
		}
		ptr = NULL;
	}
    
    bool compare(Handle* handle) {
            GenPtrHandle* temp = (GenPtrHandle*) handle;
            return ptr == temp->ptr; 
    }
    
	String toString() {
        return Integer(hash()).toString();
	}
	Bytes toBytes() {
		return toString().toBytes();
	}
};

#ifdef __WINS__
void GenPtr::updateHandle(Handle* value) {
	Object::updateHandle(value);
	internal = (GenPtrHandle*) value;
}
#endif

//GenPtr::GenPtr() :
//	Object() {
//}
GenPtr::GenPtr(GenPtrBase * ptr, bool autoDel) :
	Object() {
	GenPtrHandle* temp = new GenPtrHandle();
	temp->ptr = ptr;
	temp->autoDel = autoDel;
	updateHandle(temp);
}
GenPtr::GenPtr(const Object& object) :
	Object() {
	if (object.getType() == OT_GenPtr) {
		updateHandle(object.handle);
		incRef();
		return;
	}else{
        LEAVE(ET_TypeError_GenPtr); 
    }
}

GenPtrBase* GenPtr::getPtr() const {
	if (handle) {
		return ((GenPtrHandle*) handle)->ptr;
	}
	return 0;
}
