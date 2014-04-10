//
//  ObjBase.cpp
//  X_V
//
//  Created by zouxu on 23/7/13.
//  Copyright (c) 2013 zouxu. All rights reserved.
//

#include "ObjBase.h"
#include "Object.h"
#import "NSObject+jsonXV.h"


//ObjCBase::ObjCBase(id ID){
//    Object* obj =(Object*) this;
//    obj->deRef();
//    NSObject* nsobj = ID;
//	Object tmp =[nsobj obj];
//    obj->updateHandle(tmp.handle);
//    obj->incRef();
//}
ObjCBase::ObjCBase(){
}

id ObjCBase::ID() const{
    Object* obj =(Object*) this;
    return [NSObject fromObj:*obj];
}

ObjCBase::operator id()const{
	Object* obj =(Object*) this;
    return [NSObject fromObj:*obj];
}

ObjCBase::operator const Object&()const{
    return *this;
}

Object ObjCBase::operator=(id ref){
	NSObject* nsobj = ref;
	Object obj =[nsobj obj];
	return obj;
}

//
//XV::Handle* ObjCBase::operator=(id ref){
//	NSObject* nsobj = ref;
//	Object obj =[nsobj toObj];
//	return obj.handle;
//}




