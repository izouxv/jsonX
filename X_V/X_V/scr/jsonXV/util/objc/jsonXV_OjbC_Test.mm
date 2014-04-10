//
//  jsonXV_OjbC_Test.cpp
//  X_V
//
//  Created by zouxu on 23/7/13.
//  Copyright (c) 2013 zouxu. All rights reserved.
//
#import "NSObject+jsonXV.h"
#import "jsonXV_OjbC_Test.h"
#import "jsonXV.h"
#import "UtilPlatform.h"
#include "facadex.h"
#import "Ptr.h"

#define CMP  compList(list, listDyn);
void compList(const List& list, const List& listDyn){
   return;
    if(list != listDyn){
        WARN<<list;
        WARN<<listDyn;
        WARN<<listDyn.testInfo();
        assert(0);
    }
}

void initList(List& list, List& listDyn){
    //add section
    list.reset();
    listDyn.reset();
    for (int i=0; i<1000; i++) {
        list.add(i+1000000);
        listDyn.add(i+1000000);
        compList(list, listDyn);
    }
}
@interface NSMutableArray(DEL)
-(void)removeOneObject:(NSObject*)item;

@end
@implementation NSMutableArray(DEL)
-(void)removeOneObject:(NSObject*)item;{
    for(int i=0; i<self.count; i++){
        if([[self objectAtIndex:i] isEqual:item]){
            [self removeObjectAtIndex:i];
            return;
        }
    }
}
@end


@implementation JsonXV_OjbC_Test

static const char* KSep = "                             ";

#pragma mark - List test Section


const int KLoopInner = 3;
-(void)test_error_list_remove_add_insert_count:(int)count test:(List)listTestSample testDyna:(List)listTestSampleDyna _listDel:(List)_listDel _listInsert:(List)_listInsert _listAdd:(List)_listAdd _listSize:(int)_listSize dyna:(BOOL)dyna{
	//return;
	bool compareEveryStep = false;
    for(int i=0; i<_listSize; i++){
        listTestSample.add(i);
        listTestSampleDyna.add(i);
		if(compareEveryStep && listTestSample!=listTestSampleDyna){
			DEBUG<<"list Compare Error 1   "<<_listSize;
			DEBUG<<listTestSample;
			DEBUG<<listTestSampleDyna;
		}
	}
	for(int k=0; k<KLoopInner; k++)
    {
        if(k == KLoopInner-2){
            listTestSampleDyna.reset();
            listTestSample.reset();
        }
		for(int i=0; i<_listDel.size(); i++){
			listTestSample.removeObj(_listDel.get(i));
			listTestSampleDyna.removeObj(_listDel.get(i));
			if(compareEveryStep && listTestSample!=listTestSampleDyna){
				DEBUG<<"list Compare Error 2";
				DEBUG<<listTestSample;
				DEBUG<<listTestSampleDyna;
			}
		}
		for(int i=0; i<_listInsert.size(); i++){
			
		//	DEBUG<<listTestSampleDyna.testInfo();
			listTestSample.insert(_listInsert.get(i), Integer(_listInsert.get(i))._TInt64());
			listTestSampleDyna.insert(_listInsert.get(i), Integer(_listInsert.get(i))._TInt64());
			if(compareEveryStep && listTestSample!=listTestSampleDyna){
                
                List temp = listTestSample.toString().toObject();
				DEBUG<<"list Compare Error 3";
				DEBUG<<temp;
				DEBUG<<_listInsert.get(i);
				DEBUG<<listTestSample;
				DEBUG<<listTestSampleDyna;
				DEBUG<<listTestSampleDyna.testInfo();
			}
		}
      //  return;
		for(int i=0; i<_listAdd.size(); i++){
			listTestSample.add(_listAdd.get(i));
			listTestSampleDyna.add(_listAdd.get(i));
			if(compareEveryStep && listTestSample!=listTestSampleDyna){
				DEBUG<<"list Compare Error 4";
				DEBUG<<listTestSample;
				DEBUG<<listTestSampleDyna;
			}
		}
	}
}



-(void)test_error_list_remove_add_insert_count:(int)count testNS:(NSMutableArray*)listTestSample  testDyna:(List)listTestSampleDyna  _listDel:(List)_listDel _listInsert:(List)_listInsert _listAdd:(List)_listAdd _listSize:(int)_listSize dyna:(BOOL)dyna{
    for(int i=0; i<_listSize; i++){
        [listTestSample addObject:Integer(i).ID()];
        listTestSampleDyna.add(i);
	}

	for(int k=0; k<KLoopInner; k++)
    {
        if(k == KLoopInner-2){
            listTestSampleDyna.reset();
            [listTestSample removeAllObjects];
        }
        for(int i=0; i<_listDel.size(); i++){
            [listTestSample removeOneObject:_listDel.get(i).ID()]; 
			listTestSampleDyna.removeObj(_listDel.get(i));
		}
        //return;
        for(int i=0; i<_listInsert.size(); i++){
            if(Integer(_listInsert.get(i))._TInt64()<listTestSample.count)
            [listTestSample insertObject:_listInsert.get(i).ID()  atIndex:Integer(_listInsert.get(i))._TInt64()];
			
			listTestSampleDyna.insert(_listInsert.get(i), Integer(_listInsert.get(i))._TInt64());
        }
        for(int i=0; i<_listAdd.size(); i++){
            [listTestSample addObject:_listAdd.get(i).ID()];
			
			listTestSampleDyna.add(_listAdd.get(i));
		}
	}
}

-(List) randomList:(int)range{
	int num=random()%range+1;
	List list(num+1);
    for(int i=0; i<num; i++){
        list.add(int(random()%range));
    }
	return list;
}
-(void)test_error_list_remove_add_insert {
	PERFOR<<__FUNCTION__;
	
	
	//PERFOR<<__FUNCTION__;
  	for(int i=0; i<500; i++){
		//printf("dyna i: %d \n", i);
		int KTestRange=i+1;
		int delCase=random()%KTestRange+1;
		
		List _listDel = [self randomList:KTestRange];
		List _listInsert= [self randomList:KTestRange];
		List _listAdd= [self randomList:KTestRange];
		
		List testList(8);;
		List testListDyna ;
		[self test_error_list_remove_add_insert_count:delCase test:testList testDyna:testListDyna  _listDel:_listDel _listInsert:_listInsert _listAdd:_listAdd _listSize:i dyna:NO];
		if(testList!=testListDyna){
			testList.Save("testList1");
			testListDyna.Save("testListDyna");
			DEBUG<<testListDyna.testInfo();
			DEBUG<<testList;
			DEBUG<<testListDyna;
			DEBUG<<"list 2 remove add insert ERROR i: "<<i;
			DEBUG<<"list 2 remove add insert ERROR";
			for (int o=0; 0<testListDyna.size(); o++) {
				testListDyna.get(o);
			}
		}
    }
	
	
	
    for(int i=0; i<10; i++){
    int KTestRange=100;
    int delCase=random()%KTestRange+1;
		
	List _listDel = [self randomList:KTestRange];
    List _listInsert= [self randomList:KTestRange];  
    List _listAdd= [self randomList:KTestRange];
    
    List testList(8);;
    List testListDyna ;
		[self test_error_list_remove_add_insert_count:delCase test:testList testDyna:testListDyna _listDel:_listDel _listInsert:_listInsert _listAdd:_listAdd _listSize:K_Loop_Small dyna:NO];
		if(testList!=testListDyna){
			testList.Save("testList1");
			testListDyna.Save("testListDyna");
		//	DEBUG<<"list remove add insert ERROR";
			DEBUG<<"list 1 remove add insert ERROR";
		}
    }
	
	
	
	
}
-(void)test_error_list_insert_get:(BOOL)dynamic{
	//by_zouxu not easy to test
	List list;
	Map map;
	map.put(String("key"), String("value"));
	Map map2 = map.toString().toObject();
	PERFOR<<__FUNCTION__;
	for(int i=0;i<K_Loop_Huge/10; i++){
		list.insert(String("abcdefg"), 0);
		list.insert(String("ABC"), 100);
		list.insert("ABC", 3);
		list.insert(1, 2);
		list.insert(Integer(200), 4);
	}
}

-(void)test_error_list_add_get:(BOOL)dynamic{
	List list;
	PERFOR<<__FUNCTION__;
	for(int i=0;i<K_Loop_Small; i++){ 
		switch (random()%2) {
			case 0:{
				list.add(i);
				if(list.get(i)!=i){
					WARN<<__FUNCTION__<<" 1error";
				}
				break;
			}
			case 1:{
				list.add(Integer(i).toString());
				if(list.get(i)!=Integer(i).toString()){
					WARN<<__FUNCTION__<<" 2error";
				}
				break;
			}
			default:
				WARN<<__FUNCTION__<<" 3error";
				break;
		}
	}
}

-(void)test_error_list_set_get:(BOOL)dynamic{
	List list;
	list.add(111);
	PERFOR<<__FUNCTION__;
	for(int i=0;i<K_Loop_Huge; i++){
		list.set(i, 0);
		if(list.get(0)!=i){
			WARN<<__FUNCTION__<<" error";
		}
	}
}

-(void)test_error_list_removeObj:(BOOL)dynamic{
	PERFOR<<__FUNCTION__;
	for(int i=0;i<K_Loop_Small; i++){
		List list;
		list.add(1);
		list.add(2);
		list.removeObj(1);
		if(list.size()!=1){
			WARN<<__FUNCTION__<<" 1error";
		}
		list.removeObj(3);
		if(list.size()!=1){
			WARN<<__FUNCTION__<<" 2error";
		}
	}
}

-(void)test_error_list_Array:(BOOL)dynamic{
	PERFOR<<__FUNCTION__;
	List list;
	list.add(0);
	list.add(1);
	list.add(2);
	list.add("3");
	list.add(4); 
	for(int i=0;i<K_Loop_Huge; i++){
		Object obj = list[0];
		if(obj!=Integer(0)){
			WARN<<__FUNCTION__<<" 1error";
		}
		list[1]=i;
		if(list[1]!=Integer(i)){
			WARN<<__FUNCTION__<<" 2error";
		}
		Object obj2 = list[100];
		if(obj2.getType()!= OT_Object){
			WARN<<__FUNCTION__<<" 3error";
		}
		
		Object obj4 = list[-100];
		if(obj4.getType()!= OT_Object){
			WARN<<__FUNCTION__<<" 4error";
		}
	}
}

-(void)test_error_list_indexOfObject:(BOOL)dynamic{
	PERFOR<<__FUNCTION__;
	List list;
	list.add(0);
	list.add(1);
	list.add(2);
	list.add("3");
	list.add(4);
	for(int i=0;i<K_Loop_Huge/10; i++){
		int index = list.indexOfObject(2);
		if(index!=2){
			WARN<<__FUNCTION__<<" 1 error";
		}
       index =  list.indexOfObject("3");
		if(index!=3){
			WARN<<__FUNCTION__<<" 2 error";
		}
       index =  list.indexOfObject(-1);
		if(index>-1){
			WARN<<__FUNCTION__<<" 3 error";
		}
	}
}

-(void)test_error_list_excludeDuplicates:(BOOL)dynamic{
	return;
	PERFOR<<__FUNCTION__; 
	List list;
	list.add(0);
	list.add(1);
	list.add(2);
	for(int i=0;i<K_Loop_Huge/10; i++){
		list.add(1); 
		List tmp = list.excludeDuplicates();
		if(tmp.size()!=3){
			WARN<<__FUNCTION__<<" error";
		}
		list.removeIndex(3);
	}
}

-(void)test_error_list_remove:(BOOL)dynamic{
	PERFOR<<__FUNCTION__;
	
	List list;
	list.add(0);
	list.add(1);
	list.add(2);
	for(int i=0;i<K_Loop_Huge; i++){
		
		list.add(1);
		list.removeIndex(2);
		if(list.size()!=3){
			WARN<<__FUNCTION__<<"1 error";
		}
		list.removeIndex(100);
		if(list.size()!=3){
			WARN<<__FUNCTION__<<"2 error";
		}
	}
}

-(void)test_error_list_contains:(BOOL)dynamic{
	PERFOR<<__FUNCTION__;
	
	List list;
	list.add(0);
	list.add(1);
	list.add(2);
	list.add("abc");
	for(int i=0;i<K_Loop_Huge; i++){
		
		if(!list.contains(1) || !list.contains("abc")){
			WARN<<__FUNCTION__<<"1 error";
		}
		if(list.contains(100)){
			WARN<<__FUNCTION__<<"2 error";
		}
	}
}

-(void)test_error_list_ADD:(BOOL)dynamic{
	PERFOR<<__FUNCTION__;
	
	for(int i=0;i<K_Loop_Small; i++){
	
		List list ;
		list.add(0);
		list.add("abc");
		
		List list2(8);;
		list2.add(0);
		list2.add(1);
		list2.add("abc");
		
		List list3(8);;
		list3+=list+list2;
		
		if(list3.size()!=5){ 
			WARN<<__FUNCTION__<<" error";
		}
	}
}

-(void)test_error_list_link{
	PERFOR<<__FUNCTION__;
	
	for(int i=0;i<K_Loop_Small; i++){
		
		List list;
		list.add(0);
		list.add("abc");
		
		List list2;
		list2.add(0);
		list2.add(1);
		list2.add("abc");
		
		List list3 = List::link(list, list2);
		
		if(list3.size()!=5){
			WARN<<__FUNCTION__<<" error";
		}
	}
}


-(void)test_error_list_Mess{
	PERFOR<<__FUNCTION__;
	for(int i=0;i<K_Loop_Small; i++){
	}
}


-(void)test_facade{
    //double curTime()
    NSLog(@"curTime %f", curTime());
    NSLog(@"curTime %lld", curTimeGen());
    NSLog(@"curTime %lld", curTimeGen());
    NSLog(@"curTime %lld", curTimeGen());
    NSLog(@"curTime %lld", curTimeGen());
    NSLog(@"curTime %lld", curTimeGen());
    
//    CFacadeX::GetInstance();
//    CFacadeX::GetInstance()->SendNotificationT(0,0,0000000,0);//delay is 1/1000000s
    //    CFacadeX::GetInstance()->SendNotificationT(0,0,0000000,0);//delay is 1/1000000s
//    CFacadeX::GetInstance()->SendNotificationT(0,0,0000000,0);//delay is 1/1000000s
//    CFacadeX::GetInstance()->SendNotificationT(0,3,3000000,0);//delay is 1/1000000s
//    return;
//    CFacadeX::GetInstance()->SendNotificationT(0,3,3000000,0);//delay is 1/1000000s
//    CFacadeX::GetInstance()->SendNotificationT(0,0,0000000,0);//delay is 1/1000000s
//    return;
    CFacadeX::GetInstance()->SendNotificationT(0,3,3000000,0);//delay is 1/1000000s
    CFacadeX::GetInstance()->SendNotificationT(0,8,8000000,0);//delay is 1/1000000s
    CFacadeX::GetInstance()->SendNotificationT(0,0,0000001,0);//delay is 1/1000000s
    CFacadeX::GetInstance()->SendNotificationT(0,10,10000000,0);//delay is 1/1000000s
    
}

-(void)test_str_list{
    int KtestTime = 0;
    List list(8);;
    List listDyn ;
    
    listDyn.indexOfObject(0);
    listDyn.insert(0, 0);
    listDyn.set(0, 0);
    listDyn.get(0);
    
    initList(list, listDyn);
    while (++KtestTime<100) {
       //++KtestTime ;
       PERFOR<<" time: "<<KtestTime;
        //if(++KtestTime % 10 == 0){
        //    DEBUG<<"time: "<<KtestTime<<"  size: "<<list.size();
        //}
        
        //add
        int rand = int(random()%2000)+1;
        for(int i=0; i<rand; i++){
            list.add(i+rand);
            listDyn.add(i+rand);
            CMP
            
            int randArray = int(random()%(i+1));
            list[randArray]=randArray;
            listDyn[randArray]=randArray;
            CMP
            
            int randInsert = int(random()%(i+1));
            list[randInsert]=randInsert;
            listDyn[randInsert]=randInsert;
            CMP
        }
        
        int randLoop = int(random()%1000);
        
        log<<"\t  add: "<<list.size()<<"\t    loop: "<<randLoop<<"\t";
        
        while(--randLoop>0){
            
            //--->----->
            int rand = int(random()%list.size());
            Object obj =list.get(rand);
            Object objDyn =listDyn.get(rand);
            CMP
            assert(obj == objDyn);
            CMP
            assert(list[rand] == listDyn[rand]);
            CMP
            //set
            list.set(rand*rand, rand);
            listDyn.set(rand*rand, rand);
            CMP
            //add
            assert(list.add(rand*rand*rand) == listDyn.add(rand*rand*rand));
            //insert
            list.insert(rand+1, rand);
            listDyn.insert(rand+1, rand);
            CMP
            //removeObj
            assert(list.removeObj(obj)==listDyn.removeObj(objDyn));
            CMP
            //[]
            assert(list[rand] == listDyn[rand]);
            CMP
            //indexOfObject
            assert(list.indexOfObject(obj) == listDyn.indexOfObject(objDyn));
            CMP
            //size
            assert(list.size() == listDyn.size());
            CMP
            //remove
            assert(list.removeIndex(rand) == listDyn.removeIndex(rand));
            CMP
            //contains
            assert(list.contains(obj) == listDyn.contains(objDyn));
            CMP
            //mid
            assert(list.mid(rand/2,rand/2) == listDyn.mid(rand/2,rand/2));
            CMP
        }
        
        log<<" del: "<<list.size();
        
        while (list.size()>0) {
            int rand = int(random()%list.size());
            list[rand]=rand;
            listDyn[rand]=rand;
            CMP
            assert(list.removeIndex(rand) == listDyn.removeIndex(rand));
            CMP
            
            if(list.size()>0){
            int rand2 = int(random()%list.size());
            Object objT = list[rand2];
            Object objTDyn =listDyn[rand2];
            CMP
            
            assert(list.removeObj(objT) == listDyn.removeObj(objTDyn));
            CMP
            }
        }
        
    }
    return;
    assert(0);
    
    
    initList(list, listDyn);
    while (list.size()>0) {
        list.removeIndex(list.size()-1);
        listDyn.removeIndex(listDyn.size()-1);
        if(list != listDyn){
            WARN<<list;
            WARN<<listDyn;
            WARN<<listDyn.testInfo();
            assert(0);
        }
    }
    
    initList(list, listDyn);
    while (list.size()>0) {
        list.removeIndex(0);
        listDyn.removeIndex(0);
        if(list != listDyn){
            WARN<<list;
            WARN<<listDyn;
            WARN<<listDyn.testInfo();
            assert(0);
        }
    }
    
    
    assert(0);
    //add section
    for (int i=0; i<30; i++) {
        list.add(i);
        listDyn.add(i);
        
        WARN<<list;
        WARN<<listDyn;
        WARN<<listDyn.testInfo();
    }
    
    
	PERFOR<<__FUNCTION__;
    assert(0);
}

#pragma mark - Map test Section

-(void)test_error_map_mess{
	PERFOR<<__FUNCTION__;
    for(int i=0;i<K_Loop_Small; i++){
        Map map; 
        map.put("key", "value");
        map.put("key1", String("value"));
        map.put("key2", 1);
        map.put("key3", 0.5);
        map.put("key4", Integer(6));
        
        if(map.getKeys().size()!=5)
            WARN<<__FUNCTION__<<" 1error";
        
        if(!map.contains("key4"))
            WARN<<__FUNCTION__<<" 2error";
        if(map.contains("123123"))
            WARN<<__FUNCTION__<<" 3error";
        map.remove("key4");
        if(map.contains("key4"))
            WARN<<__FUNCTION__<<" 4error";
        
        if(map.get("key")!="value")
            WARN<<__FUNCTION__<<" 5error";
        
        map["key"]="value123";
        if(map["key"]!="value123")
            WARN<<__FUNCTION__<<" 6error";
        
        Object obj =  map["key"];
        obj = "124"; 
        if(map["key"]!="value123")
            WARN<<__FUNCTION__<<" 7error";
        
        if(map.get("key2")!=1)
            WARN<<__FUNCTION__<<" 8error";
        if(map["key2"]!=Integer(1))
            WARN<<__FUNCTION__<<" 9error"; 
        
	}
}

-(void)test_error_map_Array{
	PERFOR<<__FUNCTION__;
    
	Map map;
    for(int i=0;i<K_Loop_Small/10; i++){
		map["A"]=1;
		if(map.size() !=1)
			WARN<<__FUNCTION__<<" 1error";
		
		map["B"];
		if(map.size() !=1)
			WARN<<__FUNCTION__<<" 2error";
		
		Object obj = map["C"];
		obj = "3";
		if(map.size() !=1)
			WARN<<__FUNCTION__<<" 3error";
		
		if(map["A"] != Integer(1))
			WARN<<__FUNCTION__<<" 4error";
		
		map["C"]="abcd";
		
		if(map.get("C") != "abcd")
			WARN<<__FUNCTION__<<" 5error";
			map.reset();
    }
	
    for(int i=0;i<K_Loop_Small/100; i++){
		Map map;
		map["a"]=1;
		map["b"]=2;
		map["c"]=3;
		map[String("d")]=4;
		map[String("e")]=5;
		
		for(int j=0;j<100; j++){
		if(map.size() !=5)
			WARN<<__FUNCTION__<<" 6error";
		
			Object obj = map["f"];
			obj = "123";
			if(map.size() !=5)
				WARN<<__FUNCTION__<<" 7error";
			
			
			map.get("g")="999";
			if(map.size() !=5)
				WARN<<__FUNCTION__<<" 8error";
			
			Object obj44 = map.get("g");
			if(obj44.getType()!= OT_Object)
				WARN<<__FUNCTION__<<" 9error";
			}
	}
	
}

-(void)test_error_map_Array_VS_Put{
	DEBUG<<__FUNCTION__;
	{
		PERFOR<<KSep<<"Map Array";
		Map map;
		for(int i=0;i<K_Loop_Small; i++){
			map["a"]=1;
			map["a"]=2;
			map["b"]=3;
		}
	}
	{
		PERFOR<<KSep<<"Map Put";
		Map map;
		for(int i=0;i<K_Loop_Small; i++){
			map.put("a", 1);
			map.put("a", 2);
			map.put("b", 3); 
		}
	}
}


#pragma mark - Integer test Section

-(void)test_error_Integer{
    
    PERFOR<<__FUNCTION__<< "  int2str double has problem";
    for(int i=0;i<K_Loop_Small; i++){
        Integer num;
        num = 0.111;
        if( num._double()!=0.111)
            WARN<<__FUNCTION__<<" 1error";

        num = 100;
        num++;
        if(101!=num._TInt64())
            WARN<<__FUNCTION__<<" 2error";
        num--;
        if(100!=num._TInt64())
            WARN<<__FUNCTION__<<" 3error";
        
        Integer num2;
        num2 = 200;
        num2+=num;
        
        if(300!=num2._TInt64())
            WARN<<__FUNCTION__<<" 4error";
        
        int numTmp = num2;
        
        String num2Str = num2;
        
        if(num2Str!="300")
            WARN<<__FUNCTION__<<" 55error";
        Integer num2StrNum = num2Str;
        
        if(num2StrNum._TInt64()!=300)
            WARN<<__FUNCTION__<<" 555error";
        
        if(numTmp!=300)
            WARN<<__FUNCTION__<<" 5error";
        
        num2 = 0.55;
        double douTmp = num2; 
        if(douTmp!=0.55)
            WARN<<__FUNCTION__<<" 6error";
        
         
        if(num2._double() != 0.55)
            WARN<<__FUNCTION__<<" 666error";
        
        String numStr = num2.toString();
        
  
        if(numStr != "0.55"){
            //the string is 0.550000
            //by_zouxu
           // NSLog(@"%@  %@", numStr.ID(), num2.ID());
          //  WARN<<__FUNCTION__<<" 7error";
        }
        
        
        
        Integer numNum = numStr;
        double fuck = numNum;
       // printf("%f\n", fuck);
        
        
        if( ! DOUBLE_EQUAL(fuck, 0.55))
            WARN<<__FUNCTION__<<" 8error";
        if( ! DOUBLE_EQUAL(numNum._double(), 0.55))
            WARN<<__FUNCTION__<<" 9error";
    }
}



#pragma mark - OjbectType test Section

- (void)test_error_TypeAssign
{
	
	//Integer and String can Conversion (make sure it's number), other is not allow, you can use toObject(), or toString() to convert each other
	Integer num = 1;
	String strNum = num;
	DEBUG<<"num->str: "<<strNum;
	Integer num2 = strNum;
	DEBUG<<"str->num: "<<num2;
	
	
	String str = "str";
	Bytes strBytes = str.toBytes();
	String str2 = strBytes.toString();
	DEBUG<<"str->bytes->str: "<<str2;
	
	List list;
	list = String("[\"a\"]").toObject();
	DEBUG<<"list: "<<list;
	DEBUG<<"listStr: "<<list.toString();
	
	Map map;
	map = String("{\"a\":1}").toObject();
	DEBUG<<"map: "<<map;
	DEBUG<<"mapStr: "<<map.toString();
	
	
	
}

#pragma mark - OjbectObjC test Section

- (void)test_error_OjbectObjC
{
	@autoreleasepool {
    PERFOR<<__FUNCTION__;
    for (int i = 0; i < K_Loop_Small; i++)
    {
        Integer num(2);
        String str("123");
        List list;
        list.add(str);
        list.add(num);
        Map map;
        map.put("list", list);
		
        
        
        NSObject *numID = num;
        NSObject *strID = str;
        NSObject *listID = list;
        NSObject *mapID = map;
		map.get("qwrawe")=12345;
		map.get("safasfd")=Integer(8888888);
		Integer numsfd= map.get("asfdasdfasfd");
		numsfd=Integer(77777);
		OBJECTNOTFOUND = "abc";
        NSObject *notFound = map.get("LIST111");
		
        if (![numID isKindOfClass:[NSNumber class]] || [((NSNumber*)numID) intValue] != 2 ) NSLog(@"obj2ios NSNumber Error");
        if (![strID isKindOfClass:[NSString class]] || ![((NSString*)strID) isEqual:@"123"] ) NSLog(@"obj2ios NSString Error");
        if (![listID isKindOfClass:[NSArray class]]) NSLog(@"obj2ios NSArray Error");
        if (![mapID isKindOfClass:[NSDictionary class]]) NSLog(@"obj2ios NSDictionary Error");
        if (notFound != nil) {
			//Object::DEBUG_PRINT = 1;
			NSLog(@"%d  obj2ios Nil Error: %@ \n%@ \n%@",i, notFound, map.toString().ID(), map.get(String("LIST111")).ID());
			Object obj = map.get("LIST111");
			Object obj2 = OBJECTNOTFOUND;
			NSLog(@"obj2ios %d \n%@ ",obj.getType(), obj.ID());
		}
		
        if (numID.obj != num) NSLog(@"ios2obj NSNumber Error");
        if (strID.obj != str) NSLog(@"ios2obj NSString Error");
        if (listID.obj != list) NSLog(@"ios2obj NSArray Error");
        if (mapID.obj != map) NSLog(@"ios2obj NSDictionary Error");
    }
    }
   // NSLog(@"Object and OC complete");
    
    
 //   PERFOR<<__FUNCTION__;
    @autoreleasepool { 
        for(int i=0; i<K_OCOBJC_CMP; i++){
            if (i%100 == 0) {
             //   printf("dyna i: %d \n", i);
            }
            int KTestRange=i+1;
            int randomBase=random()%KTestRange+1;
            if(i>1500)
                randomBase=random()%100+1;
            
            List _listDel = [self randomList:randomBase];
            List _listInsert= [self randomList:randomBase];
            List _listAdd= [self randomList:randomBase];
            
            NSMutableArray* testList = [NSMutableArray new];
            List testListDyna ;
            [self test_error_list_remove_add_insert_count:i testNS:testList testDyna:testListDyna _listDel:_listDel _listInsert:_listInsert _listAdd:_listAdd _listSize:i dyna:NO];
           if(testList.obj!=testListDyna){
                testList.obj.Save("testListArray");
                testListDyna.Save("testListDyna");
                DEBUG<< _listDel.toString();
                DEBUG<< testList.obj.toString();
                DEBUG<< testListDyna.toString();
                DEBUG<<"list 2 remove add insert ERROR i: "<<i;
                DEBUG<<"list 2 remove add insert ERROR";
            }
            [testList release];
        }
    }
    
    @autoreleasepool {
        PERFOR<<__FUNCTION__<<"Obj->OC";
        List list;
        int KIndex = K_Loop_Huge;
        for (int i = 0; i < KIndex; i++)
            list.add(String(i));
        NSMutableArray* testList = list;
        if([[testList objectAtIndex:KIndex/2] obj] != String(int(KIndex/2))){
            DEBUG<<"Error";
        }
    }
}


#pragma mark - Performance Ojbect VS ObjC test Section

- (void)test_performance_List_Add:(BOOL)dynamic{
	DEBUG<<__FUNCTION__;
	{
		List list ;
		String str = "abc";
		PERFOR<<KSep<<"Add List Size: ";
		for (int i = 0; i < K_Loop_Huge; i++){
			list.add(str);
		}
        log<<list.size();
	}
	{
		NSMutableArray* list = [NSMutableArray new];
		NSString* str = @"abc";
		PERFOR<<KSep<<"Add NSMutableArray Size:";
		for (int i = 0; i < K_Loop_Huge; i++){
			[list addObject:str];
		}
        log<<list.count;
		[list release];
	}
}


- (void)test_performance_List_Get:(BOOL)dynamic{

	DEBUG<<__FUNCTION__;
	
	int index = 0;
	//index = K_Loop_Huge-1;
	{
		List list ;
		String str = "abc";
		for (int i = 0; i < K_Loop_Huge; i++)
			list.add(str);
		String str2="";
		PERFOR<<KSep<<"Get List 0  Size: "<<list.size();
		for (int i = 0; i < K_Loop_Huge; i++){
			//str2 =
			list.get(index);
		}
	}
	
	index = K_Loop_Huge/2;
	
	{
		List list ;
		String str = "abc";
		for (int i = 0; i < K_Loop_Huge; i++)
			list.add(str);
		String str2="";
		PERFOR<<KSep<<"Get List mid  Size: "<<list.size();
		for (int i = 0; i < K_Loop_Huge; i++){
			//str2 =
			list.get(index);
		}
	}
	
	
	index = K_Loop_Huge-1;
	
	{
		List list ;
		String str = "abc";
		for (int i = 0; i < K_Loop_Huge; i++)
			list.add(str);
		String str2="";
		PERFOR<<KSep<<"Get List end  Size: "<<list.size();
		for (int i = 0; i < K_Loop_Huge; i++){
			//str2 =
			list.get(index);
		}
	}
	
	index = 0;
	{
		NSMutableArray* list = [NSMutableArray new];
		NSString* str = @"abc";
		for (int i = 0; i < K_Loop_Huge; i++)
			[list addObject:str];
	//	NSString* str2 = @"";
		PERFOR<<KSep<<"Get NSMutableArray 0 Size: "<<list.count;
		for (int i = 0; i < K_Loop_Huge; i++){
			//str2 =
			[list objectAtIndex:index];
		}
		[list release];
	}
	index = K_Loop_Huge/2;
	{
		NSMutableArray* list = [NSMutableArray new];
		NSString* str = @"abc";
		for (int i = 0; i < K_Loop_Huge; i++)
			[list addObject:str];
		//NSString* str2 = @"";
		PERFOR<<KSep<<"Get NSMutableArray mid  Size: "<<list.count;
		for (int i = 0; i < K_Loop_Huge; i++){
			//str2 =
			[list objectAtIndex:index];
		}
		[list release];
	}
	index = K_Loop_Huge-1;
	{
		NSMutableArray* list = [NSMutableArray new];
		NSString* str = @"abc";
		for (int i = 0; i < K_Loop_Huge; i++)
			[list addObject:str];
	//	NSString* str2 = @"";
		PERFOR<<KSep<<"Get NSMutableArray end  Size: "<<list.count;
		for (int i = 0; i < K_Loop_Huge; i++){
			//str2 =
			[list objectAtIndex:index];
		}
		[list release];
	}
}

- (void)test_performance_List_RemoveAtRandom:(BOOL)dynamic{
	DEBUG<<__FUNCTION__;
	{
		List list ;
		String str = "abc";
		for (int i = 0; i < K_Loop_Huge; i++)
			list.add(str);
		PERFOR<<KSep<<"Remove List Size: "<<list.size();
        int cur =K_Loop_Huge;
        int delPos=0;
        while (--cur>0) {
            delPos=random()%cur;
            list.removeIndex(delPos);
        }
	}
	{
		NSMutableArray* list = [NSMutableArray new];
		NSString* str = @"abc";
		for (int i = 0; i < K_Loop_Huge; i++)
			[list addObject:str];
		PERFOR<<KSep<<"Remove NSMutableArray Size: "<<list.count;
        int cur =K_Loop_Huge;
        int delPos=0;
        while (--cur>0) {
            delPos=random()%cur;
           [list removeObjectAtIndex:delPos];
        }
		[list release];
	}
}
- (void)test_performance_List_RemoveAtIndex:(BOOL)dynamic{
	DEBUG<<__FUNCTION__;
	{
		List list ;
		String str = "abc";
		for (int i = 0; i < K_Loop_Huge; i++)
			list.add(str);
		PERFOR<<KSep<<"Remove List Size: "<<list.size();
		for (int i = 0; i < K_Loop_Huge; i++){ 
			list.removeIndex(0);
		}
	}
	{
		NSMutableArray* list = [NSMutableArray new];
		NSString* str = @"abc";
		for (int i = 0; i < K_Loop_Huge; i++)
			[list addObject:str];
		PERFOR<<KSep<<"Remove NSMutableArray Size: "<<list.count;
		for (int i = 0; i < K_Loop_Huge; i++){
			[list removeObjectAtIndex:0];
		}
		[list release];
	}
}


- (void)test_performance_List_RemoveObject:(BOOL)dynamic{
	DEBUG<<__FUNCTION__;
//	return;
	{
		List list;
		String str = "abc";
		String str2 = "abc";
		for (int i = 0; i < K_Loop_Huge; i++)
			list.add(str);
		PERFOR<<KSep<<"Remove Obj List size: "<<list.size();
		for (int i = 0; i < K_Loop_Huge; i++){
			list.removeObj(str2);
		}

  //  DEBUG<<list.testInfo();
	}
	{
		NSMutableArray* list = [NSMutableArray new];
		NSString* str = @"abc";
		NSString* str2 = @"abc";
		for (int i = 0; i < K_Loop_Huge; i++)
			[list addObject:str];
		PERFOR<<KSep<<"Remove Obj NSMutableArray size: "<<list.count;
		for (int i = 0; i < K_Loop_Huge; i++){
			[list removeObject:str2];
		}
		[list release];
	}
}

- (void)test_performance_List_Insert:(BOOL)dynamic{

	//DEBUG<<__FUNCTION__;
	{
		List list;
		String str = "abc";
		list.add(str);
		PERFOR<<"Insert List";
		for (int i = 0; i < K_Loop_Small; i++){
			list.insert(str, i);
		}
	}
	{
		NSMutableArray* list = [NSMutableArray new];
		NSString* str = @"abc";
		[list addObject:str];
		PERFOR<<"Insert NSMutableArray";
		for (int i = 0; i < K_Loop_Small; i++){
			[list insertObject:str atIndex:0];
		}
		[list release];
	}
}
- (void)test_performance_Map:(BOOL)dynamic{
    
	//DEBUG<<__FUNCTION__;
    int Loop = 1000000;
	{
		Map map;
        {
            
            PERFOR<<"Map init";
            for (Integer i = 0; i._TInt64() < Loop; i++){
                map.put(i.toString()+"abcasdfasdfasdfasdf", i);
            }
        }
        {
            PERFOR<<"Map get";
            Integer num;
            for (Integer i = 0; i._TInt64() < Loop; i++){
                num = map.get(i.toString()+"abcasdfasdfasdfasdf");
            }
            DEBUG<<"Map end: "<<num;
        }
	}
	{
		NSMutableDictionary* dic = [NSMutableDictionary new];
        {
            PERFOR<<"Dic init";
            for (int i = 0; i < Loop; i++){
                [dic setObject:[NSNumber numberWithInt:i] forKey:[NSString stringWithFormat:@"%@%@",[NSString stringWithFormat:@"%d", i ],@"abcasdfasdfasdfasdf"]];
            }
        }
        {
            PERFOR<<"Dic get";
            NSNumber* num;
            for (int i = 0; i < Loop; i++){
                num =[dic objectForKey:[NSString stringWithFormat:@"%@%@",[NSString stringWithFormat:@"%d", i ],@"abcasdfasdfasdfasdf"]];
            }
            DEBUG<<"Dic end: "<<[num intValue];
        }
		[dic release];
	}
}


#pragma mark - Other test Section

-(void)test_performance_Other{
    
    {
        return;
    }
	//return;
    {
	int max = 100;
    List list;
    for (int i=0; i<max+1; i++)
        list.add(i);
    DEBUG<<list.testInfo();
        while (list.size()>30) {
            list.removeIndex(0);
        }
        DEBUG<<list.testInfo();
    }
	return;
	{
		int max = 1000000;
		List list;
        for (int i=0; i<max+1; i++)
			list.add(i);
		
		int KSample = 10;
		for(int i=0; i<=KSample; i++){
			float index=i;
			index/=KSample;
			index*=max;
			index-=1;
			if(index<0)continue;
			PERFOR<<"get "<<int(index);
			for (int i=0; i<max; i++)
				if( list.get(index) != Integer(index))
					DEBUG<<"Error: "<<index;
		}
	}
	
	
	
	{
		int max = 10000000;
		List list;
        for (int i=0; i<max+1; i++)
			list.add(i);
		
		int KSample = 10;
		for(int i=0; i<=KSample; i++){
			float index=i;
			index/=KSample;
			index*=max;
			index-=1;
			PERFOR<<"get "<<int(index);
			for (int i=0; i<max; i++)
				list.get(index);
		}
	}
	
	
	[self test_error_OjbectObjC];
	return;
    [self test_error_list_remove_add_insert];
    return;
    {
        List list;
        for (int i=0; i<300; i++){
            list.add(i);
			if(list.get(i)!=Integer(i))
            DEBUG<<list;
        }
        DEBUG<<list;
	}
    return;

	
    return;
	{
	List list;
	for (int i=0; i<6; i++)
		list.add(i);
	DEBUG<<list.get(4);
	DEBUG<<list.get(5);
	}
	
	
	[self test_error_list_remove_add_insert];
	return;
	//return;
	{
		List list;
		DEBUG<<list;
		list.add(-1);
		DEBUG<<list;
		list.removeIndex(0);
		DEBUG<<list;
	}
	{
		List list;
		list.add(-1);
		for (int i=0; i<20; i++) {
			list.insert(i, list.size()-1);
		}
		DEBUG<<list;
		while (list.size()>0) {
			list.removeIndex(0);
		}
		DEBUG<<list;
		list.add(-1);
		for (int i=0; i<20; i++) {
			list.insert(i, (list.size()-1)*3/4);
		}
		
		
		//DEBUG<<list.testInfo();
		DEBUG<<list;
	}
	
	
	return;
	Map map;
	map["A"]="a";
	List list;
	list.add(map);
	list.add(map);
	list.add(map);
	list.add(1);
	list.add(2);
	list.add("abc");
	list.add("defggh");
	DEBUG<<list[0]["A"];
	DEBUG<<list[0]["B"];
	DEBUG<<list[1]["A"];
	DEBUG<<list;
	
	
	
	
	
	
	
//
//    NSMutableArray* testList = [NSMutableArray new];
//    [testList addObject:@"0"];
//    [testList addObject:@"1"];
//    [testList addObject:@"0"];
//    [testList insertObject:@"123" atIndex:0];
//    [testList removeOneObject:@"0"];
////    [testList insertObject:@"ab" atIndex:1];
//    NSLog(@"%@", testList);
    return;
    
    
    
    [self test_error_OjbectObjC];
    
	return;
	
    return;
    {
        List list;
        for (int i = 0; i < 6; i++)
            list.add(Integer(i).toString());
        list.removeIndex(0);
        
        DEBUG<<"2 "<<list.toString();
        return;
        while (list.size()>0) {
            list.removeIndex(0);
        }
    }
    return;
    {
        List list;
        for (int i = 0; i < 100; i++)
            list.add(Integer(i).toString());
        DEBUG<<list.testInfo();
        list.removeIndex(list.size()-1);
//        list.remove(list.size()-1);
//        list.remove(list.size()-1);
        while (list.size()>2) {
            list.removeIndex(0);
        }
        
        DEBUG<<"1 "<<list.testInfo();
        list.removeIndex(0);
        DEBUG<<"2 "<<list.testInfo();
        
    }
    {
        DEBUG<<__FUNCTION__;
        //	return;
        {
            List list;
            String str = "abc";
            String str2 = "abc";
            for (int i = 0; i < K_Loop_Small; i++)
                list.add(str);
            PERFOR<<KSep<<"Remove Obj List size: "<<K_Loop_Small;
            for (int i = 0; i < 10000; i++){
                list.removeObj(str2);
            }
        }
        {
            NSMutableArray* list = [NSMutableArray new];
            NSString* str = @"abc";
            NSString* str2 = @"abc";
            for (int i = 0; i < K_Loop_Small; i++)
                [list addObject:str];
            PERFOR<<KSep<<"Remove Obj NSMutableArray size: "<<K_Loop_Small;
            for (int i = 0; i < K_Loop_Small; i++){
                [list removeObject:str2];
            }
            [list release];
        }

    }
    
    return;
    {
        List list;
        for (int i = 0; i < 5; i++)
            list.add(Integer(i).toString());
        DEBUG<<list.removeIndex(4)<<"\n";
        DEBUG<<list.removeIndex(2)<<"\n";
        DEBUG<<list.removeIndex(2)<<"\n";
        DEBUG<<list.removeIndex(1)<<"\n";
        DEBUG<<list.removeIndex(1)<<"\n";
        DEBUG<<list.removeIndex(1)<<"\n";
        DEBUG<<list.removeIndex(1)<<"\n";
        DEBUG<<list.removeIndex(1)<<"\n";
    }
    return;
    {
        List list;
        list.add(1);
        List list1;
        list1.add(1);
        if(list == list1){
            DEBUG<<"==";}
        else{
            DEBUG<<"!=";
        }
       // return;
    }
    return;
    {
        NSString* nsStr = String("abc");
        NSLog(@"Str: %@", nsStr);
    }
    return;
    {
        List list;
        for (int i = 0; i < 5; i++)
            list.add(Integer(i).toString());
        // PRINT_OBJ(list.testInfo());
        
      //  printf("index: %d\n",list.indexOfObject(0));
        printf("index: %d\n",list.indexOfObject("0"));
        printf("index: %d\n",list.indexOfObject("1"));
        printf("index: %d\n",list.indexOfObject("2"));
        printf("index: %d\n",list.indexOfObject("3"));
        printf("index: %d\n",list.indexOfObject("4"));
        printf("index: %d\n",list.indexOfObject(0));
        printf("index: %d\n",list.indexOfObject(0));
        return;
    }
    {
        List list;
        for (int i = 0; i < 20; i++)
            list.add(Integer(i).toString());
        DEBUG<<list.toString();
        list.removeIndex(19);
        list.removeIndex(18);
        list.removeIndex(0);
        list.removeIndex(-1);
        list.removeIndex(1000);
        list.removeObj(0);
        DEBUG<<list.toString();
        list.removeObj(3);
        DEBUG<<list.toString();
        list.removeObj("6");
        list.removeObj("7");
        list.removeObj("8");
        list.removeObj("9");
        list.removeObj("18");
        list.removeObj("10");
        DEBUG<<list.toString();
        return;
    }
    {
        {
            List list; 
            for (int i = 0; i < 2; i++)
                list.add(Integer(i).toString());
            while (list.size()>0)
                list.removeIndex(0);
            DEBUG<<"listRemove";
        }
        return;
    }
    {
		List::TEST_nodeDeep = 0;
		List::TEST_nodeCount=0;
		List list;
		String str = "abc";
		list.add(str); 
		for (int i = 0; i < 30; i++){
			list.insert(Integer(i).toString(), 0);
		} 
        DEBUG<<"List1: "<<list.toString();
	}
    {
		List::TEST_nodeDeep = 0;
		List::TEST_nodeCount=0;
		List list; 
		for (int i = 0; i < 30; i++){
			list.add(Integer(i).toString());
		}
        DEBUG<<"List2: "<<list.toString();
	}
    printf("fuck");
    return;
    {
		List::TEST_nodeDeep = 0;
		List::TEST_nodeCount=0;
		List list;
		String str = "abc";
		list.add(str);
		PERFOR<<"Insert Dy List Insert: mid   ";
		for (int i = 0; i < 100; i++){
			list.insert(Integer(i).toString(), i/2);
		}
        log<<"List size: "<<list.size()<<" NodeCount: "<<List::TEST_nodeCount<<"  Deep: "<< List::TEST_nodeDeep;
        DEBUG<<"List: "<<list.toString();
	}
    return;
    {
        List list;
		String str = "abc";
        list.add("start");
        String str2("w");
		for (int i = 0; i < 20; i++)
            //  list.add(Integer(i).toString());
			list.insert(Integer(i).toString(), 0);
   //     list.insert(str2, i/2);
        
        DEBUG<<"DEBUG: "<< list.testInfo();
        DEBUG<<"DEBUG: "<< list.testInfo();
        
    }
    return;
    {
        List list;
		String str = "abc";
		for (int i = 0; i < 200; i++)
			list.add(Integer(i).toString());
        
        DEBUG<<"DEBUG: "<< list.testInfo();
        DEBUG<<"DEBUG: "<< list.testInfo();
        
    }
//    printf("curMem: %f\n", currentMemoryUsage());
//    new char[1024*1000];
//    printf("curMem: %f\n", currentMemoryUsage());
//    
    return;
    {
        List list(16);
		String str = "abc";
		for (int i = 0; i < K_Loop_Huge; i++)
			list.add(str);
        return;
    }
    {
		List list;
		String str = "abc";
		for (int i = 0; i < K_Loop_Huge; i++)
			list.add(str);
		String str2="";
		PERFOR<<KSep<<"Get List 0";
		for (int i = 0; i < K_Loop_Huge; i++){
			//str2 =
			list.get(0);
		}
        NSLog(@"finalish");
	}
    return;
    
    
    {
        {
            Map map;
            map["B"];
            map["a"];
            DEBUG<<"Map: "<<map.toString();
         //   Object obj = map["C"];
         //   obj = "3";
        }
        
        
        {
        Map map; 
            map["B"];
            DEBUG<<"Map1: "<<map.toString();
            map["A"]=1;
            DEBUG<<"Map2: "<<map.toString();
            Object obj = map["C"];
            obj = "3";
            DEBUG<<"Map3: "<<map.toString();
            map["C"]="abcd";
            DEBUG<<"Map4: "<<map.toString();
        }
        
    }
	return;
	{
		List list;
		for (int i=0; i<10; i++) {
			list.add(i);
		}
		NSLog(@"ListInfo: %@  %d\n", list.toString().ID(), list.size());
		list.removeObj(0);
		list.removeObj(3);
		list.removeObj(5);
		list.removeObj(6);
		list.removeObj(9);
		NSLog(@"ListInfo: %@  %d\n", list.toString().ID(), list.size());
	}
	return; 
	{
		
		List list;
		list.add(0);
		list.insert(1, 0);
		NSLog(@"ListInfo: %@  %d\n", list.toString().ID(), list.size());
		list.removeIndex(0);
		NSLog(@"ListInfo: %@  %d\n", list.toString().ID(), list.size());
		list.add(8);
		NSLog(@"ListInfo: %@  %d\n", list.toString().ID(), list.size());
		list.removeObj(8);
		NSLog(@"ListInfo: %@  %d\n", list.toString().ID(), list.size());
        return;
		
	}
    {
        List list;
		list.add(1);
		list.add(2);
		list.removeObj(1);
        return;
    }
    {
    List list;
    list.add(1);
    Object obj = list.get(0);
    
    printf("type3: %d\n", obj.getType());
    NSLog(@"ListInfo: %@\n", obj.toString().ID());
    NSLog(@"ListGet: %@\n", list.testInfo().ID());
    NSLog(@"List: %@\n", list.toString().ID());
    }
    
    
    
    
	return;
	{
		{
			PERFOR<<"FOR INT+=";
			int size = 0;
			for (int i = 0; i < K_Loop_Huge; i++)
				for (int j = 0; j < 10000; j++)
					for (int k = 0; k < 10000; k++)
						for (int l = 0; l < 10000; l++)
					size+=1;
			printf("Size: %d\n", size);
		}
	}
	{
		{
			PERFOR<<"Malloc 1G";
			malloc(1024*1024*1024);
		}
		{
			PERFOR<<"Malloc 100K time";
			int size = 1024*1024;
			for (int i = 0; i < size; i++){
				malloc(1024*1024);
			}
		}
	}
	
	{
		List list(16);
		list.add("1");
		list.add(1);
		list.add(3);
		list.Save("testJSON");
	}
	{
		List list = Object::Load("testJSON");
		NSLog(@"%@", list.toString().ID());
	}
	
}

-(void)test_performance_NewFeature:(BOOL)dynamic{
	
	DEBUG<<__FUNCTION__<<"    dynamic list: "<<dynamic;

	{
		List::TEST_nodeDeep = 0;
		List::TEST_nodeCount=0;
		List list;
		String str = "abc";
		PERFOR<<"Add List ";
		for (int i = 0; i < K_Loop_Huge; i++){
			list.add(str);
		}
       // DEBUG<<"\n"<<list.toString();
	//	list.Save("list");
        log<<"List size: "<<list.size()<<" NodeCount: "<<List::TEST_nodeCount<<"  Deep: "<< List::TEST_nodeDeep;
	}
	{
		NSMutableArray* list = [NSMutableArray new];
		NSString* str = @"abc";
		PERFOR<<"Add ";
		for (int i = 0; i < K_Loop_Huge; i++){
			[list addObject:str];
		}
          log<<"NSMutableArray size: "<<list.count;
		[list release];
	} 
	//return;

	int index = K_Loop_Huge-1;
	//index=0;
   // index = K_Loop_Huge/2;
	
	{
		List::TEST_nodeDeep = 0;
		List::TEST_nodeCount=0;
		List list;
		String str = "abc";
		for (int i = 0; i < K_Loop_Huge; i++)
			list.add(str);
		String str2="";
		PERFOR<<"Get List end";
		for (int i = 0; i < K_Loop_Huge; i++){
			// str2 = list.get(index);
            list.get(index);
		}
        log<<"   "<<index<<" - "<<list.size();
	}
	 
	{
		NSMutableArray* list = [NSMutableArray new];
		NSString* str = @"abc";
		for (int i = 0; i < K_Loop_Huge; i++)
			[list addObject:str];
	//	NSString* str2 = @"";
		PERFOR<<"Get NSMutableArray";
		for (int i = 0; i < K_Loop_Huge; i++){
			// str2 = [list objectAtIndex:index];
             [list objectAtIndex:index];
		}
        log<<"   "<<index<<" - "<<list.count;
		[list release];
	}
	
	
	{
		List::TEST_nodeDeep = 0;
		List::TEST_nodeCount=0;
		List list;
		String str = "abc";
		for (int i = 0; i < K_Loop_Huge; i++)
			list.add(str);
		PERFOR<<"Remove List Size: "<<K_Loop_Huge;
		for (int i = 0; i < K_Loop_Huge; i++){
			list.removeIndex(0);
		}
	}
	{
		NSMutableArray* list = [NSMutableArray new];
		NSString* str = @"abc";
		for (int i = 0; i < K_Loop_Huge; i++)
			[list addObject:str];
		PERFOR<<"Remove NSMutableArray Size: "<<K_Loop_Huge;
		for (int i = 0; i < K_Loop_Huge; i++){
			[list removeObjectAtIndex:0];
		}
		[list release];
	}
	{
		List::TEST_nodeDeep = 0;
		List::TEST_nodeCount=0;
		List list;
		String str = "abc";
		list.add(str);
		PERFOR<<"Insert Dy List Insert: 0  ";
		for (int i = 0; i < K_Loop_Huge; i++){
			list.insert(str, 0);
		}
        log<<"List size: "<<list.size()<<" NodeCount: "<<List::TEST_nodeCount<<"  Deep: "<< List::TEST_nodeDeep;
	}
	{
		NSMutableArray* list = [NSMutableArray new];
		NSString* str = @"abc";
		[list addObject:str];
		PERFOR<<"Insert: 0   ";
		for (int i = 0; i < K_Loop_Huge; i++){
			[list insertObject:str atIndex:0];
		}
        log<<"NSMutableArray size: "<<list.count;
		[list release];
	}
    
    
    
	{
		List::TEST_nodeDeep = 0;
		List::TEST_nodeCount=0;
		List list;
		String str = "abc";
		list.add(str);
		PERFOR<<"Insert Dy List Insert: mid   ";
		for (int i = 0; i < K_Loop_Huge; i++){
			list.insert(str, i/2);
		}
        log<<"List size: "<<list.size()<<" NodeCount: "<<List::TEST_nodeCount<<"  Deep: "<< List::TEST_nodeDeep;
	}
	{
		NSMutableArray* list = [NSMutableArray new];
		NSString* str = @"abc";
		[list addObject:str];
		PERFOR<<"Insert: mid  ";
		for (int i = 0; i < K_Loop_Huge/100; i++){
			[list insertObject:str atIndex:i/2];
		}
        log<<"NSMutableArray size: "<<list.count;
		[list release];
	}
    
    
    
	{
		List::TEST_nodeDeep = 0;
		List::TEST_nodeCount=0;
		List list;
		String str = "abc";
		list.add(str);
		PERFOR<<"Insert Dy List Insert: end   ";
		for (int i = 0; i < K_Loop_Huge; i++){
			list.insert(str, i);
		}
        log<<"List size: "<<list.size()<<" NodeCount: "<<List::TEST_nodeCount<<"  Deep: "<< List::TEST_nodeDeep;
	}
	{
		NSMutableArray* list = [NSMutableArray new];
		NSString* str = @"abc";
		[list addObject:str];
		PERFOR<<"Insert: end  ";
		for (int i = 0; i < K_Loop_Huge; i++){
			[list insertObject:str atIndex:i];
		}
        log<<"NSMutableArray size: "<<list.count;
		[list release];
	}
	
	
	DEBUG<<"\n\n";
}
-(void)test_testMap1000K {
    DEBUG<<"1000k start";
    int testLoop =10000;
    Map map;
    for(Integer i=0; i._TInt64()<testLoop; i++){
        map.put(i.toString()+"asldfkjalsdkjfalskdfjasklf", i);
    }
    
    DEBUG<<"1000k mid";
    Integer num;
    for(Integer i=0; i._TInt64()<testLoop; i++){
        num = map.get(i.toString()+"asldfkjalsdkjfalskdfjasklf");
    }
    DEBUG<<"1000k end:"<<num;
}
-(void)Test_ioStream{
    Bytes bytes =String("121").toBytes();
    TPtr8 ptr = bytes.desc();
    printf("array: %d\n", ptr[ptr.length-1]);
    ptr[ptr.length-1]='3';
    String newStr =bytes.toString();
    //DEBUG<<newStr;

    //    outStream.write(1);
    //    outStream.writeInt(2222);
    //    outStream.writeInt64(9999999999);
    
    //    printf("%d\n",inStream.read());
    //    printf("%d\n",inStream.readInt());
    //    printf("%lld\n",inStream.readInt64());
    //  printf("%",inStream.readVarString());
    //   outStream.writeString(String("abc").toBytes());
    //  Bytes by =inStream.getRemainderBytes();
    //String str = by.toString();
    // DEBUG<<by.toString();
    
    
    ByteArrayOutputStream outStream;
      outStream.write(1);
       outStream.writeInt(2222);
       outStream.writeInt64(9999999999);
    outStream.writeString(bytes);
    outStream.writeString(bytes);
    
    ByteArrayInputStream inStream(outStream.toBytes());
    printf("%d\n",inStream.read());
    printf("%d\n",inStream.readInt());
    printf("%lld\n",inStream.readInt64());
    DEBUG<<inStream.readString();
    ByteArrayInputStream ais(inStream.getRemainderBytes());
    int lengthStr =ais.readInt();
    DEBUG<<lengthStr;
    Bytes strByte(lengthStr);
    ais.read(strByte);
    String byte2str(strByte.desc());
    DEBUG<<byte2str ;
    
    
    
    outStream.write(1);    
}
-(void)Test_facade{
    DEBUG<<"**********  JSONXV TEST FACADE  ***********\n";
    PERFOR<<"JSONXV FACADE END\n\n";
    [self test_facade];
    return ;
}
- (void)Test_jsonX
{
	DEBUG<<"**********  JSONXV TEST START  ***********\n";
    PERFOR<<"JSONXV TEST END\n\n";

    [self Test_ioStream];
    
    List list;
    for(int i=0; i<100; i++){
        list.add(i);
    }
    while (list.size()>0) {
        list.removeIndex(0);
    }
    
    while (YES) { 
        [self test_str_list];
    }
	[self test_testMap1000K];
	//Other
	[self test_performance_Other];
//    return;
//    return;
	//return;
	//New Feature
    
	DEBUG<<"Performance";
    
	for (int i=0; i<2; i++)
	{
		BOOL key = YES;
		//if(i==1)
		//	key=NO;
		if(key){
			[self test_performance_NewFeature:key];
		}
		//return;
		//Performance
		[self test_performance_List_Get:key];
		[self test_performance_List_Add:key];
		if(key){
			[self test_performance_List_RemoveAtIndex:key];
			[self test_performance_List_RemoveObject:key];
			[self test_performance_List_RemoveAtRandom:key];
			[self test_performance_List_Insert:key];
		}
        
		//return;
		//LIST
		[self test_error_list_Array:key];
		[self test_error_list_excludeDuplicates:key];
		[self test_error_list_indexOfObject:key];
		[self test_error_list_contains:key];
		[self test_error_list_ADD:key];
		[self test_error_list_add_get:key];
		[self test_error_list_set_get:key];
		if(key){
			[self test_error_list_remove_add_insert];
			[self test_error_list_insert_get:key];
			[self test_error_list_removeObj:key];
			[self test_error_list_remove:key];
		}
        
        [self test_performance_Map:key];
		
	}
	
    
    
    //MAP
    [self test_error_map_mess];
    [self test_error_map_Array];
	[self test_error_map_Array_VS_Put];
    
    //Integer
    [self test_error_Integer];
	
	//OjbectObjC
	[self test_error_OjbectObjC];
	
	//test type assign
	[self test_error_TypeAssign];
	
}


@end
