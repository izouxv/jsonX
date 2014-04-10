/*
 ============================================================================
 Name		: Map.h
 Author	  : 
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CMap declaration
 ============================================================================
 */

#ifndef MAP_H
#define MAP_H

// INCLUDES 

#include "object.h"
class List;
class MapHandle;
class ObjArrayOperHandle;
#define MapSizeMax 256
#define MapSizeNormal 32
#define MapSizeMin 1
class Map: public Object {
public:

	Map(int mapSize = MapSizeMin/* = 256*/);

	Map(const Object& object);

	void put(const Object& key, const Object& value);

	Object& get(const Object& key) const;

	void remove(const Object& key);

	bool contains(const Object& key) const;

	int size() const;

	List getKeys() const;
    
    List getTables() const;//dont call this function, it is only for zouxu
	
	void reset();
	
	Map operator+(const Map& data) const;

	void operator+=(const Map& data);

#ifdef __WINS__
	void updateHandle(Handle* value);
	MapHandle* internal;
#endif
};



enum ArrayOperType {
    ArrayOperType_Idle,
	ArrayOperType_Assign,
    ArrayOperType_Once_AssignOk,
//    ArrayOperType_AsignSucNextDel,
    ArrayOperType_Deleted,
};

class ObjArrayOper: public Object {
public:
	ObjArrayOper(const List& aOperList, const Map& aMap, const Object& aKey);
    ObjArrayOper( const Object& object);
	void setValueAndRelease( const Object& aValue);
    void setStatus(ArrayOperType asign);
    ArrayOperType getStatus();
#ifdef __WINS__
	void updateHandle(Handle* value);
	ObjArrayOperHandle* internal;
#endif
};






#endif // MAP_H
