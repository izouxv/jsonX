/*
 ============================================================================
 Name		: bytes.h
 Author	  : 
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : Cbytes declaration
 ============================================================================
 */

#ifndef BYTES_H
#define BYTES_H

// INCLUDES 
#include "Object.h"
class TPtr8;
class BytesHandle;
class List;

class Bytes: public Object {
public:

	Bytes(int size=0);

	Bytes(TUint8* data, int size);

	Bytes(const Object& object);

	Bytes(const TPtr8& des);
    
	Bytes(const char* chars);//utf8
    
	Bytes(const char* chars, int length);//bytes, etc:jpg,png,view....

	void Empty();
    
	operator const TPtr8&() const;
    
	operator const char*() const;
    
	operator char*()const;
    
	const char* charc() const;
    
    char* Char() const;

	const TPtr8& desc() const;

	TPtr8 des();

	Bytes& operator=(const TPtr8& thing);
    
    const unsigned char* getChatAndEmptySelf();
    
	int length() const;
    
	int maxLength() const;

	TUint8 get(int index) const;

	void set(int index, TUint8 value);
	
	unsigned int crc32() const;
    
	String md5() const;//32bytes visible char

	Bytes md5Bytes() const;
	
	Bytes base64Encode() const;
	
	Bytes base64Decode() const;
    
	Object toObject() const;
    
    List split(int maxSize, bool dyn=true)const;
    
    Bytes operator+(const Bytes& byte) const;
    
    void operator+=(const Bytes& byte);
    
private:
    void initWithSize(int size);
    void initWithContent( const char* chars, int length);
   
    
#ifdef __WINS__
public:
	String toNums() const;
	void updateHandle(Handle* value);
	BytesHandle* internal;
#endif

};

#endif // BYTES_H
