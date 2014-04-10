//
//  Util.cpp
//  X_V
//
//  Created by zouxu on 7/7/13.
//  Copyright (c) 2013 zouxu. All rights reserved.
//

#include "Util.h"
//#include "jsonXV.h"
#include "jsonXVdefines.h"
#include "Integer.h"
#include "Handle.h"
#include "List.h"
#include "Pair.h"
#include "Bytes.h"
#include "String.h"
#include "Map.h"
#include "ByteArrayOutputStream.h"
#include "ByteArrayInputStream.h"
#include "unicode_utf8.c"
#include "crc32.h"
#include "md5.h"
#include <stdio.h>
#include "base64.h"
#include <math.h>
#include <limits.h>
#include <float.h>
#include "UtilPlatform.h"
#include "Ptr.h"
//#include <stdio.h>

int Util::ConvertToUnicodeFromUtf8(TPtr& ptr, const TPtr8& ptr8){
    int length = ptr.maxLength;
    int err = enc_utf8_to_unicode_str((unsigned char *)ptr8.source, ( unsigned short *)ptr.source, &length);
    ptr.length = length;
    return err;
}
int Util::ConvertFromUnicodeToUtf8(TPtr8& ptr8, const TPtr& ptr){
    int length = ptr8.maxLength;
    int err = enc_unicode_to_utf8_str(ptr.source, (unsigned char *)ptr8.source, &length);
    ptr8.length = length;
    return err;
}


void Util::memtrim( const void* pvFrom, unsigned int size)
{
	assert (pvFrom!= NULL); 
	unsigned char* pbFrom = (unsigned char*)pvFrom;
	while(size-- > 0)
		 *pbFrom++=0; 
}


//	pvFrom		|		[---mem---]	|	[---mem---]		|	[---mem---]					|
//	left2Right	|		true		|		false		|			true or false		|
//	pvTo		|	[---mem---]		|		[---mem---]	|					[---mem---]	|

void* Util::memcpy(void* pvTo, const void* pvFrom, unsigned int size )
{
	if(!size)return const_cast<void*>( pvFrom);
	bool left2Right = true;
	if(pvFrom<pvTo)
		left2Right = false;
//	printf("%d -- %d  %d\n", size, pvFrom, pvTo);
	assert((pvTo!= NULL) && (pvFrom!= NULL));
	if(left2Right){
		unsigned char* pbTo = (unsigned char*)pvTo;
		unsigned char* pbFrom = (unsigned char*)pvFrom;
		while(size-- > 0) 
			*pbTo++ = *pbFrom++;
	}else{
		unsigned char* pbTo = (unsigned char*)pvTo;
		unsigned char* pbFrom = (unsigned char*)pvFrom;
		pbTo+=size-1;
		pbFrom+=size-1;
//		pbTo+=size;
//		pbFrom+=size;
		//size+=1;
		//size-=1;
		while(size-- > 0){
	//		printf("%d --\n", size);
			*pbTo-- = *pbFrom--;
		}
	}
	return pvTo;
}


void Util::ObjectSavingDo(const Object& object, ByteArrayOutputStream& output) {
	output.writeInt(int(object.getType()));
	switch (object.getType()) {
        case OT_Integer: {
            output.writeInt64(Integer(object));
            break;
        }
        case OT_String: {
            //		output.writeString(String(object).getBytes());
           //by_zouxu output.writeStringByUnicode(object);
            break;
        }
        case OT_List: {
            List list(object);
            int listSize = list.size();
            output.writeInt(listSize);
            for (int i = 0; i < listSize; i++) {
                ObjectSavingDo(list.get(i), output);
            }
            break;
        }
        case OT_Map: {
#if 0
            Map map(object);
            List tables = map.getTables();
            int tablesSize = tables.size();
            int itemsSize = 0;
            for (int i = 0; i < tablesSize; i++) {
                List items = tables.get(i);
                itemsSize = items.size();
                for (int j = 0; j < itemsSize; j++) {
                    Pair pair = (Pair) items.get(j);
                    ObjectSavingDo(pair.getKey(), output);
                    ObjectSavingDo(pair.getValue(), output);
                }
            }
#else
            Map map(object);
            List keyList = map.getKeys();
            int keyListSize = keyList.size();
            output.writeInt(keyListSize);
            for (int i = 0; i < keyListSize; i++) {
                Object key = keyList.get(i);
                Object value = map.get(key);
                ObjectSavingDo(key, output);
                ObjectSavingDo(value, output);
            }
#endif
            break;
        } //	case OT_Bytes:
        default:
            LEAVE(ET_TypeError_NotFound);
	}
}


void Util::ObjectSave(const String& aFileName, const Object& object ){
	
	Bytes bytes = object.toBytes();
	const char* content = (const char*)bytes.des().source;
	Bytes fN = aFileName.toBytes();
	const char* fileName =(const char*) fN.des().source;
	fileWrite_Replace(fileName,content, bytes.length());
	
}

bool Util::ObjectLoad(const String& aFileName, Object& object){
	char* content = NULL; 
	Bytes fN = aFileName.toBytes();
	const char* fileName =(const char*) fN.des().source;
    int length;
	fileRead(fileName,&content, &length);
	if(content){ 
		Bytes byte(content);
		Object tmp = byte.toObject();
		if(OT_Object == tmp.getType())
			return false;
		object = tmp;
		return true;//by_zouxu toObject is not judge
	}
	return false;
}




void Util::ObjectSaving(const Object& object, const String& aFileName) {
    
#ifdef __WINS__
	Bytes bytes = object.toBytes();
	CFileProxy::writeReplace(aFileName, bytes);
#else
	ByteArrayOutputStream output;
	ObjectSavingDo(object, output);
	Bytes bytes = output.toBytes();
	//by_zouxu CFileProxy::writeReplace(aFileName, bytes);
#endif
}

void Util::ObjectReadingDo(Object& object, ByteArrayInputStream& input) {
	TUint type = input.readInt();
    
	switch (type) {
        case OT_Integer: {
            object = Integer(input.readInt64());
            break;
        }
        case OT_String: {
            //		object = input.readString();
           //by_zouxu object = input.readStringByUnicode();
            break;
        }
        case OT_List: {
            int listSize = input.readInt();
            List list(listSize / 2 + 1);
            for (int i = 0; i < listSize; i++) {
                Object objectTmp;
                ObjectReadingDo(objectTmp, input);
                list.add(objectTmp);
            }
            object = list;
            break;
        }
        case OT_Map: {
            int keyListSize = input.readInt();
            Map map(keyListSize / 4 + 1);
            for (int i = 0; i < keyListSize; i++) {
                Object key, value;
                ObjectReadingDo(key, input);
                ObjectReadingDo(value, input);
                map.put(key, value);
            }
            object = map;
            break;
        }
        default:
            LEAVE(ET_TypeError_NotFound);
	}
}

bool Util::ObjectReading(Object& object, const String& aFileName) {
    
#ifdef __WINS__
	Bytes bytes = CFileProxy::read(aFileName);
	TBool boo = bytes.toObject(object);
	return boo;
#else
	Bytes bytes ="";//= CFileProxy::read(aFileName);
	if (bytes.length() <= 0)
        return false;
	ByteArrayInputStream input(bytes);
	ObjectReadingDo(object, input);
	return true;
#endif
}
 void Util::mapTocJSON(cJSON* json, const String& aKey, const Object& aObject) {
	//	Bytes keyChat = aKey.getBytes();
	//	ObjectType type = aObject.type;
	//	cJSON * object = cJSON_CreateObject();
	//	cJSON_AddItemToObject(json, keyChat.charc(), object);
	//	addMapTocJSON(json, aKey, aObject);
	Map map = Map(aObject);
	List numToStrList;
    
#if 0//for performance
	List tables = map.getTables();
	int tablesSize = tables.size();
	int itemsSize = 0;
	for (int i = 0; i < tablesSize; i++) {
		List items = tables.get(i);
		itemsSize = items.size();
		for (int j = 0; j < itemsSize; j++) {
			Pair pair = (Pair) items.get(j);
			objectTocJSON(json, pair.getKey(), pair.getValue(), numToStrList);
		}
	}
#else
	List keyList = map.getKeys();
	 int size = keyList.size();
	for (int i = 0; i < size; i++) {
		String key(keyList.get(i));
		Object value(map.get(key));
		objectTocJSON(json, key, value, numToStrList);
	}
#endif
    
	if (numToStrList.size() > 0) {
		static String key("fuckingNokiaNum");
		String value("");
		for (int i = 0; i < numToStrList.size(); i++) {
			value += numToStrList.get(i);
			if (i != numToStrList.size() - 1)
				value += ",";
		}
		objectTocJSON(json, key, value, numToStrList);
	}
}
cJSON* Util::addListTocJSON(cJSON* json, const String& aKey,
                           const Object& aObject) {
	//ObjectType type = aObject.getType();
	cJSON * object = cJSON_CreateArray();
	//	cJSON_AddItemToObject(json, keyChat.charc(), object);
	if (aKey.length() > 0) {
		Bytes keyChat = aKey.toBytes();
		cJSON_AddItemToObject(json, keyChat.charc(), object);
	} else
		cJSON_AddItemToArray(json, object);
	return object;
}
cJSON* Util::addMapTocJSON(cJSON* json, const String& aKey,
                          const Object& aObject) {
	//ObjectType type = aObject.getType();
	cJSON * object = cJSON_CreateObject();
	//	cJSON_AddItemToObject(json, keyChat.charc(), object);
	if (aKey.length() > 0) {
		Bytes keyChat = aKey.toBytes();
		cJSON_AddItemToObject(json, keyChat.charc(), object);
	} else
		cJSON_AddItemToArray(json, object);
	return object;
}

void Util::listTocJSON(cJSON* json, const String& aKey, const Object& aObject) {
	//	Bytes keyChat = aKey.getBytes();
	//	ObjectType type = aObject.type;
	//	cJSON * object = cJSON_CreateArray();
	//	cJSON_AddItemToObject(json, keyChat.charc(), object);
	//	addListTocJSON(json, aKey, aObject);
	List list = List(aObject);
	List numToStrList;
	for (int i = 0; i < list.size(); i++) {
		Object item(list.get(i));
		objectTocJSON(json, String(""), item, numToStrList);
	}
}
void Util::integerTocJSON(cJSON* json, const String& aKey,
                         const Object& aObject, List& aNum2StrList) {
	Integer number = aObject;
    
	//"double" represented "int64", but double accuracy is 15 only, so i use string replace.
	if (number._TInt64() > 4294967295) {
		aNum2StrList.add(aKey);
		return stringTocJSON(json, aKey, aObject);
	}
	cJSON * object = cJSON_CreateNumber(number);
	if (aKey.length() > 0) {
		Bytes keyChat = aKey.toBytes();
		cJSON_AddItemToObject(json, keyChat.charc(), object);
	} else
		cJSON_AddItemToArray(json, object);
}
void Util::stringTocJSON(cJSON* json, const String& aKey, const Object& aObject) {
    
#if 1
	Bytes keyChat = aKey.toBytes();
	String value = aObject.toString();
    
	Bytes valueB = value.toBytes();
	char* ch = (char*) valueB.getChatAndEmptySelf();
    
	cJSON * item = cJSON_CreateNull();
	if (item) {
		item->type = cJSON_String;
		item->valuestring = ch;
	}
    
	if (aKey.length() > 0) {
#if 1
		char* ch = (char*) keyChat.charc();
		cJSON_AddItemToObject(json, ch, item);
#else
		char* ch = (char*) keyChat.getChatAndEmptySelf();
		cJSON_AddItemToObject_WithOutMemCopy(json, ch, item);
#endif
	} else
		cJSON_AddItemToArray(json, item);
#else
	Bytes keyChat = aKey.getBytes();
	//	String value = String(aObject);
	String value = aObject.toString();
	const char* ch = "";
	//!!!!! don't move the arg
	Bytes valueB;//must put this place, because ch is ptr of it
	if (value.length() > 0) {
		valueB = value.getBytes();
		ch = valueB.charc();
	}
	//	const char* cha = valueB.charc();
	//	cJSON * object = cJSON_CreateString(valueB.charc());
	cJSON * object = cJSON_CreateString(ch);
	if (aKey.length() > 0)
        cJSON_AddItemToObject(json, keyChat.charc(), object);
	else
        cJSON_AddItemToArray(json, object);
#endif
}
void Util::objectTocJSON(cJSON* json, const String& aKey, const Object& aObject,
                        List& aNum2StrList) {
	//Bytes keyChat = aKey.getBytes();
	ObjectType type = aObject.getType();
	switch (type) {
        case OT_List:
            return listTocJSON(addListTocJSON(json, aKey, aObject), aKey, aObject);
        case OT_Map:
            return mapTocJSON(addMapTocJSON(json, aKey, aObject), aKey, aObject);
        case OT_Integer:
            return integerTocJSON(json, aKey, aObject, aNum2StrList);
        case OT_String:
        default:
            return stringTocJSON(json, aKey, aObject);
	}
}
Bytes Util::tocJSONBytes(const Object& aObject) {
	Object ob(aObject);
    
	cJSON* root = NULL;
	//cJSON_CreateObject();
	if (OT_List == aObject.getType()) {
		root = cJSON_CreateArray();
		listTocJSON(root, String(""), aObject);
	} else if (OT_Map == aObject.getType()) {
		root = cJSON_CreateObject();
		mapTocJSON(root, String(""), aObject);
	}
    
	char* out = cJSON_Print(root);
	Bytes res = Bytes(out);
	SAFE_DEL(out);
	cJSON_Delete(root);
	//	DEBUG << "Str:" << res;
	return res;
}

bool Util::jsonData2Object(const Bytes& aData, Object& obj) {
	cJSON * json = cJSON_Parse(aData.charc());
	if (!json)
		return false;
	if (cJSON_Array == json->type)
		obj = Util::cJSON2List(json);
	else if (cJSON_Object == json->type)
		obj = Util::cJSON2Map(json);
	else {
		cJSON_Delete(json);
		return false;
	}
	cJSON_Delete(json);
	return true;
}
List Util::cJSON2List(cJSON* array) {
	List list;
	int arraySize = cJSON_GetArraySize(array);
	for (int i = 0; i < arraySize; i++) {
		cJSON * item = cJSON_GetArrayItem(array, i);
		list.add(cJSON2Object(item));
	}
	return list;
}
Object Util::cJSON2Object(cJSON* item) {
	switch (item->type) {
        case cJSON_False:
            return Integer(0);
        case cJSON_True:
            return Integer(1);
        case cJSON_NULL:
            //		return Integer(-1);
            return String("");
        case cJSON_Number:
            return Integer(item->valuedouble);
        case cJSON_String:
            return String(Bytes(item->valuestring).des());
        case cJSON_Array:
            return cJSON2List(item);
        case cJSON_Object:
            return cJSON2Map(item);
	}
	Object value;
	return value;
}
Map Util::cJSON2Map(cJSON* json) {
	int mapSize = 1;
	Map map(mapSize);
	if (!json)
		return map;
	{
		cJSON* temJson = json->child;
		while (temJson) {
			temJson = temJson->next;
			mapSize++;
		}
		mapSize /= 4;
		if (mapSize < 1)
			mapSize = 1;
		if (mapSize > MapSizeNormal)
			mapSize = MapSizeNormal;
		map = Map(mapSize);
	}
    
	cJSON* temJson = json->child;
	while (temJson) {
		//String key(Bytes(temJson->string).desc());
		String key(temJson->string);
		//{
		//	Bytes bytes = key.getBytes();
		//	printf("%s\n", bytes.des().source);
		//}
		Object value(cJSON2Object(temJson));
		temJson = temJson->next;
		map.put(key, value);
	}
	//	int64以string存成json，json会把这个转换的Tag 存到fuckingNokiaNum标签，转回来的时候会先判断fuckingNokiaNum这个标签。然后把string转会intge
	static String key("fuckingNokiaNum");
	if (map.contains(key))
	{
		String tagsStr = map.get(key);
		List tags = tagsStr.Split(',');
		for (int i = 0; i < tags.size(); i++) {
			String tag = tags.get(i);
			Integer integer(map.get(tag));
			//map.remove(tag);
			map.put(tag, integer);
		}
		map.remove(key);
	}
    
	return map;
}



void Util::md5decrypt(const Bytes& bytes,  unsigned char* decrypt){
    MD5_CTX md5;
	MD5Init(&md5); 
	//unsigned char encrypt[] ="admin";//21232f297a57a5a743894a0e4a801fc3
	unsigned char* encrypt= bytes.desc().source;
	//unsigned char decrypt[16];
	MD5Update(&md5,encrypt,strlen((char *)encrypt));
	MD5Final(&md5,decrypt);
    
//    char* ptr = new char[64];
//	char* ptrTmp = ptr;
//	for(i=0;i<16;i++)
//	{
//		//printf("%02x",decrypt[i]);
//		sprintf(ptrTmp,"%02x",decrypt[i]);
//		ptrTmp+=2;
//	}
//	String md5Str(ptr);
//	//printf("\nstr:%s\n",ptr);
//	delete [] ptr;
//	
//	return md5Str;
}

Bytes Util::md5Bytes(const Bytes& bytes){
    unsigned char* decrypt = new unsigned char[16+1];
    md5decrypt(bytes, decrypt);
    decrypt[16]=0;
    return Bytes(decrypt, 16);
   //return Bytes((const char*)decrypt);
}
String Util::md5(const Bytes& bytes){
    
    unsigned char decrypt[16+1];
    md5decrypt(bytes, decrypt);
    
    char* ptr = new char[32+1];
    char* ptrTmp = ptr;
    for(int i=0;i<16;i++){
        //printf("%02x",decrypt[i]);
        sprintf(ptrTmp,"%02x",decrypt[i]);
        ptrTmp+=2;
    }
    ptr[32]= 0;
    String md5Str(ptr);
    //printf("\nstr:%s\n",ptr);
    delete [] ptr;
    return md5Str;
    
//	MD5_CTX md5;
//	MD5Init(&md5);
//	int i;
//	//unsigned char encrypt[] ="admin";//21232f297a57a5a743894a0e4a801fc3
//	unsigned char* encrypt= bytes.desc().source;
//	unsigned char decrypt[16];
//	MD5Update(&md5,encrypt,strlen((char *)encrypt));
//	MD5Final(&md5,decrypt);
//	//printf("加密前:%s\n加密后:",encrypt);
//	
//	char* ptr = new char[64];
//	char* ptrTmp = ptr;
//	for(i=0;i<16;i++)
//	{
//		//printf("%02x",decrypt[i]);
//		sprintf(ptrTmp,"%02x",decrypt[i]);
//		ptrTmp+=2;
//	}
//	String md5Str(ptr);
//	//printf("\nstr:%s\n",ptr);
//	delete [] ptr;
//	
//	return md5Str;
}
unsigned int Util::crc32(const Bytes& bytes){
	//unsigned char encrypt[] ="admin";//0x880E0D76
	//long crc32 = crc(encrypt, 5);
    //	unsigned char* encrypt= bytes.desc().source;
	unsigned char* encrypt=(unsigned char*) bytes.Char();
	unsigned int crc32 = crc(encrypt, bytes.length());
//	unsigned int crc32 = crc(encrypt, strlen((char *)encrypt));
	//printf("crc32:%X\n",crc321);
	return crc32;
}

Bytes Util::base64Encode(const Bytes& bytes){
	char* source= (char*)bytes.desc().source;
	char *to = new char[bytes.length()*4/3+10];
	long length= base64_encode(to, source, strlen((char *)source));
	Bytes tmp((TUint8*)to, length);
	return tmp;
}

Bytes Util::base64Decode(const Bytes& bytes){ 
	char* source= (char*)bytes.desc().source;
	char *to = new char[bytes.length()*3/4+10];
	long length= base64_decode(to, source, strlen((char *)source));
	Bytes tmp((TUint8*)to, length);
	return tmp;
}


bool Util::string_2_number(const short* num,long long & intValue,  double&doubleValue){
    double n=0,sign=1,scale=0;int subscale=0,signsubscale=1;
    
	/* Could use sscanf for this? */
	if (*num=='-') sign=-1,num++;	/* Has sign? */
	if (*num=='0') num++;			/* is zero */
	if (*num>='1' && *num<='9')	do	n=(n*10.0)+(*num++ -'0');	while (*num>='0' && *num<='9');	/* Number? */
	if (*num=='.' && num[1]>='0' && num[1]<='9') {num++;		do	n=(n*10.0)+(*num++ -'0'),scale--; while (*num>='0' && *num<='9');}	/* Fractional part? */
	if (*num=='e' || *num=='E')		/* Exponent? */
	{	num++;if (*num=='+') num++;	else if (*num=='-') signsubscale=-1,num++;		/* With sign? */
		while (*num>='0' && *num<='9') subscale=(subscale*10)+(*num++ - '0');	/* Number? */
	}
    
	n=sign*n*pow(10.0,(scale+subscale*signsubscale));	/* number = +/- number.fraction * 10^+/- exponent */
	
  //  printf("%s %d %f\n", (char*)num,(int)n, n);
    
    intValue=(int)n;
	doubleValue =n;
    return true;
}

void Util::number_2_string(char** num,long long intValue,  double doubleValue){
 	char *str;
   // short* tmp;
	double d=doubleValue;
	if (fabs(((double)intValue)-d)<=DBL_EPSILON && d<=INT_MAX && d>=INT_MIN)
	{
		str=new char[21+1];	/* 2^64+1 can be represented in 21 chars. */
     //   tmp = new short [21+1];
		//if (str) sprintf(str,"%lld",intValue);
        if (str) sprintf(str,"%d",(int)intValue);
	}
	else
	{
		str=(char*)malloc(64+1);	/* This is a nice tradeoff. */
   //     tmp = new short [64+1];
		if (str)
		{
			if (fabs(floor(d)-d)<=DBL_EPSILON && fabs(d)<1.0e60)sprintf(str,"%.0f",d);
			else if (fabs(d)<1.0e-6 || fabs(d)>1.0e9)			sprintf(str,"%e",d);
			else												sprintf(str,"%f",d);
		}
	}
    
    int length = strlen(str);
    str[length]=0;
    *num = str;
//    num = &str;
//    for(int i=0; i<length; i++){
//        tmp[i]=str[i];
//    }
//    tmp[length]=0;
    
   // num = &tmp;
}

