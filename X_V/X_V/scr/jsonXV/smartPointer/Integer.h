/*
 ============================================================================
 Name		: String.h
 Author	  : 
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CString declaration
 ============================================================================
 */

#ifndef Integer_H
#define Integer_H

// INCLUDES
#include "Object.h"
class IntegerHandle;

class Integer: public Object {
public:
	Integer(const Object& object);
	
	Integer(TInt64 aNum);
	Integer(double aNum);
	Integer(int aNum=0);
	
	void setDouble(double aNum);
    
	operator TInt64() const;
	operator int() const;
	operator double() const;
    
	TInt64 _TInt64() const;
	double _double() const;
    
	Integer operator+(TInt64 integer) const;
	Integer operator-(TInt64 integer) const;
	void operator+=(TInt64 integer);
	void operator-=(TInt64 integer);
	Integer &operator++();
	Integer operator++(int integer);
	Integer &operator--();
	Integer operator--(int integer);
    
private:
	void newHandle(TInt64 aInt, double aDou);

#ifdef __WINS__
	void updateHandle(Handle* value);
	IntegerHandle* internal;
#endif

};

#endif // Integer_H
