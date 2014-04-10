/*
 * uesrinfo.cpp
 *
 *  Created on: 2011-9-20
 *      Author: Administrator
 */

#include "facadex.h"
#include "Map.h"
#include "List.h"
#include "String.h"
#include "facadeCallBackX.h"
#include "GenPtr.h"
//#include "defineUtil.h"
#include "GenPtr.h"
#include "UtilPlatform.h"
#include "Integer.h"
//#include "FileProxyX.h"
#include "Logger.h"

int CFacadeCallBackX::THIS() {
    int64_t add =(int64_t) this;
    return add;
//	return reinterpret_cast<int> (this);
}
CFacadeCallBackX::~CFacadeCallBackX() {
	if (typeList.size() > 0) {
		//		LOG_LC(_L("%S"),String::valueOf(int(this)).des());
		//		LOG_LC(_L("%S"),String::valueOf(typeList.size()).des());
		//		LOG_LC(_L("%S"),CFacadeX::GetInstance()->callBackListAndType.toString().des() );
		for (int i = typeList.size() - 1; i >= 0; i--) {
			//			String typeString = typeList.get(i);
			//			LOG_LC(_L("@@@@@---KEY----%S"),typeString.des() );
			//			LOG_LC(_L("%S"),String::valueOf(typeList.size()).des());
			//			LOG_LC(_L("%S"),String::valueOf(i).des());
			RemoveCallBack(Integer(typeList.get(i)));
		}
		typeList.reset();
		//		LOG_LC(_L("==============================\n\n%S"),CFacadeX::GetInstance()->callBackListAndType.toString().des() );
	}
}
void CFacadeCallBackX::AddCallBack(int type) {
	typeList.add(Integer(type));
	CFacadeX::GetInstance()->AddCallBack(genPtr, type);
}
void CFacadeCallBackX::RemoveCallBack(int type) {
	typeList.removeIndex(Integer(type));
	CFacadeX::GetInstance()->RemoveCallBack(genPtr, type);
}
CFacadeX* CFacadeX::GetInstance(bool del) {
	static CFacadeX* instance = NULL;
	if (del) {
		SAFE_DEL(instance);
		return instance;
	}
	if (!instance)
		instance = CFacadeX::NewL();
	return instance;
}
CFacadeX* CFacadeX::NewL() {
	CFacadeX* self = new  CFacadeX();
	//CleanupStack::PushL(self);
	self->ConstructL();
	//CleanupStack::Pop(); // self;
	return self;
}
CFacadeX::CFacadeX() :
	callBackListAndType(MapSizeNormal) {
	//	CActiveScheduler::Add(this); // Add to scheduler
}
void CFacadeX::ConstructL() {
	//iTimer = CTimeOut::NewL(0, *this);
	//	User::LeaveIfError(iTimer.CreateLocal()); // Initialize timer
	notComsumerCallBack = NULL;
}
CFacadeX::~CFacadeX() {
	//SAFE_DEL(iTimer);
	//	Cancel();
	//	iTimer.Close();
}

void CFacadeX::SetNotComsumedCallBack(CFacadeCallBackX* aCallBack) {
	notComsumerCallBack = aCallBack;
}

TBool CFacadeX::consumeEvent(int type, const Object& t, TUint delay,
		const Object& t2) {
	if (E_Facade_CMD == type) {
		String value = t;
		List cmdList = value.Split(':');
		if (cmdList.size() > 1 && String("cmd") == String(cmdList.get(0))) {
			String cmdtype = cmdList.get(1);
            if (String("cmd") == cmdtype) {
				CFacadeX::GetInstance()->SendNotificationT(
						E_Facade_CMD, String(""), 0, t2);
				return EFalse;
			}
		}
	}
	return EFalse;
}

void CFacadeX::SendNotificationT(int type, const Object& t, TUint delay,
		const Object& notUsed) {

	if (CFacadeX::consumeEvent(type, t, delay, notUsed))
		return;

//	TTime curTime;
//	curTime.HomeTime();
//	TTime doMissionTime(curTime.Int64() + delay * 1000);
    
    int64_t doMissionTimeInt64 =curTimeGen()+ delay;//=doMissionTime.Int64();

	Map event(MapSizeMin);
	event.put(String("type"), Integer(type));
	event.put(String("object"), t);
	event.put(String("object2"), notUsed);
	event.put(String("calltime"), Integer(doMissionTimeInt64));

	InserEventToListRight(event);
	After(0);
}

void CFacadeX::InserEventToListRight(const Map& mis) {

	if (eventList.size() == 0) {
		eventList.add(mis);
	} else {
		TInt64 misTime = Integer(mis.get(String("calltime")));
		for (int i = eventList.size() - 1; i >= 0; i--) {
			Map event = (Map) eventList.get(i);
			//			Integer type = event.get(String("type"));
			//			Object object(event.get(String("object")));
			//			Object object2(event.get(String("object-2")));
			//			String calltime = event.get(String("calltime"));
			TInt64 doMissionTime = Integer(event.get(String("calltime")));
			//			eventList.add(mis);
            
           // INFO<<"time:  doT:"<<doMissionTime<<"  misT:"<<misTime;
			if (misTime < doMissionTime) {
                if(eventList.size()==i+1)
                    eventList.add(mis);
                else
                    eventList.insert(mis, i + 1);
				break;
			} else if (0 == i) {
				eventList.insert(mis, 0);
				break;
			}
		}
	}
  //  INFO<<"map: \r\n"<<mis;
   // INFO<<"events: \r\n"<<eventList;
}

TBool CFacadeX::CallHandleNotification(CFacadeCallBackX* aCallBackPtr,
		int type, const Object& t, const Object& t2) {
	
    int64_t timeStart = curTimeGen();

	TBool res = aCallBackPtr->HandleNotification(type, t, t2);

//	TTime timeEnd;
//	timeEnd.HomeTime();
    int64_t timeEnd = curTimeGen();

	TInt64 runTime = timeEnd - timeStart;
	runTime /= 1000000;

	if (runTime >= 2) {
		Map map;
		map.put(String("type"), Integer(type));
		map.put(String("t1"), t);
		map.put(String("t2"), t2);
		INFO << "Call HandleNotification timeOut: "
					<< Integer(runTime).toString() << "s" << "\r\n"
					<< map.toString() << "\r\n\r\n";
	}
	return res;
}

void CFacadeX::CallAllTimeOutObject() {
	//TInt64 curTime= curTimeGen();;
	TInt64 doMissionTime = 0;
	TInt64 cur = curTimeGen();;
	//	for (int i = eventList.size() - 1; i >= 0; i--)
	while (eventList.size() > 0) {
		Map event = eventList.get(eventList.size() - 1);
		Integer type = event.get(String("type"));
	//	cur = curTimeGen();
		doMissionTime = Integer(event.get(String("calltime")));// calltime.toInt();
		if (doMissionTime <= cur) {
			eventList.removeObj(event);
			// DEBUG << "Event Del\r\n" << event.toString() << "\r\n"<< eventList.toString();

			TBool consumed = EFalse;
			Object object(event.get(String("object")));
			Object object2(event.get(String("object2")));
			Integer calltime = event.get(String("calltime"));

			if (callBackListAndType.contains(type.toString())) {
				//DEBUG<<event.toString();
				List callBackList = callBackListAndType.get(type.toString());
				for (int j = callBackList.size() - 1; j >= 0; j--) {
					GenPtr genPtr = (GenPtr) callBackList.get(j);
					GenPtrBase* callBackPtr = genPtr.getPtr();
					CFacadeCallBackX* callBack =
							(CFacadeCallBackX*) callBackPtr;
					if (CallHandleNotification(callBack, type, object, object2)) {
						consumed = ETrue;
						break;
					}
				}
			}
			if (!consumed && notComsumerCallBack)
				CallHandleNotification(notComsumerCallBack, type, object,
						object2);

		} else {
			break;
		}
	}
	if (eventList.size() > 0) {
		//curTime.HomeTime();
		//#ifdef __WINS__
		//		CFileProxy::writeAddL(
		//				String("c:\\data\\timeTmp.txt"),
		//				String(String::valueOf2(curTime.Int64() - cur) + String("\r\n")).getBytes());
		//#endif
		//cur = curTime.Int64();
		//DEBUG << eventList.toString();
		Map event = eventList.get(eventList.size() - 1);
		doMissionTime = Integer(event.get(String("calltime")));// calltime.toInt();
		//DEBUG<< (String("After: ")+Integer(doMissionTime - cur).toString());

		After(doMissionTime - cur);
	}

	//	if (eventList.size() > 0)
	//		After((doMissionTime - cur));
}

void CFacadeX::AddCallBack(const GenPtr& callback, int type) {
	//CFacadeCallBackX* callback, int type
	//	LOG_OUT(_L("\r\n\r\n1   --  %d"), 1);
	//	LOG_OUT(_L("\r\n\r\nsin%S"), &callBackListAndType.toString().des());
	if (callBackListAndType.contains(Integer(type).toString())) {
		List callBackList = callBackListAndType.get(Integer(type).toString());
		for (int i = callBackList.size() - 1; i >= 0; i--) {
			GenPtr genPtr = (GenPtr) callBackList.get(i);
			if (genPtr.getPtr() == callback.getPtr()) {
				return;
			}
		}
		//		callBackList.insert(callback, 0);
		callBackList.add(callback);
	} else {
		List callBackList;
		callBackList.add(callback);
		callBackListAndType.put(Integer(type).toString(), callBackList);
	}
	//	LOG_OUT(_L("\r\n\r\n%S"), &callBackListAndType.toString().des());
}
void CFacadeX::RemoveCallBack(const GenPtr& callback, int type) {

	List callBackList = callBackListAndType.get(Integer(type).toString());
	for (int i = callBackList.size() - 1; i >= 0; i--) {
		GenPtr genPtr = (GenPtr) callBackList.get(i);
		if (callback.getPtr() == genPtr.getPtr()) {
			callBackList.removeObj(genPtr);
		}
		//		GenPtr genPtr = (GenPtr) callBackList.get(i);
		//		String callType = callback->getCallBackType();
		//		if (callType == genPtr.getType()) {
		//			callBackList.remove(callBackList.get(i));
		//		}
	}
}
//typedef  void (CFacadeX::*FunPr)();// (*FunP)(20);
void CFacadeX::After(int time) {
  //  DEBUG<< (String("After1: ")+Integer(time).toString());

    timerAfter(this, time);
    return;
   //  void (*FunPtr)();
   // timerAfter(FunPtr, time);
}
void CFacadeX::TimerExpired() {
	CallAllTimeOutObject();
}
