jsonX
=====

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
        
       
        
        
