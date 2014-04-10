/*
 ============================================================================
 Name		: ByteArrayOutputStream.h
 Author	  : 
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CByteArrayOutputStream declaration
 ============================================================================
 */

#ifndef BYTEARRAYOUTPUTSTREAM_H
#define BYTEARRAYOUTPUTSTREAM_H

// INCLUDES

#include "Object.h"
// CLASS DECLARATION

/**
 *  CByteArrayOutputStream
 * 
 */

class OutputStream: public Object {
public:
	// Constructors and destructor
	
	virtual ~OutputStream();
	
	OutputStream();
	
	virtual void write(TUint8 aByte) = 0;
	
	virtual void writeShort(TUint16 aShort) = 0;
	
	virtual void writeInt(TUint aInt) = 0;
	
	virtual Bytes toBytes() = 0;
	
};


class ByteArrayOutputStreamHandle;
class ByteArrayOutputStream: public OutputStream {
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~ByteArrayOutputStream();

	ByteArrayOutputStream();

	void write(TUint8 aByte);

	void writeShort(TUint16 aShort);

	void writeInt(TUint aInt);

	void writeInt64(TInt64 aInt64);

	void writeVarChar(const Bytes& value);

	void writeShortString(const Bytes& value);

	void writeString(const Bytes& value);

	void writeIntBytes(const Bytes& bytes);

	void writeBytes(const Bytes& bytes);

	Bytes toBytes();

private:
	void initBuffer();
    
	void writeVarInt(int aInt);
	void writeVarString(const Bytes& value);


#ifdef __WINS__
	virtual void updateHandle(Handle* value);
	ByteArrayOutputStreamHandle* internal;
#endif
};

#endif // BYTEARRAYOUTPUTSTREAM_H
