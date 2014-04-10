/*
 ============================================================================
 Name		: Pair.h
 Author	  : 
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CPair declaration
 ============================================================================
 */

#ifndef PAIR_H
#define PAIR_H

// INCLUDES 

#include "Object.h"

class PairHandle;
class String;

class Pair: public Object {
public:

	Pair(const Object& object);

	Pair(const Object& aKey, const Object& aValue);
    
	void setKey(const Object& obj);

	Object& getKey() const;

	void setValue(const Object& obj);

	Object& getValue() const; 

};

#endif // PAIR_H
