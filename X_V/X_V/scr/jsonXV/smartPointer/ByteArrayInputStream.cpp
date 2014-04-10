/*
 ============================================================================
 Name		: ByteArrayInputStream.cpp
 Author	  : 
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CByteArrayInputStream implementation
 ============================================================================
 */
#include "ByteArrayOutputStream.h"
#include "ByteArrayInputStream.h"
#include "Handle.h" 
#include "Bytes.h"
#include "String.h"
#include "Ptr.h"
#include "Logger.h"
#include <stdio.h>

InputStream::~InputStream() {
}

InputStream::InputStream() :
Object() {
}



class ByteArrayInputStreamHandle: public XV::Handle {
public:
	ObjectType getType() {
		return OT_ByteArrayInputStream;
	}
    int sizeOf(){
        return sizeof(this);
    }
	ByteArrayInputStreamHandle(const Bytes& aBytes) :
		Handle() {
		markPos = 0;
		readPos = 0;
		bytes = aBytes;
	}

	int markPos;
	int readPos;
	Bytes bytes;

	int read() {
		if (bytes.length() > readPos) {
			return bytes.get(readPos++);
		} else {
			return -1;
		}
	}

	int read(Bytes& thing) {
      
        //		if (bytes.length() > readPos + thing.length()) {
        //
        //		}
        //		HBufC8* temp = HBufC8::NewLC(bytes.length());
        //		TPtr8 ptr = temp->Des();
        //		//ERR by_zouxu
        //		//		int ok = file.Read(readPos, ptr);
        //		int ok;
        //		if (ok == KErrNone) {
        //			readPos += bytes.length();
        //			return ptr[0];
        //		} else if (ok == KErrEof) {
        //			return -1;
        //		} else {
        //			LEAVE(ET_IO);
        //		}
        //		CleanupStack::PopAndDestroy();
        
        printf("read bytes...fucking error ...................");
		 
        return 0;
	}
    bool compare(Handle* handle) {
        return false;
    }
	void destroy() {
	}
	String toString() {
		return String("ByteArrayInputStream");
	}
	Bytes toBytes() {
		return toString().toBytes();
	}
};

ByteArrayInputStream::ByteArrayInputStream(const Bytes& bytes) :
	InputStream() {
	ByteArrayInputStreamHandle* temp = new ByteArrayInputStreamHandle(bytes);
	updateHandle(temp);
}

int ByteArrayInputStream::read() {

		ByteArrayInputStreamHandle* temp = (ByteArrayInputStreamHandle*) handle;
		if (temp->readPos < temp->bytes.length()) {
			return temp->bytes.get(temp->readPos++);
		} else {
			return -1;
		}
}

int ByteArrayInputStream::read(Bytes& data) {
		ByteArrayInputStreamHandle* temp = (ByteArrayInputStreamHandle*) handle;
		int len = data.length();
     //  printf("pos:%d  readlen:%d  leng:%d", temp->readPos, len, temp->bytes.length());
		if (temp->readPos + len <= temp->bytes.length()) {
			for (int i = 0; i < len; i++) {
				data.set(i, read());//by_zouxu need set ptr
			}
			return 0;
		} 
	return 1;
}

int ByteArrayInputStream::available() {
		ByteArrayInputStreamHandle* temp = (ByteArrayInputStreamHandle*) handle;
		if (temp->readPos < temp->bytes.length()) {
			return temp->bytes.length() - temp->readPos;
		}
    return 0;
}

void ByteArrayInputStream::mark(int pos) {

}

void ByteArrayInputStream::reset() {

}

TInt64 ByteArrayInputStream::readInt64() {
	TInt64 one = readInt();
	TInt64 two = readInt();
	TInt64 res = (one << 32 | two);
	return res;
}
TUint ByteArrayInputStream::readInt() {
	unsigned int one = read();
	unsigned int two = read();
	unsigned int three = read();
	unsigned int four = read();
	TUint res = (one << 24 | two << 16 | three << 8 | four);
	return res;
}

TUint16 ByteArrayInputStream::readShort() {
	int hex = read();
	int num = read();
	TUint16 res = (hex << 8 | num);
	return res;
}

String ByteArrayInputStream::readVarChar2String() {
	Bytes bytes = readVarChar();
	return String(bytes.desc());
}
Bytes ByteArrayInputStream::readBytes() {
	int len = readInt();
	Bytes bytes(len);
	if (len > 0)
		read(bytes);
	return bytes;
}

Bytes ByteArrayInputStream::readIntBytes() {
	return readBytes();
}
Bytes ByteArrayInputStream::readShortBytes() {
	int len = readShort();
	Bytes bytes(len);
	if (len > 0)
		read(bytes);
	return bytes;
}
String ByteArrayInputStream::readShortString() {
	Bytes bytes = readShortBytes();
	return String(bytes.desc());
}

String ByteArrayInputStream::readString() {
	int len = readInt();
   // printf("len:%d\n",len);
	Bytes bytes(len);
	if (len > 0)
		read(bytes);
	return String(bytes.desc());
}

String ByteArrayInputStream::readVarString() { 
	return readString();
}

Bytes ByteArrayInputStream::readVarChar() {
	int len = read();
	Bytes bytes(len);
	if (len > 0)
		read(bytes);
	return bytes;
	//return String(bytes.desc());
}
Bytes ByteArrayInputStream::getRemainderBytes() {
	if (handle) {
        ByteArrayInputStreamHandle* temp = (ByteArrayInputStreamHandle*) handle;
        int len = temp->bytes.length()-temp->readPos;
        Bytes bytes(len);
        if (len > 0)
            read(bytes);
        return bytes;
        
        
//		ByteArrayInputStreamHandle* temp = (ByteArrayInputStreamHandle*) handle;
//       printf("pos:%d  leng:%d\n", temp->readPos,  temp->bytes.length());
//		if (temp->readPos < temp->bytes.length()) {
//            TPtr8 ptr =temp->bytes.des().Mid(temp->readPos);
//             Bytes byte(ptr);
//            const char* source = byte.charc();
//           printf("1: %s    2: %s", source, ptr.source);
////            DEBUG<<byte;
//			return Bytes(ptr);
//		}
	}
	return Bytes();
}
