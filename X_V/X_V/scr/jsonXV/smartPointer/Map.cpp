/*
 ============================================================================
 Name		: Map.cpp
 Author	  : 
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CMap implementation
 ============================================================================
 */

#include "Map.h"
#include "Handle.h"
#include "List.h"
#include "Pair.h" 
#include "String.h" 
#include "Integer.h"
#include "Util.h"
#include "Bytes.h"

#define EnabelGlobleKey 0
//
//class MapKeyGloble {
//#ifdef __WINS__
//public:
//#endif
//	Map allKeys;
//	MapKeyGloble(){
//		allKeys = Map(MapSizeMax);
//	}
//public:
//	static MapKeyGloble* GetInstance() {
//		static MapKeyGloble* instance = NULL;
//		if (!instance)
//			instance = new MapKeyGloble;
//		return instance;
//	}
//
//	String get(const String& aKey) {
//		String obj = allKeys.get(aKey);
//		if(obj.getType()==OT_Object)
//			return obj;
//		allKeys.put(aKey, String(""));
//		return aKey;
//	}
//
//	void Shink() {
////		for (int i = allKeys.size() - 1; i >= 0; i--) {
////			Object object(allKeys.get(i));
////			int refCount = object.handle->getRefCount();
////			if (refCount == 2) {
////				allKeys.remove(i);
////			}
////		}
//	}
//};




class ObjArrayOperHandle: public XV::Handle {
public:
    virtual void destroy(){
//        if( asign==ArrayOperType_Deleted)
//            return;
//        if(asign != ArrayOperType_AsignSucNextDel){
//            asign=ArrayOperType_Deleted;
//            operList.removeObj(Object(this));
//        }
    }
    int sizeOf(){
        return sizeof(this);
    }
    virtual bool compare(Handle* handle){return false;};
	virtual String toString(){return "err oper handle is null";};
	Bytes toBytes() { return toString().toBytes(); }
	ObjectType getType() {
		return OT_ObjArrayOper;
	}
    ObjArrayOperHandle(const List& aOperList, const Map& aMap, const Object& aKey) :
    Handle() {
        operList = aOperList;
        map = aMap;
        key = aKey;
        asign = ArrayOperType_Idle;
	}
    List operList;
    Map map;
    Object key;
    ArrayOperType asign;
};

ObjArrayOper::ObjArrayOper(const List& aOperList, const Map& aMap, const Object& aKey) :
Object() {
    LEAVE(ET_TypeError_Map);
    updateHandle(new ObjArrayOperHandle(aOperList, aMap, aKey));
}
void ObjArrayOper::setValueAndRelease( const Object& aValue){
    ObjArrayOperHandle* tmp = (ObjArrayOperHandle*)handle;
    tmp->map.put(tmp->key, aValue);
//    tmp->asign=ArrayOperType_AsignSucNextDel;
   // printf("objOper 1: %d\n", tmp->operList.size());
    tmp->asign=ArrayOperType_Deleted;
    tmp->operList.removeObj(*this);
   // printf("objOper 2: %d\n", tmp->operList.size());
}
void ObjArrayOper::setStatus(ArrayOperType asign){
    ObjArrayOperHandle* tmp = (ObjArrayOperHandle*)handle;
    tmp->asign = asign;
}
ArrayOperType ObjArrayOper::getStatus(){
    ObjArrayOperHandle* tmp = (ObjArrayOperHandle*)handle;
    return tmp->asign;
}

ObjArrayOper::ObjArrayOper( const Object& object):
Object() {
    if (object.getType() == OT_ObjArrayOper) {
		updateHandle(object.handle);
		incRef();
    }
}

class MapHandle: public XV::Handle {
public:
	int mapSize;
	List table;
	List keys;

	ObjectType getType() {
		return OT_Map;
	}
    int sizeOf(){
        return sizeof(this);
    }
    
	void reset(int size) {
		//		table.clear();
		//		keys.clear();
        mapSize =size;// MapSizeMin;
		keys = List();
		table = List();
		for (int i = 0; i < mapSize; i++) {
			table.add(List(2));
		}
	}
	MapHandle(int mapS) :
		Handle() { 
        reset(mapS);
	}

	void put(const Object& key, const Object& value) {
		int cell = key.hashCode() % mapSize;
        List& temp = (List&)table.get(cell);
		int len = temp.size();
		for (int i = 0; i < len; i++) {
			Pair& pair = (Pair&) temp.get(i);
			if (pair.getKey() == key) {
				pair.setValue(value);
				temp.set(pair, i);
				return;
			}
		}
#if EnabelGlobleKey//by_zouxu not tested
		if (key.getType() == OT_String) {
			Object newKey = key;
			{
				static String str = "";
				static Map MapKeyGloble = Map(MapSizeMax);
				String keyGlobal = MapKeyGloble.get(key);
				if(keyGlobal.getType()==OT_Object) 
					MapKeyGloble.put(key,str);
				else
					newKey = keyGlobal;
			}
			keys.add(newKey);
			Pair add(newKey, value);
			temp.add(add);
		} else
#endif
		{
			keys.add(key);
			Pair add(key, value);
			temp.add(add); 
            
		}
		table.set(temp, cell);
        
        if(keys.size()/mapSize>=8){
            GrowMapSize(mapSize*4);
        }
	}
    
 
	Object& get(const Object& key) {
//		if (keys.contains(key)) {
			int cell = key.hashCode() % mapSize;
			//List temp(table.get(cell));
            List& temp = (List&)table.get(cell);
			int len = temp.size();
			for (int i = 0; i < len; i++) {
				Pair& pair = (Pair&) temp.get(i);
				//if (pair.getKey() == key) {
                if (pair.getKey().hashCode() == key.hashCode() && pair.getKey() == key) {
					return pair.getValue();
				}
			}
//		}
		NOTFOUND(ET_NotFound_MapItem)
	} 

//	Object& GET(const Object& key) {
////		if (keys.contains(key)) {
//        int cell = key.hashCode() % mapSize;
//        //List temp(table.get(cell));
//        List& temp = (List&)table.get(cell);
//        int len = temp.size();
//        for (int i = 0; i < len; i++) {
//            Pair& pair = (Pair&) temp.get(i);
//            if (pair.getKey() == key) {
//                return pair.getValue();
//            }
//        }
////		}
//		
//		NOTFOUND(ET_NotFound_MapItem)
//	}

	void remove(const Object& key) {
		//if (keys.contains(key)) {
			int cell = key.hashCode() % mapSize;
			List& temp = (List&)table.get(cell);
			//int len = temp.size();
			//			for (int i = 0; i < len; i++) {
			for (int i = temp.size() - 1; i >= 0; i--) {
				 Pair& pair = (Pair&) temp.get(i);
				if (pair.getKey() == key) {
					temp.removeIndex(i);
					keys.removeObj(key); 
				}
			} 
	}

	void destroy() { 
	}

	bool contains(const Object& key) {
	//	Object& obj =get(key);
	//	return obj.getType() != OT_Object;
		return keys.contains(key);
	}

	int size() {
		return keys.size();
	}
	
    void GrowMapSize(TUint aMapSize) {
        if(aMapSize<=mapSize)return;
		Map map(aMapSize);
		for (int i = 0; i < keys.size(); i++)
			map.put(keys.get(i), get(keys.get(i)));
        
          mapSize = aMapSize;
          table = map.getTables();
          keys = map.getKeys();
        }
	bool compare(Handle* handle) {
		//if(!handle)return false;
		//MapHandle* temp = (MapHandle*) handle;
		//return (temp && mapSize == temp-> mapSize && table == temp->table && keys == temp->keys);
		MapHandle* temp = (MapHandle*) handle;
		if(keys.size() != temp->keys.size())
			return false;
		
			for (int i = 0; i < keys.size(); i++)
				//if (get(keys.get(i)) != temp->get(keys.get(i)))
				if ( temp->get(keys.get(i)) != get(keys.get(i)) )
					return false;
			return true;
	}
    
	String toString() {
		String str = String(toBytes().desc());
		return str;
	}
	Bytes toBytes() {
		Object object(this);
		Map map(object);
		Bytes bytes = Util::tocJSONBytes(map);
		return bytes;
	}
};

Map::Map(int mapSize) :
	Object() {
        updateHandle(new MapHandle(mapSize));
}
Map::Map(const Object& object) :
	Object() {
	if (object.getType() == OT_Map) {
		updateHandle(object.handle);
		incRef();
		return;
	}else{
        LEAVE(ET_TypeError_Map);
        updateHandle(new MapHandle(MapSizeMin));
    }
}

void Map::put(const Object& key, const Object& value) {
	MapHandle* temp = (MapHandle*) handle;
	temp->put(key, value);
}

Object& Map::get(const Object& key) const {
	MapHandle* temp = (MapHandle*) handle;
    return temp->get(key);
}
void Map::remove(const Object& key) {
	MapHandle* temp = (MapHandle*) handle;
    return temp->remove(key);
}
bool Map::contains(const Object& key) const {
	MapHandle* temp = (MapHandle*) handle;
    return temp->contains(key);
}

int Map::size() const {
	MapHandle* temp = (MapHandle*) handle;
    return temp->size();
}

Map Map::operator+(const Map& data) const {
	Map map;
	for (int i = 0; i < getKeys().size(); i++) {
		Object key = getKeys().get(i);
		map.put(key, get(key));
	}
	for (int i = 0; i < data.getKeys().size(); i++) {
		Object key = data.getKeys().get(i);
		map.put(key, data.get(key));
	}
	return map;
}

void Map::operator+=(const Map& data) {
	*this = *this + data;
}

List Map::getKeys() const {
	MapHandle* temp = (MapHandle*) handle;
    return temp->keys;
}
List Map::getTables() const {
	MapHandle* temp = (MapHandle*) handle;
    return temp->table;
}
//Object& Map::operator[] (const Object& key){
//	return get(key) ;
//}
//void Map::GrowMapSize(TUint aMapSize) {
//	MapHandle* temp = (MapHandle*) handle;
//		return temp->GrowMapSize(aMapSize);
//}

void Map::reset() {
	MapHandle* temp = (MapHandle*) handle;
    temp->reset(MapSizeMin);
}

#ifdef __WINS__
void Map::updateHandle(Handle* value) {
	Object::updateHandle(value);
	internal = (MapHandle*) value;
}
#endif

