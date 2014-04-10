//
//  ObjBase.h
//  X_V
//
//  Created by zouxu on 23/7/13.
//  Copyright (c) 2013 zouxu. All rights reserved.
//

#import <objc/objc.h>
//class Object;
//namespace XV {
//    class Handle;
//}
class Object;
class ObjCBase {
protected://test section
    ObjCBase();
//    ObjCBase(id ID);
	operator const Object&()const;
	Object operator=(id ref);
public:
    id ID() const;
	operator id()const;
};