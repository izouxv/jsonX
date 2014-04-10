/*
 ============================================================================
 Name		: ByteArrayOutputStream.cpp
 Author	  : 
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CByteArrayOutputStream implementation
 ============================================================================
 */

#include "ByteArrayOutputStream.h"
#include "Handle.h"
#include "Bytes.h"
#include "String.h"
#include "Ptr.h"

OutputStream::OutputStream() :
Object() {
}

OutputStream::~OutputStream() {
}



class ByteArrayOutputStreamHandle: public XV::Handle {
public:

	TPtr8 ptr; 
	TUint8* data;
	int size;

	ObjectType getType() {
		return OT_ByteArrayOutputStream;
	}
    
    int sizeOf(){
        return sizeof(this);
    }
	ByteArrayOutputStreamHandle() :
		ptr(), Handle() {
		size = K_AddStep;
		data = new TUint8[size];
		ptr.Set(data, 0, size);
	}

	void requestSize(int value) {
		if (value > size - ptr.Length()) {
			size += K_AddStep;
			size += value;
			TUint8* temp = new TUint8[size];
			TPtr8 tempPtr(temp, 0, size);
			tempPtr = ptr;
			if (data) {
				delete []data, data = NULL;
			}
			data = temp;
			ptr.Set(data, ptr.Length(), size);
		}
	}
    
    bool compare(Handle* handle) {
        return false;
    }
    
	void append(const TPtr8& value) {
		requestSize(value.Length());
		ptr += value;
	}
	void appendChar(TUint8 value) {
		requestSize(1);
        int length =ptr.Length();
		ptr[length]=value;
        ptr.length+=1;
	}

	void destroy() {
		if (data) {
			delete [] data, data = NULL;
		}
	}

	Bytes toBytes() {
		Bytes bytes = Bytes(ptr);
		if (data) {
			delete []data, data = NULL;
			size = 0;
		}
		return bytes;
	}
	String toString() {
		return String("ByteArrayOutputStreamHandle");
	}
};

ByteArrayOutputStream::ByteArrayOutputStream() :
	OutputStream() {
	ByteArrayOutputStreamHandle* temp = new ByteArrayOutputStreamHandle();
	updateHandle(temp);
}

ByteArrayOutputStream::~ByteArrayOutputStream() {
}

void ByteArrayOutputStream::write(TUint8 aByte) {
	initBuffer();
//	TBuf8<1> append;
//	append.Append(aByte);
//	char append = aByte;
	ByteArrayOutputStreamHandle* temp = (ByteArrayOutputStreamHandle*) handle;
    temp->appendChar(aByte);
}

void ByteArrayOutputStream::writeShort(TUint16 aShort) {
	write(TUint8((aShort & 0xff00) >> 8));
	write(TUint8(aShort & 0xff));
}
void ByteArrayOutputStream::writeInt64(TInt64 aInt64) {
	writeInt(aInt64 >> 32);
	writeInt(aInt64);
}
void ByteArrayOutputStream::writeInt(TUint aInt) {
	write(TUint8((aInt & 0xff000000) >> 24));
	write(TUint8((aInt & 0xff0000) >> 16));
	write(TUint8((aInt & 0xff00) >> 8));
	write(TUint8(aInt & 0xff));
}

void ByteArrayOutputStream::writeVarInt(int value) {
	int i = 32;
	for (; i > 0; i--) {
		int test = value & (2 << (i - 1));
		if (test > 0) {
			break;
		}
	}
	i = i / 7 + (i % 7 ? 1 : 0);
	for (; i > 0; i--) {
		TUint8 it = ((value & (127 << (i - 1))) + (i > 1 ? 128 : 0));
		write(it);

	}
}

void ByteArrayOutputStream::writeVarChar(const Bytes& value) {
	int len = value.length();
	write(len);
	if (len > 0)
		writeBytes(value);
}

void ByteArrayOutputStream::writeVarString(const Bytes& value) {
	int len = value.length();
	writeVarInt(len);
	if (len > 0)
		writeBytes(value);
}

void ByteArrayOutputStream::writeShortString(const Bytes& value) {
	int len = value.length();
	writeShort(len);
	if (len > 0)
		writeBytes(value);
}

void ByteArrayOutputStream::writeString(const Bytes& value) {
	int len = value.length();
	writeInt(len);
	if (len > 0)
		writeBytes(value);
}

void ByteArrayOutputStream::writeIntBytes(const Bytes& value) {
	int len = value.length();
	writeInt(len);
	if (len > 0)
		writeBytes(value);
}

void ByteArrayOutputStream::writeBytes(const Bytes& bytes) {
	ByteArrayOutputStreamHandle* temp = (ByteArrayOutputStreamHandle*) handle;
	temp->append(bytes);
}

Bytes ByteArrayOutputStream::toBytes() { 
		ByteArrayOutputStreamHandle* temp =
				(ByteArrayOutputStreamHandle*) handle;
		return temp->toBytes();
}

void ByteArrayOutputStream::initBuffer() {
	if (!handle) {
		ByteArrayOutputStreamHandle* temp = new ByteArrayOutputStreamHandle();
		handle = temp;
	}
}
#ifdef __WINS__
void ByteArrayOutputStream::updateHandle(Handle* value) {
	Object::updateHandle(value);
	internal = (ByteArrayOutputStreamHandle*) value;
}
#endif
