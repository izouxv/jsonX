/*
 ============================================================================
 Name		: String.cpp
 Author	  : 
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CString implementation
 ============================================================================
 */

#include "Integer.h"
#include "Handle.h"  
#include "List.h"
#include "Pair.h" 
#include "bytes.h"
#include "String.h"
#include "Util.h"
#include "Ptr.h"
#include <math.h>

class IntegerHandle: public XV::Handle {
public:
	TInt64 number;
    double valuedouble;
	ObjectType getType() {
		return OT_Integer;
	}
    int sizeOf(){
        return sizeof(this);
    }
	IntegerHandle() :
		Handle() {
		number = -1;
        valuedouble = -1.0;
	}
	void destroy() {
	}
	~IntegerHandle() {
	} 
	int hash() { 
		return number;
	} 
	bool compare(Handle* handle) {
		//if(!handle)return false;
		IntegerHandle* temp = (IntegerHandle*) handle;
		return number == temp->number || DOUBLE_EQUAL(valuedouble,temp->valuedouble);
	}
	String toString() {
        return String(Object(this));
	}
	Bytes toBytes() {
		return toString().toBytes();
	}
};

void Integer::newHandle(TInt64 aInt, double aDou) {
	IntegerHandle* temp = new IntegerHandle();
	temp->number = aInt;
	temp->valuedouble = aDou;
	updateHandle(temp);
}

//void Integer::newHandle(TInt64 aNum) {
//	IntegerHandle* temp = new IntegerHandle();
//	temp->number = aNum;
//	updateHandle(temp);
//}

Integer::Integer(int aNum) :
Object() {
	newHandle(aNum, aNum);
}

Integer::Integer(TInt64 aNum) :
	Object() {
        newHandle(aNum, aNum);
}

void Integer::setDouble(double aNum){
	((IntegerHandle*) handle)->number=aNum;
	((IntegerHandle*) handle)->valuedouble=aNum;
}

Integer::Integer(double aNum) :
Object() {
	newHandle(aNum, aNum);
	//newHandle(aNum);
}
//Integer::Integer(long long aNum) :
//	Object(OT_Integer) {
//	TInt64 num = aNum;
//	newHandle(num);
//}
Integer::Integer(const Object& object) :
	Object() {
	if (object.getType() == OT_Integer) {
		updateHandle(object.handle);
		incRef();
		return;
    } else if (object.getType() == OT_String) {
        String str(object);
        TUint16 * source = str.des().source;
        TInt64 aInt;
        double aDou;
        bool ret = Util::string_2_number((const short*)source,aInt, aDou);
        if(ret)
            newHandle(aInt,  aDou);
        else{
            LEAVE(ET_TypeError_Integer_StrParserError);
            newHandle(-1, -1);
        }
		return;
	}else{
        LEAVE(ET_TypeError_Integer);
        newHandle(-1, -1);
    }
}
 
//Integer& Integer::operator=(Int64 aNum) {
//	Integer temp(aNum);
//	*this = temp;
//	return *this;
//}

//Integer::operator int() const {
//	if (handle) {
//		return ((IntegerHandle*) handle)->number;
//	}
//	return -1;
//}
Integer::operator TInt64() const {
    return ((IntegerHandle*) handle)->number;
}
Integer::operator int() const {
    return ((IntegerHandle*) handle)->number;
}

Integer::operator double() const{
    return ((IntegerHandle*) handle)->valuedouble;
}

//Integer::operator int() const {
//	if (handle) {
//		return ((IntegerHandle*) handle)->number;
//	}
//	return -1;
//}

TInt64 Integer::_TInt64() const {
    return ((IntegerHandle*) handle)->number;
}

double Integer::_double() const {
    return ((IntegerHandle*) handle)->valuedouble;
}
 
//Integer Integer::operator+(TInt64 nteger) const{
//	Integer sum(INT() + nteger);
//	return sum;
//}
Integer &Integer::operator++() { 
    IntegerHandle* temp = ((IntegerHandle*) handle);
    ++temp->number;
    ++temp->valuedouble;
		return *this; 
}
Integer Integer::operator++(int integer) {
 
		IntegerHandle* temp = ((IntegerHandle*) handle);
    Integer tmp(temp->number);
    temp->number++;
    temp->valuedouble++;
		return tmp; 
}
Integer &Integer::operator--() {
 
    IntegerHandle* temp = ((IntegerHandle*) handle);
    --temp->number;
    --temp->valuedouble;
		return *this; 
}
Integer Integer::operator--(int integer) {
 
		IntegerHandle* temp = ((IntegerHandle*) handle);
    Integer tmp(temp->number);
    temp->number--;
    temp->valuedouble--;
		return tmp; 
}
Integer Integer::operator+(TInt64 integer) const {
	Integer sum(_TInt64() + integer);
	return sum;
}
Integer Integer::operator-(TInt64 integer) const {
	Integer sum(_TInt64() - integer);
	return sum;
}
void Integer::operator+=(TInt64 integer) {
	*this = *this + integer;
}
void Integer::operator-=(TInt64 integer) {
	*this = *this - integer;
}

#ifdef __WINS__
void Integer::updateHandle(Handle* value) {
	Object::updateHandle(value);
	internal = (IntegerHandle*) value;
}
#endif

