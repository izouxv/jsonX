/*
 ============================================================================
 Name		: List.h
 Author	  : 
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CList declaration
 ============================================================================
 */

#ifndef LIST_H
#define LIST_H

// INCLUDES 

#include "Object.h"

class ListHandleBase;
const int KListInitSize = 8;
class List: public Object {
public:
	/*explicit*/
	List(int size=0);

	/*explicit*/
	List(const Object& obj);

	void set(const Object& obj, int index);

	Object& get(int index) const;

	INLINE int add(const Object& obj);//return index

	void insert(const Object& obj, int index);
	
	bool removeObj(const Object& obj);
	
	Object& operator[] (int index);

    int indexOfObject(const Object& object);//-1 is not found, other is index
    
	List excludeDuplicates() const;
    
	INLINE int size() const;

	void reset();

	bool removeIndex(int index);

	bool contains(const Object& object);

	List operator+(const List& data) const;

	void operator+=(const List& data);
	
	//!!! warning, just reference to two pointer of list, the linkList is ONLY for GET, set remove add is NOT available currently,
	static List link(const List& listA, const List& listB);//it's a dangerous method
	
	List mid(int pos, int length)const;
    
    String testInfo()const;//for linkedlist

private:
#ifdef __WINS__
	void updateHandle(XV::Handle* value);
	ListHandleBase* internal;
#endif
    
public:
    static int TEST_nodeCount;
    static int TEST_nodeDeep;
};



#endif // LIST_H
