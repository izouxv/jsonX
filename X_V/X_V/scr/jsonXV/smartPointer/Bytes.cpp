/*
 ============================================================================
 Name		: bytes.cpp
 Author	  : 
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : Cbytes implementation
 ============================================================================
 */

#include "bytes.h"
#include "Handle.h"  
#include "String.h"
#include "Util.h"
#include "Ptr.h"
#include "List.h"
#include "Util.h"


//	static TPtr8 empty;
//TPtr8 Bytes::empty(0, 0);

class BytesHandle: public XV::Handle {
public:
	TUint8* data;
//	int size;
	TPtr8 ptr;
	ObjectType getType() {
		return OT_Bytes;
	}
    int sizeOf(){
        return sizeof(this);
    }
	BytesHandle() :
		ptr(), Handle() {
		data = NULL;
//		size = 0;
	}

	void destroy() {
		if (data) {
			delete [] data, data = NULL;
		}
	}

	bool compare(Handle* handle) {
		//if(!handle)return false;
		BytesHandle* temp = (BytesHandle*) handle;
		return (temp && ptr == temp->ptr);
	}
	String toString() {
		return String(ptr);
	}
	Bytes toBytes() {
		return Bytes(ptr);
	}
};

//Bytes::Bytes() :
//	Object() {
//}

void Bytes::initWithSize(int size){
    BytesHandle* temp = new BytesHandle();
	temp->data = new TUint8[size + 1];
    temp->data[size]=0;
//	temp->size = size;
	temp->ptr.Set(temp->data, size, size + 1);
	updateHandle(temp);
}
Bytes::Bytes(int size) :
	Object() {
    initWithSize(size);
}

Bytes::Bytes(const Object& object) :
	Object() {
	if (object.getType() == OT_Bytes) {
		updateHandle(object.handle);
		incRef();
		return;
    }else if (object.getType() == OT_String) {
        String str =object;
        Bytes bytes =str.toBytes();
        updateHandle(bytes.handle);
		incRef();
        return;
	}else{
        LEAVE(ET_TypeError_Bytes); 
        initWithSize(0);
    }
}

Bytes::Bytes(TUint8* data, int size) :
	Object() {
	BytesHandle* temp = new BytesHandle();
	temp->data = data;
//	temp->size = size;
	temp->ptr.Set(temp->data, size, size + 1);
	updateHandle(temp);
}

Bytes::Bytes(const TPtr8& des) :
	Object() {
	BytesHandle* temp = new BytesHandle();
	int size = des.Length();
	temp->data = new TUint8[size + 1];
//	temp->size = len;
	temp->ptr.Set(temp->data, 0, size + 1);
	temp->ptr = des;
	updateHandle(temp);
}

void Bytes::initWithContent(const char* chars, int len){
    TPtr8 des(reinterpret_cast<TUint8*> (const_cast<char*> (chars)), len, len);
	//TPtr8 des((TUint8*) chars, len - 1,len );
	BytesHandle* temp = new BytesHandle();
	int size = des.Length();
	temp->data = new  TUint8[size+1];
    temp->data[size]=0;
	temp->ptr.Set(temp->data, 0, size);
	temp->ptr = des;
//	temp->size = temp->ptr.Length();
	updateHandle(temp);
}
Bytes::Bytes(const char* chars, int length){
    initWithContent(chars, length);
}
Bytes::Bytes(const char* chars) {
	int len(0);
	while (chars[len++]);
    len--;
    initWithContent(chars, len);
}

void Bytes::Empty() {
	if (handle) {
		BytesHandle* temp = ((BytesHandle*) handle);
        temp->ptr.length=0;
        Util::memtrim(temp->data, temp->ptr.maxLength);
//		temp->size = 0;
//		temp->ptr.Set(temp->data, 0, temp->size);
	}
}

Bytes::operator char*()const{
     return Char();
}
Bytes::operator const char*() const{
    return Char();
}

Bytes::operator const TPtr8&() const { 
		return ((BytesHandle*) handle)->ptr; 
}

Bytes& Bytes::operator=(const TPtr8& thing) {
	Bytes temp(thing);
	*this = temp;
	return *this;
}

const unsigned char* Bytes::getChatAndEmptySelf() {
	if (handle) {
		const unsigned char* c = (((BytesHandle*) handle)->ptr.source);
		//unsigned char* c = ((BytesHandle*) handle)->data;
		((BytesHandle*) handle)->data = NULL;
//		((BytesHandle*) handle)->size = 0;
        ((BytesHandle*) handle)->ptr.length=0;
		return c;
	}
	return NULL;
}

int Bytes::length() const {
    if (handle) {
		BytesHandle* temp = ((BytesHandle*) handle);
        return temp->ptr.length;
        //		temp->size = 0;
        //		temp->ptr.Set(temp->data, 0, temp->size);
	}
    return 0;
//		return ((BytesHandle*) handle)->size; 
}

int Bytes::maxLength() const{
    if (handle) {
		BytesHandle* temp = ((BytesHandle*) handle);
        return temp->ptr.maxLength;
	}
    return 0;
}

const char* Bytes::charc() const {
	return Char();
}
char* Bytes::Char() const{
    if (handle) {
		return (char*) (((BytesHandle*) handle)->ptr.source);
	}
	return NULL;
}
const TPtr8& Bytes::desc() const {
		return ((BytesHandle*) handle)->ptr; 
}
TPtr8 Bytes::des() { 
		return ((BytesHandle*) handle)->ptr; 
}

TUint8 Bytes::get(int index) const {
		BytesHandle* temp = ((BytesHandle*) handle);
		if (temp->ptr.length > index) {
			return temp->data[index];
		}
	return 0;
}

void Bytes::set(int index, TUint8 value) { 
		BytesHandle* temp = ((BytesHandle*) handle);
		if (temp->ptr.length > index) {
			temp->data[index] = value;
			return;
		} 
}

unsigned int Bytes::crc32() const{ 
	return Util::crc32(*this);
}
String Bytes::md5() const {
	return Util::md5(*this);
}
Bytes Bytes::md5Bytes() const {
    return Util::md5Bytes(*this);
}
Object Bytes::toObject() const { 
	Object obj; 
    bool err= Util::jsonData2Object(*this, obj);
	if(!err) 
		LEAVE(ET_Error_jsonData2Object);
	return obj;
}
List Bytes::split(int maxSize, bool dyn)const{
    if(maxSize<=0){
        List list;
        return list;
    }
    List list;
    if(length()/maxSize<300){
        list=List(length()/maxSize+1);
    }
	if (length() <= 0)
		return list;
	TUint packet = 0;
	while (packet*maxSize<length()) {
		TPtr8 ptr(desc().Mid(packet*maxSize, maxSize));
        list.add(Bytes(ptr));
        packet++;
	}
	return list;
}
Bytes Bytes::base64Encode() const{
	return Util::base64Encode(*this);
}

Bytes Bytes::base64Decode() const{
	return Util::base64Decode(*this);
}

Bytes Bytes::operator+(const Bytes& byte) const {
	int len = length() + byte.length();
    if(len<=maxLength()){
        BytesHandle* hand = ((BytesHandle*) handle);
        TUint8* dataSecond = hand->data+length();
        BytesHandle* second = ((BytesHandle*) byte.handle);
        Util::memcpy(dataSecond, second->data, byte.length());
        hand->ptr.length+=byte.length();
        return *this;
        //return Bytes(*this);
    }else{
      	TUint8* dataFirst = new TUint8[len];
        TUint8* dataSecond = dataFirst+length();
        BytesHandle* first = ((BytesHandle*) handle);
        BytesHandle* second = ((BytesHandle*) byte.handle);
        Util::memcpy(dataFirst, first->data, length());
        Util::memcpy(dataSecond, second->data, byte.length());
        Bytes sum(dataFirst, len);
        return sum;
    }

//    int len = length() + byte.length(); 
//	TPtr8 copy(data, 0, len);
//	if (length()) {
//		copy.Append(*this);
//	}
//	if (byte.length()) {
//		copy.Append(byte);
//	}
//	Bytes sum(copy);
//	delete[] data, data = NULL;
//	return sum;
}

void Bytes::operator+=(const Bytes& byte) {
	*this = *this + byte;
}

#ifdef __WINS__
String Bytes::toNums() const {
	StringBuilder strB;
	strB << "Length: " << length() << "\r\n";
	for (int i = 0; i < length(); i++) {
		strB << get(i) << " ";
	}
	return strB.toString();
}
void Bytes::updateHandle(Handle* value) {
	Object::updateHandle(value);
	internal = (BytesHandle*) value;
}
#endif
