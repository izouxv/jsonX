/*
 ============================================================================
 Name		: String.cpp
 Author	  : 
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CString implementation
 ============================================================================
 */

#include "String.h"
#include "Handle.h" 
#include "List.h"
#include "Pair.h" 
#include "bytes.h"
#include "Integer.h" 
#include "Util.h"
#include "Ptr.h"

class StringHandle: public XV::Handle {
public:

	StringHandle() :
		ptr(), Handle() {
		data = NULL;
		size = 0;
		hashX = 0;
	}
	ObjectType getType() {
		return OT_String;
	}
    int sizeOf(){
        return sizeof(this);
    }
	void destroy() {
		if (data) {
			delete []data, data = NULL;
		}
        hashX = 0;
	}

//    inline int strcmp(const char *dest, const char *source){
//        //assert((NULL != dest) && (NULL != source));
//        while (*dest && *source && (*dest == *source)) { 
//            dest++;
//            source++; 
//        } 
//        return *dest - *source;//0 is equal ,    >_< 
//    }
    
	bool compare(Handle* handle) {
		StringHandle* temp = (StringHandle*) handle;
		if(hashX && temp->hashX && hashX!=temp->hashX)
			return false;
        return ptr == temp->ptr;
//		if(hashX && temp->hashX)
//			return hashX==temp->hashX;
//        return ptr == temp->ptr;
        
//        if(size!=temp->size)
//            return false;
//        return !strcmp((char*)data, (char*)temp->data);
	}
    
    unsigned int SDBMHash(TUint16 *str) {
        unsigned int hash = 0; 
        while (*str)  {
            // equivalent to: hash = 65599*hash + (*str++);
            hash = (*str++) + (hash << 6) + (hash << 16) - hash;
        }
//        unsigned int hash = 5381;
//        while (*str) {
//            hash += (hash << 5) + (*str++);
//        }
        return (hash & 0x7FFFFFFF);
    }
    
    int hash() {
        if(hashX)
            return hashX;
        return hashX = SDBMHash(data);
		hashX = size;
		for (int i = 0; i < size; i++) {
            hashX += data[i];
		}
		return hashX;
	}

	TUint16* data;
	int size;
	TPtr ptr;
    int hashX;
    
	String toString() {
        Object object(this);
		String string(object);
        return string;
	}
	Bytes toBytes() {
		//return toString().getBytes();
		
		int len = size;
		if (len) {
			len *= 3;
			len += 6;
			TUint8* data = new TUint8[len+1];
			TPtr8 thing(data, 0, len);
			Util::ConvertFromUnicodeToUtf8(thing, ptr);
			return Bytes(data, thing.Length());
		}
		return Bytes();
		
		
	}
};

//String::String(const String& object) :
//	Object() {
//	*this = object;
//}

String::String(const Object& object) :
	Object() {
	if (object.getType() == OT_String) {
		updateHandle(object.handle);
		incRef();
		return;
    }else if (object.getType() == OT_Bytes) {
        Bytes bytes =object;
        initWithChar(bytes.charc());
        return;
    }else if (object.getType() == OT_Integer) {
        Integer number(object);
        char* tmp=NULL;
        Util::number_2_string(&tmp, number._TInt64(), number._double());
        initWithChar(tmp);
        if(tmp)
            delete []tmp;
	}else{
        LEAVE(ET_TypeError_String);
        initWithChar("");
    }
}

String::String(const TPtr& des) :
	Object() {
	StringHandle* temp = new StringHandle();
	temp->size = des.Length();
	temp->data = new TUint16[temp->size+1];
	temp->ptr.Set(temp->data, 0, temp->size);
	temp->ptr = des;
	updateHandle(temp);
}

String::String(const TPtr8& des):
	Object() {
	StringHandle* temp = new StringHandle();
	temp->size = des.Length();
	temp->data = new  TUint16[temp->size+1];
	temp->ptr.Set(temp->data, 0, temp->size);
    Util::ConvertToUnicodeFromUtf8(temp->ptr, des);
	if (temp->ptr.Length() > 0 && temp->ptr[0] == 0xFEFF)
		temp->ptr.Delete(0, 1);
	temp->size = temp->ptr.Length();
	updateHandle(temp);
}
void String::SET(const String& aString) {
	if (handle) {
		StringHandle* temp = (StringHandle*) handle;
		delete temp->data;
        temp->hashX=0;
		TPtr des = aString.desc();
		temp->size = des.Length();
		temp->data = new TUint16[temp->size+1];
		temp->ptr.Set(temp->data, 0, temp->size);
		temp->ptr = des;
	}
}

void String::initWithChar(const char* chars){
    int len(0);
	while (chars[len++]);
	TPtr8 des(reinterpret_cast<TUint8*> (const_cast<char*> (chars)), len - 1,len );
	//TPtr8 des((TUint8*) chars, len - 1,len );
	StringHandle* temp = new StringHandle();
	temp->size = des.Length();
	temp->data = new TUint16[temp->size+1];
	temp->ptr.Set(temp->data, 0, temp->size);
    Util::ConvertToUnicodeFromUtf8(temp->ptr, des);
	temp->size = temp->ptr.Length();
	updateHandle(temp);
}
String::String(const char* chars) :
	Object() {
    initWithChar(chars);
}

String::operator const TPtr&() const {
    return ((StringHandle*) handle)->ptr;
}

const TPtr& String::desc() const {
    return ((StringHandle*) handle)->ptr;
}

TPtr String::des(){
    return ((StringHandle*) handle)->ptr;
}

String& String::operator=(const TPtr& thing) {
	String temp(thing);
	*this = temp;
	return *this;
}

String& String::operator=(const TPtr8& thing) {
	String temp(thing);
	*this = temp;
	return *this;
}

String& String::operator=(const char* thing) {
	String temp(thing);
	*this = temp;
	return *this;
}

int String::length() const {
		return ((StringHandle*) handle)->size;
}

String String::operator+(const Integer& integer) const{
    return *this + String(integer);
}

String String::operator+(const String& string) const {
	int len = length() + string.length();
	TUint16* data = new TUint16[len];
	TPtr copy(data, 0, len);
	if (length()) {
		copy.Append(*this);
	}
	if (string.length()) {
		copy.Append(string);
	}
	String sum(copy);
	delete[] data, data = NULL;
	return sum;
}

String String::operator+(const TPtr& string) const {
	return *this + String(string);
}

String String::operator+(const char* string) const {
	return *this + String(string);
}

//void String::operator+=(const Integer& string) {
//	*this = *this + string;
//}

void String::operator+=(const String& string) {
	*this = *this + string;
}

void String::operator+=(const TPtr& string) {
	*this = *this + string;
}
void String::operator+=(const char* string) {
	*this = *this + string;
}

String String::substring(int start, int end) const {
	if (end == -1) {
		end = length();
	}
	if (end < start || start < 0 || start >= length() || end > length()) {
		LEAVE(ET_OutOfBounds);
	}

	if (end > start && handle) {
		StringHandle* temp = (StringHandle*) handle;
		TPtr tempPtr(temp->data + start, end - start, end - start);
		return String(tempPtr);
	}
	return String("");
}

bool String::startsWith(const String& str) const {
	int count = str.length();
	if (length() >= count && count) {
		for (int i = 0; i < count; i++) {
			if (this->charAt(i) != str.charAt(i)) {
				return false;
			}
			return true;
		}
	}
	return false;

}

bool String::startsWithIgnoreCase(const String& str) const {
	return (*this).toLowerCase().startsWith(str.toLowerCase());
}

bool String::endsWith(const String& str) const {
	int count = str.length();
	int len = length();
	if (len >= count && count) {
		for (int i = 0; i < count; i++) {
			if (this->charAt(len - count + i) != str.charAt(i)) {
				return false;
			}
			return true;
		}
	}
	return false;
}

bool String::endsWithIgnoreCase(const String& str) const {
	return (*this).toLowerCase().endsWith(str.toLowerCase());
}

TInt16 String::charAt(int index) const {
	if (length() && index >= 0 && index < length()) {
		return ((StringHandle*) handle)->ptr[index];
	}
	LEAVE(ET_OutOfBounds);
	return 0;
}

String String::toLowerCase() const {
		StringHandle* temp = (StringHandle*) handle;
		String tempStr(temp->ptr);
		((StringHandle*) tempStr.handle)->ptr.LowerCase();
		return tempStr;
}

String String::toUpperCase() const {
		StringHandle* temp = (StringHandle*) handle;
		String tempStr(temp->ptr);
		((StringHandle*) tempStr.handle)->ptr.UpperCase();
		return tempStr;
}

//Bytes String::getBytes() const {
//	int len = length();
//	if (len) {
//		len *= 3;
//        len += 6;
//		TUint8* data = new TUint8[len+1];
//		TPtr8 thing(data, 0, len);
//         Util::ConvertFromUnicodeToUtf8(thing, *this);
//		return Bytes(data, thing.Length());
//	}
//	return Bytes();
//}


int String::crc32() const{
	return toBytes().crc32();
}
String String::md5() const {
	return toBytes().md5();
}
Object String::toObject() const { 
	return toBytes().toObject();
}
Bytes String::md5Bytes() const {
	return toBytes().md5Bytes();
}

const char* String::getCharc() const{
    return toBytes().charc();
}
void String::remove(const TPtr& aWord) {
	if (handle) {
		StringHandle* temp = (StringHandle*) handle;
		int pos = temp->ptr.Find(aWord);
		while (pos != KErrNotFound) {
			temp->size -= aWord.Length();
			temp->ptr.Delete(pos, aWord.Length());
			pos = temp->ptr.Find(aWord);
		}  
        temp->hashX=0;
	}
}

#ifdef __WINS__
void String::updateHandle(Handle* value) {
	Object::updateHandle(value);
	internal = (StringHandle*) value;
}
#endif

 

//TInt64 String::toInt() {
//	TLex16 a(desc());
//	TInt64 num;
//	if (a.Val(num) == KErrNone)
//		return num;
//	else
//		return -1;
//}
#if 0
String String::Mem() {
#if 0
	TInt totalAllocSize;
	User::Heap().AllocSize(totalAllocSize);
	//	HAL::Get(EMemoryRAM)//EMemoryRAMFree 
	int memRam, memRamFree;
	HAL::Get(HALData::EMemoryRAM, memRam);
	HAL::Get(HALData::EMemoryRAMFree, memRamFree);

	TBuf<256> buf;
#if 1
	//	buf.Format(_L("Alloc: %d_%d_%d"), totalAllocSize / (1024 * 1024),
	//			(totalAllocSize % (1024 * 1024)) / 1024, totalAllocSize % 1024);
#ifdef _DEBUG
	buf.Format(_L("Cur: %d, %d/%d     oh: %d-%d  vl: %d-%d   p: %d"),
			totalAllocSize, memRamFree, memRam, Object::objectCount,
			Handle::handleCount, CBaseViewX::baseViewCount,
			CLayoutEleX::layoutCount, ImgSource::count);
#endif
#else
	TMemoryInfoV1Buf info;
	UserHal::MemoryInfo(info);
	buf.Format(_L("Alloc: %d, "
					"TRam: %d, "
					"TRom: %d, "
					"MaxFreeRam: %d,"
					"FreeRam: %d, "
					"InterDiskRam: %d"), totalAllocSize, info().iTotalRamInBytes,
			info().iTotalRomInBytes, info().iMaxFreeRamInBytes,
			info().iFreeRamInBytes, info().iMaxFreeRamInBytes);
#endif
	return buf;
#endif
}
#endif
List String::Split(char aSeperate){ 
	List list;
	bool loop = true;
	if (length() <= 0)
		return list;
	TUint pos = 0;
	while (loop) {
		TPtr ptr(desc().Mid(pos));
		TInt curLength = ptr.LocateF(aSeperate);
		if (KErrNotFound != curLength) {
			TPtr ptrarray(desc().Mid(pos, curLength));
       //     printf("%d\n", ptrarray.length);
			list.add(String(ptrarray));
			pos += curLength;
			pos++;//ingore the char;
		} else {
			TPtr ptrarray(desc().Mid(pos));
			list.add(String(ptrarray));
			loop = false;
		}
	}
	return list;
}
bool String::isArabic(TUint16 ch) {
	return ch >= 0x0600 && ch <= 0x06ff;
}
TBool String::IsCJK(TUint16 ch) {
	return (ch >= 0x2e80 && ch <= 0x9fff) || (ch >= 0xf900 && ch <= 0xfaff);
}
TBool String::IsMarks(TUint16 ch) {
	return ch == ',' || ch == '.' || ch == ' ' || ch == '-' || ch == '?' || ch
			== '!' /*|| ch == ':'*/;
}
//
//void String::fuck(TFuckingType type, const TPtr& aString, List aList,
//		const List& aSmileList, TBool havaFaceTag) {
//	TPtrC word = aString;
//	if (havaFaceTag) {
//		for (int j = 0; j < aSmileList.size(); j++) {
//			String faceEle = aSmileList.get(j);
//			int findPos = word.Find(faceEle);
//			if (findPos != KErrNotFound) {
//				TPtrC wordTmp = word.Mid(0, findPos);
//				if (wordTmp.Length() > 0) {
//					fuck(type, wordTmp, aList, aSmileList, EFalse);
//				}
//				TPtrC faceTmp = word.Mid(findPos, faceEle.length());
//				aList.add(Pair(Integer(E_Smile), String(faceTmp)));//Face
//				int curPos = findPos + faceEle.length();
//				word.Set(word.Mid(curPos, word.Length() - curPos));
//				j = -1;
//				continue;
//			}
//		}
//	}
//	{//for separete
//		int sepPos = 0;
//		int sepLength = 0;
//		for (int j = 0; j < word.Length(); j++) {
//			TUint16 curTmpChar = word[j];
//			sepLength++;
//			if (IsMarks(curTmpChar)) {
//				if (sepLength > 1) {//for test
//					TPtrC sepWord = word.Mid(sepPos, sepLength - 1);
//					aList.add(Pair(Integer(type), String(sepWord)));
//					sepPos = j;
//					sepLength = 1;
//				}
//				//for separete
//				TPtrC sepWord = word.Mid(sepPos, sepLength);
//				aList.add(Pair(Integer(E_Marks), String(sepWord)));
//				sepLength = 0;
//				sepPos = j + 1;
//			} else if (j == word.Length() - 1) {
//				TPtrC sepWord = word.Mid(sepPos, sepLength);
//				aList.add(Pair(Integer(type), String(sepWord)));
//			}
//		}
//	}
//}
//
//TRgb String::toRgb() {
//	TPtrC rgbStr = desc();//.Mid(1, 6);
//	TUint32 bb = String::toIntRadix(rgbStr.Mid(0, 2), EHex);
//	TUint32 gg = String::toIntRadix(rgbStr.Mid(2, 2), EHex);
//	TUint32 rr = String::toIntRadix(rgbStr.Mid(4, 2), EHex);
//	return TRgb(rr, gg, bb);
//	
////	for (int i = 0; i < 6; i++) {
////		TUint16 ch = upper.GetDes()[i];
////		if (ch <= 'F' && ch >= 'A') {
////			result += (ch - 'A' + 10) << (4 * (5 - i));
////		} else if (ch <= '9' && ch >= '0') {
////			result += (ch - '0') << (4 * (5 - i));
////		} else {
////			valid = false;
////			break;
////		}
////	}
//	
//}
//List String::lines(const List& aSmileList) const {
//	//draw text string, need consider space return line and \n, \r\n, \f
//	List list;
//	TRect rect;
//	int pos = 0;
//	int length = 0;
//	TPoint point;
//	TPtrC _text = desc();
//	RDEBUG(_text);
//
//	for (int i = 0; i < _text.Length(); i++) {
//		TUint16 curChar = _text[i];
//		if (curChar == '\r') {
//			char nextchar = _text[i + 1];
//			length = 0;
//			pos = i + 1;
//			if (nextchar == '\n') {
//			} else {
//			}
//		} else if (curChar == '\n') {
//			list.add(Pair(Integer(E_NewLine), String("")));
//			length = 0;
//			pos = i + 1;
//		} else if (curChar == '\f') {
//			list.add(Pair(Integer(E_NewLine), String("")));
//			length = 0;
//			pos = i + 1;
//		} else if (i < _text.Length()) {//ABC abc 123
//			TFuckingType typE;
//			if (IsCJK(curChar))
//				typE = E_SQUARE;
//			else if (isArabic(curChar))
//				typE = E_R2L;
//			else
//				typE = E_L2R;
//			length++;
//			TBool wordEnd = EFalse;
//			if (i < _text.Length() - 2) {
//				TUint16 nextChar = _text[i + 1];
//				if (nextChar == '\r') {
//					wordEnd = ETrue;
//				} else if (nextChar == '\n') {
//					wordEnd = ETrue;
//				} else if (nextChar == '\f') {
//					wordEnd = ETrue;
//				} else {
//					TFuckingType typeNext;
//					if (IsCJK(nextChar))
//						typeNext = E_SQUARE;
//					else if (isArabic(nextChar))
//						typeNext = E_R2L;
//					else
//						typeNext = E_L2R;
//					if (typeNext != typE)
//						wordEnd = ETrue;
//				}
//			} else if (i == _text.Length() - 1) {
//				wordEnd = ETrue;
//			}
//			if (wordEnd) {//for smile
//				TPtrC word = _text.Mid(pos, length);
//				fuck(typE, word, list, aSmileList);
//				pos = i;
//				length = 0;
//			}
//		}
//	}
//	return list;
//}
