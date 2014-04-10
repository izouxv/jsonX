/*
 * userinfo.h
 *
 *  Created on: 2011-9-20
 *      Author: Administrator
 */

#ifndef _FECADE_H_
#define _FECADE_H_
//#include <e32std.h>
//#include <e32base.h>
#include "Object.h"
//class Map;
#include "Map.h"
#include "List.h"
#include "FacadeDefine.h"
class GenPtr;
class CFacadeCallBackX;

class CFacadeX{
	friend class CFacadeCallBackX;

public:
	//CFacadeX::GetInstance()->SendNotificationT(;
	static CFacadeX* GetInstance(bool del = false);
	void SendNotificationT(int type, const Object& t, TUint delay = 0,
			const Object& t2 = Object());//delay is 1/1000000s
	virtual ~CFacadeX();
	void SetNotComsumedCallBack(CFacadeCallBackX* aCallBack);

private:
	void AddCallBack(const GenPtr& callback, int type);
	void RemoveCallBack(const GenPtr& callback, int type);// remove all type of the callback when it is -1

private:
	static TBool consumeEvent(int type, const Object& t, TUint delay,
			const Object& t2);
	static CFacadeX* NewL();
	CFacadeX();
	void ConstructL();
	void CallAllTimeOutObject();
	void InserEventToListRight(const Map& mis);
	TBool CallHandleNotification(CFacadeCallBackX* aCallBackPtr, int type, const Object& t,
			const Object& t2);

//private:
public:
	//from MTimeOutNtf
	virtual void TimerExpired();
	void After(int time);

private:
	Map callBackListAndType;// type & callBackList
	//#ifdef __WINS__
	//public:
	//#endif
	List eventList;//event list// big--->>small 
	//	RTimer iTimer;
//	CTimeOut* iTimer;
	CFacadeCallBackX* notComsumerCallBack;
};

#endif /* USERINFO_H_ */
