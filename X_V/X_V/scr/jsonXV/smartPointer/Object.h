/*
 ============================================================================
 Name		: Object.h
 Author	  : 
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CObject declaration
 ============================================================================
 */

#ifndef OBJECT_H
#define OBJECT_H

 
// INCLUDES

#include "jsonXVdefines.h"

#define K_AddStep 5

enum ObjectType {
	OT_Object,

	OT_Integer,
	OT_String,
	OT_Bytes,
	OT_List,
	OT_Map,
	
	OT_Pair,
	OT_StringBuilder,
	OT_ByteArrayInputStream,
	OT_ByteArrayOutputStream,
    
	OT_GenPtr,
    
    OT_ObjArrayOper,//only for xv, can't asign, can asign by
};


enum ExceptionType {
    
    ET_Error_jsonData2Object=0,
    
    ET_TypeError_ObjectAssign=100,
    ET_TypeError_NotFound,
    
    ET_TypeError_List = 200,
    ET_TypeError_Map,
    ET_TypeError_Integer,
    ET_TypeError_Integer_StrParserError,
    ET_TypeError_String,
    ET_TypeError_Bytes,
    ET_TypeError_GenPtr,
    ET_TypeError_Pair,
    
    ET_NotFound_MapItem = 300,
    ET_NotFound_ListItem,
    ET_NotFound_ObjectArray,
    ET_NotFound_ObjC,
    
    ET_OutOfBounds = 400,
    
	ET_LoadObj_Error = 500,
	
    ET_MemError_ListAllocMem = 1000,
	//ET_Null = 400,
	// ET_IO = -3000,
	//  ET_IndexOutOfBounds = -500,
	//   ET_EOF = -4000
};


namespace XV {
    class Handle;
}

class String;
class Bytes;

class Object
#if OBJC
:public ObjCBase
#endif
{
//	friend class List;
	friend class XV::Handle;
public:
	//for Integer
	Object(int num);
	Object(double num);
	
	//for String
	Object(const char* chars);
    
public:
	virtual ~Object();
    
	Object();
    
	/*explicit*/ Object(const Object& obj);

	Object(XV::Handle* handle);
	
	INLINE ObjectType getType() const;

	bool isNull() const;

	Object& operator=(const Object& ref);

    bool operator==(const Object& ref) const;
    
	Object& operator[] (const Object& keyOrIndex);//for Map&List, performance is not good

	bool operator!=(const Object& ref) const;
    
    int hashCode() const;

	void clean();//will sub count and set null set root ptr, dont useed by List Map
	
    XV::Handle* handle;
	
	String toString() const;
    
    int sizeOf()const;//not tested
	
	Bytes toBytes()const;
	
	void Save(const String& aFN);
    
	static Object Load(const String& aFN);
	
//protected:
#if DEBUG
	virtual void updateHandle(XV::Handle* value);
#else
	INLINE void updateHandle(XV::Handle* value);
#endif

	INLINE void deRef();//!!! waring, don't invoke this function, it's danger
    
	INLINE void incRef();//!!! waring, don't invoke this function, it's danger
	
#ifdef __WINS__ 
public:
	int getRef();
#endif
#ifdef _DEBUG
	static TInt objectCount;
#endif

};

static Object OBJECTNOTFOUND;

#endif // OBJECT_H
