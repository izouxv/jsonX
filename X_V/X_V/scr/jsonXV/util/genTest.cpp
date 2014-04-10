//
//  Util.h
//  X_V
//
//  Created by zouxu on 7/7/13.
//  Copyright (c) 2013 zouxu. All rights reserved.
//

#include "genTest.h"
#include "jsonXV.h"
#include "UtilPlatform.h"
#include <iostream>
#include "FacadeX.h"
#include <assert.h>
#include <float.h>
#include "ByteArrayInputStream.h"
#include "ByteArrayOutputStream.h"





#if 0
static const int K_Loop_Huge = 10000000;
static const int K_Loop_Small =100000;
static const int K_OCOBJC_CMP =5000;
#else
static const int K_Loop_Huge = 100000;
static const int K_Loop_Small =10000;
static const int K_OCOBJC_CMP =1000;
#endif








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


static const char* KSep = "                             ";

#pragma mark - List test Section


const int KLoopInner = 3;

void test_error_list_remove_add_insert_count(int count,  List& listTestSample,
                                             List& listTestSampleDyna,
                                             const List&_listDel,
                                             const List&_listInsert,
                                             const List&_listAdd,
                                             int _listSize,
                                             bool dyna
                                             )
{
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

List randomList(int range) {
    int num=random()%range+1;
    List list(num+1);
    for(int i=0; i<num; i++){
        list.add(int(random()%range));
    }
    return list;
}
void test_error_list_remove_add_insert() {
    PERFOR<<__FUNCTION__;
    
    
    //PERFOR<<__FUNCTION__;
    for(int i=0; i<500; i++){
        //printf("dyna i: %d \n", i);
        int KTestRange=i+1;
        int delCase=random()%KTestRange+1;
        
        List _listDel =   randomList(KTestRange);
        List _listInsert=  randomList(KTestRange);
        List _listAdd=  randomList(KTestRange);
        
        List testList(8);
        List testListDyna;
        test_error_list_remove_add_insert_count(delCase, testList, testListDyna, _listDel, _listInsert, _listAdd, i, false);
 
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
        
        List _listDel = randomList(KTestRange);
        List _listInsert= randomList(KTestRange);
        List _listAdd= randomList(KTestRange);
        
        List testList(8);
        List testListDyna ;
            test_error_list_remove_add_insert_count(delCase, testList, testListDyna, _listDel, _listInsert, _listAdd, i, false);
        if(testList!=testListDyna){
            testList.Save("testList1");
            testListDyna.Save("testListDyna");
            //	DEBUG<<"list remove add insert ERROR";
            DEBUG<<"list 1 remove add insert ERROR";
        }
    }
}

 void test_error_list_insert_get(bool  dynamic){
    //by_zouxu not easy to test
    List list ;
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

 void test_error_list_add_get(bool  dynamic){
        List list ;
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

         void test_error_list_set_get(bool  dynamic){
        List list ;
        list.add(111);
        PERFOR<<__FUNCTION__;
        for(int i=0;i<K_Loop_Huge; i++){
            list.set(i, 0);
            if(list.get(0)!=i){
                WARN<<__FUNCTION__<<" error";
            }
        }
    }

          void test_error_list_removeObj(bool  dynamic){
        PERFOR<<__FUNCTION__;
        for(int i=0;i<K_Loop_Small; i++){
            List list ;
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

        
        void test_error_list_Array(bool  dynamic){
        PERFOR<<__FUNCTION__;
        List list ;
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

        void test_error_list_indexOfObject(bool  dynamic){
        PERFOR<<__FUNCTION__;
        List list ;
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

        void test_error_list_excludeDuplicates(bool  dynamic){
        return;
        PERFOR<<__FUNCTION__;
        List list ;
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

                void test_error_list_remove(bool  dynamic){
        PERFOR<<__FUNCTION__;
        
        List list ;
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

        void test_error_list_contains(bool  dynamic){
        PERFOR<<__FUNCTION__;
        
        List list ;
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

              void test_error_list_ADD(bool  dynamic){
        PERFOR<<__FUNCTION__;
        
        for(int i=0;i<K_Loop_Small; i++){
            
            List list ;
            list.add(0);
            list.add("abc");
            
            List list2;
            list2.add(0);
            list2.add(1);
            list2.add("abc");
            
            List list3;
            list3+=list+list2;
            
            if(list3.size()!=5){
                WARN<<__FUNCTION__<<" error";
            }
        }
    }
    
  void test_error_list_link(){
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
    
     
    
    
void test_facade(){
//    double cur = curTime();
//    printf("1   %f  \n", cur);
//      cur = curTime();
//    printf("2   %f  \n", cur);
//      cur = curTime();
//    printf("3   %f  \n", cur);
    
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
    
  void test_str_list(){
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
    
  void test_error_map_mess(){
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
    
 void test_error_map_Array(){
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
    
    void test_error_map_Array_VS_Put(){
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
    
void test_error_Integer(){
        
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
            
            
//            if( ! DOUBLE_EQUAL(fuck, 0.55))
//                WARN<<__FUNCTION__<<" 8error";
//            if( ! DOUBLE_EQUAL(numNum._double(), 0.55))
//                WARN<<__FUNCTION__<<" 9error";
        }
    }
    
    
    
#pragma mark - OjbectType test Section
    
   void test_error_TypeAssign()
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
        

   void test_testMap1000K(){
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
           void Test_ioStream(){
         Bytes bytes =String("121").toBytes();
//        TPtr8 ptr = bytes.desc();
//        printf("array: %d\n", ptr[ptr.length-1]);
//        ptr[ptr.length-1]='3';
//        String newStr =bytes.toString();
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

void Test_StrSplit(){
    List list = String("a,b,c").Split(',');
    DEBUG<<list;
    DEBUG<<list;
    
    String str("abcdefghijk");
    Bytes strByte = str.toBytes();
    List listByte = strByte.split(3);
    DEBUG<<listByte;
    for(int i=0; i<listByte.size(); i++){
        Bytes item =listByte[i];
        String str =item.toString();
        DEBUG<<str;
    }
    
}
void Test_Bytes(){
    {
        Bytes bytes(100);
        bytes.Empty();
        String strin("1");
        String strin2("2");
//        bytes+=strin.toBytes();
//        bytes+=strin2.toBytes();
        for (int i=0;i<1000; i++) {
            String strin2(i);
            strin2+="_";
            bytes+=strin2.toBytes();
        }
        String str3 = bytes.toString();
        DEBUG<<str3;
        DEBUG<<str3;
    }
    {
    String strin("abcdef1234567");
    String strin2("ABC");
    Bytes bytes=strin2.toBytes();
    bytes+=strin.toBytes();
    String str3 = bytes.toString();
    DEBUG<<str3;
    DEBUG<<str3;
    }
}
void Test_autoAddString(){

    String strin("abcdef1234567");
    String strin2("ABC");
    Bytes byte2=strin2.toBytes();
    ByteArrayOutputStream baos;
    baos.writeInt(1000);
    baos.writeShortString(strin.toBytes());
    baos.writeShortString(byte2);
    baos.writeShortString(strin2.toBytes());
    Bytes byte=baos.toBytes();
    
    ByteArrayInputStream bais(byte);
    int packId=bais.readInt();
    Bytes content2 = bais.readShortBytes();
    printf("content2: %s\n", content2.Char());
    DEBUG<<content2.toString();
    Bytes content = bais.readShortBytes();
    int shortLength =bais.readShort();
    Bytes remainBytes =bais.getRemainderBytes();
    DEBUG<<"Str: "<<remainBytes.toString();
    DEBUG<<"Str: "<<remainBytes;
    
}

void Test_map(){
    Map map;
    for(int i=0; i<100; i++){
        map.put(i, 1);
      //  DEBUG<<map.getKeys();
    }
}

void Test_jsonX(){
    return;
    Test_Bytes();
      Test_autoAddString();
        DEBUG<<"**********  JSONXV TEST START  ***********\n";
        PERFOR<<"JSONXV TEST END\n\n";
      
       test_facade();
     // return;
      Test_map();
      Test_ioStream();
      Test_StrSplit();
      
        
       // while (true) {
      //      test_str_list();
     //   }
      test_testMap1000K();
        //Other
        //    return;
        //    return;
        //return;
        //New Feature
        
        DEBUG<<"Performance";
        
        for (int i=0; i<2; i++)
        {
            bool key = true;
 
            //return;
            //LIST
            test_error_list_Array(key);
            test_error_list_excludeDuplicates(key);
            test_error_list_indexOfObject(key);
            test_error_list_contains(key);
            test_error_list_ADD(key);
            test_error_list_add_get(key);
            
           // test_error_list_remove_add_insert(key);
            test_error_list_insert_get(key);
            test_error_list_removeObj(key);
            test_error_list_remove(key);
            
            
        }
        
        
        
      //MAP
      test_error_map_mess();
      test_error_map_Array();
      test_error_map_Array_VS_Put();
      test_error_Integer();
      test_error_TypeAssign();
        
    }
    
    
    void jsonXVTestCpp(){
        
      //  printf("\n%lld\n", curTimeGen());
       // printf("\n%f\n", curTime());
        
        String str = "123";
        DEBUG<<str;
        Test_jsonX();
        
    }
    
