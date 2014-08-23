jsonX
=====
ios with c++ smart point

sample 1:

        Map map; 
        map.put("key", "value");
        map.put("key1", String("value"));
        map.put("key2", 1);
        map.put("key3", 0.5);
        map.put("key4", Integer(6));
         WARN<<__FUNCTION__<<map;
        
        
sample 2:

        Integer num(2);
        String str("123");
        List list;
        list.add(str);
        list.add(num);
        Map map;
        map.put("list", list);
         WARN<<__FUNCTION__<<map;
        
       
        
sample 3: 

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
        
