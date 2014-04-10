/*
 ============================================================================
 Name		: Object.cpp
 Author	  : 
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CObject implementation
 ============================================================================
 */

#include "Object.h"
#include "Handle.h" 
#include "String.h"
#include "Map.h"
#include "List.h"
#include "Integer.h"
#include "Util.h"
#include "Bytes.h"



//	operator TInt64() const;
//	operator int() const;
//	operator double() const;
//	INLINE int getTYPE() const;


Object::Object() {
	handle = NULL;
#ifdef _DEBUG 
	objectCount++;
#endif
}

//Object::Object(ObjectType aType) {
//	handle = null;
//	type = aType;
//#ifdef __WINS__ 
//	objectCount++;
//#endif
//}
Object::Object(XV::Handle* handle) {
	updateHandle(handle);
	//	type = aType;
	incRef();
#ifdef _DEBUG 
	objectCount++;
#endif
}
Object::Object(const Object& obj) {
    if(OT_ObjArrayOper == obj.getType()){
		handle = NULL;
        return;
	}
    
	updateHandle(obj.handle);
	//	type = obj.getType();
	incRef();
#ifdef _DEBUG 
	objectCount++;
#endif
}


Object::~Object() {
	deRef();
	handle = null;
#ifdef _DEBUG 
	objectCount--;
#endif
}

Object::Object(int num){ 
	Integer tmp(num);
	updateHandle(tmp.handle);
	incRef();
#ifdef _DEBUG
	objectCount++;
#endif
}

Object::Object(double num){
	Integer tmp(num);
	tmp.setDouble(num);
	updateHandle(tmp.handle);
	incRef();
#ifdef _DEBUG
	objectCount++;
#endif
} 


//Object::operator TInt64() const{
//	return Integer(*this)._TInt64();
//}
//Object::operator int() const{ 
//	return Integer(*this)._TInt64();
//}
//Object::operator double() const{ 
//	return Integer(*this)._double();
//}

Object::Object(const char* chars){
	String tmp(chars);
	updateHandle(tmp.handle);
	incRef();
#ifdef _DEBUG
	objectCount++;
#endif
}

bool Object::isNull() const {
	return handle == NULL;
}

ObjectType Object::getType() const {
	if (handle)
		return handle->getType();
	return OT_Object;
}

//INLINE int Object::getTYPE() const{
//	if (handle)
//		return handle->type4test;
//	return OT_Object;
//}
void Object::updateHandle(XV::Handle* value) {
	handle = value;
}

//Object& getTest(){
//    return OBJECTNOTFOUND;
//}

Object& Object::operator[] (const Object&  keyOrIndex){
	if(OT_Map == this->getType() && OT_String == keyOrIndex.getType() ){
//		Object object(this->handle);
//		Map map=object; 
		// map["ab"], map["ab"]="12",做一个专门的handle，然后把这个map传进去，如果发现这个handle的值有变化就把这个数值付给map
//		return map.get(keyOrIndex);
//        Object& test =getTest();
//        if(&test == &OBJECTNOTFOUND)
//            printf("==");
//        else
//             printf("!=");
//        
//        
        
		
		Map map(this->handle);
		
        Object& objGet = map.get(keyOrIndex);
//        if(&objGet == &OBJECTNOTFOUND){//Not found
        if(objGet.getType() == OT_Object){//Not found 
            static List operList = List(16);//by_zouxu
            if(operList.size()>0){
                ObjArrayOper oper = operList.get(operList.size()-1);
                if(oper.getStatus() == ArrayOperType_Once_AssignOk){//this value is not assigned , need delete 
                    oper.setStatus(ArrayOperType_Deleted);
                    operList.removeIndex(operList.size()-1);
                }
            }
            
            ObjArrayOper oper = ObjArrayOper(operList, map, keyOrIndex);
            
        //    printf("objOper -1: %d\n", operList.size());
            oper.setStatus(ArrayOperType_Assign);
            int index = operList.add(oper);
            oper.setStatus(ArrayOperType_Once_AssignOk);
        //    printf("objOper 0: %d\n", operList.size());
            Object& tmp = operList.get(index);
          //  printf("%d   %d\n", index, tmp.getType());
          //  oper.handle->deRef();
            return tmp;
        }else{
            return objGet;
        }
	}
	else if(OT_List == this->getType()&& OT_Integer == keyOrIndex.getType()){
		//Object object(this->handle);
		List list(this->handle);
		return list.get(Integer(keyOrIndex)._TInt64());
	}else{
	}
	NOTFOUND(ET_NotFound_ObjectArray)
}

Object& Object::operator =(const Object& ref) { 
//	bool valid = this->getType() == OT_Object /*|| ref.getType() == OT_Object */|| this->getType() == ref.getType();
//	if (!valid) {
//		LEAVE(-800);
//	}else
    
    
    
    
    //printf("%d-%d\n",getType(),  ref.getType());
    if(OT_Object == ref.getType())
        LEAVE(ET_TypeError_ObjectAssign);
    
    
    
	
	
	
	
    if(OT_ObjArrayOper == ref.getType()){
        if (handle && --handle->refCount <= 0) {
			handle->destroy();
			delete handle, handle = NULL;
		}
        if( ObjArrayOper(ref).getStatus() == ArrayOperType_Assign){
            handle = ref.handle;
            if(handle)
                handle->refCount++;
        }
    }else if( getType() == OT_ObjArrayOper && ObjArrayOper(*this).getStatus() == ArrayOperType_Once_AssignOk){
//            ObjArrayOper(*this).setStatus(ArrayOperType_Deleted);
            ObjArrayOper(*this).setValueAndRelease(ref);
		//		NOTFOUND(0);
            return *this;
    }else
        
        
        
		
		
		
            
            
//    if(this != &ref){
    if(handle != ref.handle){
//		deRef();
//		updateHandle(ref.handle);
//		incRef();
        
        if (handle && --handle->refCount <= 0) {
			handle->destroy();
			delete handle, handle = NULL;
		}
        handle = ref.handle;
		if(handle)
        handle->refCount++;
	} 
	return *this;
}

bool Object::operator==(const Object& ref) const {
    if (handle && ref.handle) {
        return (handle == ref.handle) || ( handle->getType() == ref.handle->getType() && handle->compare(ref.handle));
	}
	return false;
    
	if (handle) {
		return handle->equals(ref.handle);
	}
	return false;
}

//bool Object::operator!=(int num) const {
//	if (num == null) {
//		return !isNull();
//	}
//	return true;
//}

bool Object::operator!=(const Object& ref) const {
	return !(*this == ref);
}

int Object::hashCode() const {
	if (handle) {
		return handle->hash();
	}
	return 0;
}

void Object::clean() {
	deRef();
	handle = NULL;
}

void Object::deRef() {
    //int rc = handle->deRef();
    if (handle && --handle->refCount <= 0) {
        handle->destroy();
        delete handle, handle = NULL;
    }
}
void Object::incRef() {
	if (handle) {
//		handle->incRef();
		handle->refCount++;
	}
}
String Object::toString() const {
	if (handle) {
		return handle->toString();
	}
	return String("err handle is NULL");
}

int Object::sizeOf()const{
    if (handle) {
		return handle->sizeOf();
	}
    return sizeof(this);
}

Bytes Object::toBytes()const{
	if (handle) {
		return handle->toBytes();
	}
	return String("err handle is NULL").toBytes();
}


	
void Object::Save(const String& aFN){
	Bytes bytes = toBytes();
	Util::ObjectSave(aFN, bytes);
}
Object Object::Load(const String& aFN){
	Object obj;
	if(Util::ObjectLoad(aFN,obj))
		return obj;
	LEAVE(ET_LoadObj_Error);
	return obj;
	 
}


#ifdef __WINS__
int Object::getRef() {
	if (handle)
		return handle->refCount;
	return 0;
}
#endif
#ifdef _DEBUG
TInt Object::objectCount = 0;
#endif
