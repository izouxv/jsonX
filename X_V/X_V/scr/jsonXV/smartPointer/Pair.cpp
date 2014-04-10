/*
 ============================================================================
 Name		: Pair.cpp
 Author	  : 
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CPair implementation
 ============================================================================
 */

#include "Pair.h"
#include "Handle.h"
#include "String.h"
#include "Bytes.h"

class PairHandle: public XV::Handle {
public:
	PairHandle() :
		Handle() {
	}
	ObjectType getType() {
		return OT_Pair;
	}
	void destroy() {

	}
    
    int sizeOf(){
        return sizeof(this);
    }
	bool compare(Handle* handle) {
		//if(!handle)return false;
		PairHandle* temp = (PairHandle*) handle;
		if (temp) {
			return key == temp->key && value == temp->value;
		}
		return false;
	}

	Object key;
	Object value;

	String toString() {
		return key.toString() + String("-") + value.toString();
	}
	Bytes toBytes() {
		return toString().toBytes();
	}
};


Pair::Pair(const Object& aKey, const Object& aValue) :
	Object() {
	PairHandle* temp = new PairHandle();
	temp->key = aKey;
	temp->value = aValue;
	updateHandle(temp);
}

//Pair::Pair(const String& aKey, const String& aValue) :
//	Object(OT_Pair) {
//	PairHandle* temp = new PairHandle();
//	temp->key = aKey;
//	temp->value = aValue;
//	updateHandle(temp);
//}

Pair::Pair(const Object& object) :
	Object() {
	if (object.getType() == OT_Pair) {
		updateHandle(object.handle);
		incRef();
		return;
	}else{
        LEAVE(ET_TypeError_Pair);
    }
}

void Pair::setKey(const Object& obj) {
	if (!handle) {
		handle = new PairHandle();
	}
	PairHandle* temp = (PairHandle*) handle;
	temp->key = obj;
}

Object& Pair::getKey() const {
		return ((PairHandle*) handle)->key;
}

void Pair::setValue(const Object& obj) {
	if (!handle) {
		handle = new PairHandle();
	}
	PairHandle* temp = (PairHandle*) handle;
	temp->value = obj;
}

Object& Pair::getValue() const {
    return ((PairHandle*) handle)->value;
}
 
