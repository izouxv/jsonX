/*
 ============================================================================
 Name		: StringBuilder.cpp
 Author	  : 
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CStringBuilder implementation
 ============================================================================
 */

#include "StringBuilder.h"
#include "Handle.h" 
#include "Integer.h"
#include "Bytes.h"
#include "String.h"
#include "Ptr.h"

class StringBuilderHandle: public XV::Handle {
public:
	ObjectType getType() {
		return OT_StringBuilder;
	}
	StringBuilderHandle() :
		ptr(), Handle() {
		size = K_AddStep;
		data = new TUint16[size];
		ptr.Set(data, 0, size);
	}
    
    int sizeOf(){
        return sizeof(this);
    }
	void requestSize(int value) {
		if (value > size - ptr.Length()) {
			size += K_AddStep;
			size += value;

			TUint16* temp = new TUint16[size];
			TPtr tempPtr(temp, 0, size);
			tempPtr = ptr;
			if (data) {
				delete data, data = NULL;
			}
			data = temp;
			ptr.Set(data, ptr.Length(), size);
		}
	}

	void append(const String& value) {
		requestSize(value.length());
		ptr += value;
	}

//	void append(const TChar value) {
//		requestSize(1);
//		ptr.Append(value);
//	}

	void destroy() {
		if (data) {
			delete []data, data = NULL;
		}
	}

	String toString() {
		String str = String(ptr);
		if (data) {
			delete data, data = NULL;
			size = 0;
		}
		return str;
	}
	Bytes toBytes() {
		return toString().toBytes();
	}
    bool compare(Handle* handle) {
        return false;
    }
	TPtr ptr;
	TUint16* data;
	int size;
};

StringBuilder::StringBuilder() {
	updateHandle(new StringBuilderHandle());
}

StringBuilder& StringBuilder::append(const String& value) {
		StringBuilderHandle* temp = (StringBuilderHandle*) handle;
		temp->append(value);
		return *this;
}

//StringBuilder& StringBuilder::append(const TChar value) {
//	if (handle) {
//		StringBuilderHandle* temp = (StringBuilderHandle*) handle;
//		temp->append(value);
//		return *this;
//	} else {
//		LEAVE(ET_Null);
//	}
//}

StringBuilder& StringBuilder::operator<<(const String& string) {
	return append(string);
}

StringBuilder& StringBuilder::operator<<(const char* things) {
	return append(things);
}

StringBuilder& StringBuilder::operator<<(const Object& object){
	return append(object.toString());
}

StringBuilder& StringBuilder::operator<<(int things) {
	return append(  Integer(things).toString());//zouxu_err
}

String StringBuilder::toString() {
    StringBuilderHandle* temp = (StringBuilderHandle*) handle;
    return temp->toString();
}
