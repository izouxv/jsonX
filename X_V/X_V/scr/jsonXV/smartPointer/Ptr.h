/*
 ============================================================================
 Name		: Ptr.h
 Author	  : 
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CObject declaration
 ============================================================================
 */

#ifndef Ptr_H
#define Ptr_H

#include "jsonXVdefines.h"

class TPtr{
public:
    TUint16*source;
    TUint length;
    TUint maxLength;
public:
//	TPtr(int, int){source=NULL,length=0;maxLength=0;};
	//TPtr(const TPtr& tmp){Set(tmp.source, tmp.length, tmp.maxLength);};
    TPtr(){source=NULL,length=0;maxLength=0;};
	TPtr(TUint16*ptr,TUint len, TUint maxLen){Set(ptr,len, maxLen);};
	inline int Length()const{return length;};
	inline void Set(TUint16*ptr, TUint len, TUint maxLen){source = ptr;length=len;maxLength=maxLen;};
	void operator+=(const TPtr& str){
        if(maxLength-length<str.length)
            return;
        for(int i=0; i<str.length;i++)
            source[length+i]=str.source[i];
        length+=str.length;
    };
	inline bool operator==(const TPtr& ref) const{
        if(length!=ref.length)
            return false;
        for(int i=0; i<length; i++){
            if(source[i]!=ref.source[i])
                return false;
        }
        return true; 
    };
    inline int operator[] (TUint x){
        if(x>=length)return 0;
        return source[x];
    };
	void Delete(TUint pos, TUint len){
        if(pos+len>=length)return;
        for(int i=0; i<len; i++)
            source[pos+i]=source[pos+len+i];
        length+=len;
    };
	void Append(TPtr ptr){
        if(length+ptr.length>maxLength)return;
        for(int i=0; i<ptr.length; i++)
            source[length+i]=ptr.source[i];
        length+=ptr.length;
    };
	void LowerCase(){ 
        for(int i=0; i<length; i++)
            if(source[i]>='A' && source[i]<='Z')
                source[i]+=0x20;
    };
	void UpperCase(){
        for(int i=0; i<length; i++)
            if(source[i]>='a' && source[i]<='z')
                source[i]-=0x20;
    };
	int Find(TPtr ptr){
        return 0;
    };
	TPtr Mid(TUint pos)const{
        if(pos>=length)return TPtr();
        return TPtr(source+pos, length-pos, length-pos);
    };
	TPtr Mid(TUint pos, TUint aLength)const{
        if(pos+aLength>=length)return TPtr();
        return TPtr(source+pos, aLength, aLength);
    };
    int LocateF(char aChar){ 
        TUint16*tmp=source;
        int pos = 0;
        while(tmp && *tmp && aChar!=*tmp++)
            pos++;
        if(pos == length)
            return -1;
        return pos;
    }
    TPtr& operator=(const TPtr& ref){
        if (this == &ref )
            return *this;
        //by_zouxu, this need delete and alloc and copy, but currently i am not in the mood to do it
        if(maxLength<ref.length)
            return *this;
        for(int i=0; i<ref.length; i++){
            source[i]=ref.source[i];
        }
        length=ref.length; 
        source[length]=0;
        return *this;
    };
	
};
class TPtr8{
    public:
    TUint8*source;
    TUint length;
    TUint maxLength;
public:
    TPtr8(){source=NULL,length=0;maxLength=0;};
//	TPtr8(int, int){};
//	TPtr8(const TPtr8&){};
	TPtr8(TUint8*ptr, TUint len, TUint maxLen){
        Set(ptr,len, maxLen);
    };
	inline int Length()const{return length;};
	inline void Set(TUint8*ptr, TUint len, TUint maxLen){source = ptr;length=len;maxLength=maxLen;};
	void operator+=(const TPtr8& str){
        if(maxLength-length<str.length)
            return;
        for(int i=0; i<str.length;i++)
            source[length+i]=str.source[i];
        length+=str.length;
    };
	bool operator==(const TPtr8& ref){
        if(length!=ref.length)
            return false; 
        for(int i=0; i<length; i++){
            if(source[i]!=ref.source[i])
                return false;
        }
        return true;
    };
	TUint8& operator[] (int x){
        return *(this-> source+x);
    };
//	void Delete(int pos, int length){};
	void Append(TPtr8 ptr){
        if(length+ptr.length>maxLength)return;
        for(int i=0; i<ptr.length; i++)
            source[length+i]=ptr.source[i];
        length+=ptr.length;
    };
//	void LowerCase(){};
//	void UpperCase(){};
//	int Find(TPtr8){};
	TPtr8 Mid(TUint pos){
        if(pos>=length)return TPtr8();
        return TPtr8(source+pos, length-pos, length-pos);
    };
    TPtr8 Mid(TUint pos, TUint aLength)const{
        if(pos >=length)return TPtr8();
        int leng = aLength;
        if(pos+leng>=length)
            leng=length-pos;
        return TPtr8(source+pos, leng, leng);
    };
    TPtr8& operator=(const TPtr8& ref){
        //set a empty TPtr8 not tested, by_zouxu
            if (this == &ref )
                return *this;
            //by_zouxu, this need delete and alloc and copy
            if(maxLength<ref.length)
               return *this;
        
        for(int i=0; i<ref.length; i++){
            source[i]=ref.source[i];
        }
        length=ref.length;
        source[length]=0;
            return *this;
    };
};
 

#endif // Ptr_H
