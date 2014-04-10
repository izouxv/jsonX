/*
 ============================================================================
 Name		: ByteArrayInputStream.h
 Author	  : 
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CByteArrayInputStream declaration
 ============================================================================
 */

#ifndef BYTEARRAYINPUTSTREAM_H
#define BYTEARRAYINPUTSTREAM_H

// INCLUDES

#include "Object.h"

class InputStream: public Object {
public:
	// Constructors and destructor
	
	virtual ~InputStream();
	
	InputStream();
	
	virtual int read() = 0;
	
	virtual int read(Bytes& data) = 0;
	
	virtual int available() = 0;
	
	virtual void mark(int pos) = 0;
	
	virtual void reset() = 0;
	
};

class ByteArrayInputStream: public InputStream {
public:

	ByteArrayInputStream(const Bytes& bytes);

	int read();

	int read(Bytes& data);

	int available();

	void mark(int pos);

	void reset();

	TUint readInt();
	
	TInt64 readInt64();

	TUint16 readShort();

	Bytes readVarChar();
	
	String readVarChar2String();
	
	Bytes readShortBytes();
	
	Bytes readBytes();
	
	Bytes readIntBytes();
	
	String readShortString();
	
	String readString();
    
	Bytes getRemainderBytes();
private:
    
	String readVarString();
};

#endif // BYTEARRAYINPUTSTREAM_H
