//
//  Util.h
//  X_V
//
//  Created by zouxu on 7/7/13.
//  Copyright (c) 2013 zouxu. All rights reserved.
//

#ifndef __X_V__Util__
#define __X_V__Util__

#include "cJSON.h"
class TPtr;
class TPtr8;
class String;
class Object;
class List;
class Bytes;
class Map;
class ByteArrayOutputStream;
class ByteArrayInputStream;

#define SAFE_DEL(aPtr) if(aPtr) { delete aPtr; aPtr = NULL;}

class Util{
public:
//UTF8 <-> UNICODE
	static int ConvertToUnicodeFromUtf8(TPtr& ptr, const TPtr8& ptr8);
	static int ConvertFromUnicodeToUtf8(TPtr8& ptr8, const TPtr& ptr);

//MEM
public:
    static void memtrim( const void* pvFrom, unsigned int size);
	static void* memcpy(void* pvTo, const void* pvFrom, unsigned int size);
	
	
//Object<->save, not used
public:
	static void ObjectSave(const String& aFileName, const Object& object );
	static bool ObjectLoad(const String& aFileName, Object& object);
	
	static void ObjectSaving(const Object& object, const String& aFileName);
	static bool ObjectReading(Object& object, const String& aFileName);
private:
	static void ObjectSavingDo(const Object& object, ByteArrayOutputStream& output);
	static void ObjectReadingDo(Object& object, ByteArrayInputStream& input);
	
	
//JSON Object -> STRING
public:
	static Bytes tocJSONBytes(const Object& aObject);
private:
	static void objectTocJSON(cJSON* json, const String& aKey, const Object& aObject, List& aNum2StrList);
	static void mapTocJSON(cJSON* json, const String& aKey, const Object& aObject);
	static void listTocJSON(cJSON* json, const String& aKey, const Object& aObject);
	static void integerTocJSON(cJSON* json, const String& aKey, const Object& aObject, List& aNum2StrList);
	static void stringTocJSON(cJSON* json, const String& aKey, const Object& aObject);
	static cJSON* addListTocJSON(cJSON* json, const String& aKey, const Object& aObject);
	static cJSON* addMapTocJSON(cJSON* json, const String& aKey, const Object& aObject);

//STRING ->JSON OBject
public: 
	static bool jsonData2Object(const Bytes& aData, Object& obj);
private:
	static Map cJSON2Map(cJSON* json);
	static List cJSON2List(cJSON* array);
	static Object cJSON2Object(cJSON* item);
	
	
//HASH
public:
	static String md5(const Bytes& bytes);
	static Bytes md5Bytes(const Bytes& bytes);
	static unsigned int crc32(const Bytes& bytes);
	static Bytes base64Encode(const Bytes& bytes);
	static Bytes base64Decode(const Bytes& bytes);
private:
    static void md5decrypt(const Bytes& bytes, unsigned char* decrypt);

    
    //PARSER NUMBER
public:
    static bool string_2_number(const short* num,long long & intValue,  double&doubleValue);
    static void number_2_string(char** num,long long  intValue,  double doubleValue);
};




#endif /* defined(__X_V__Util__) */
