/*
 * userinfo.h
 *
 *  Created on: 2011-9-20
 *      Author: Administrator
 */

#ifndef _FECADECALLBACK_H_
#define _FECADECALLBACK_H_
//#include <e32std.h>
//#include <e32base.h>
#include "Object.h" 
#include "List.h"
#include "GenPtr.h"
class CFacadeCallBackX: public GenPtrBase {
public:
	virtual ~CFacadeCallBackX();//destroyed will rempve all types of this callback
	void AddCallBack(int type);
	void RemoveCallBack(int type);
	virtual TBool HandleNotification(int type, const Object& t,
			const Object& t2)=0;//must override
	int THIS();
private:
	List typeList;
};

#endif /* USERINFO_H_ */
