
/*
 ============================================================================
 Name	 : List.cpp
 Author	  :
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CList implementation
 ============================================================================
 */

#include "List.h"
#include "Handle.h"
#include "String.h"
#include "Util.h"
#include "assert.h"
#include "Map.h"
#include "Integer.h"
#include "Bytes.h"
#include <stdio.h>



#if 0
#define KRevese 0
const int KMaxNodeItemSize = 2;
const int KMaxListItemSize = 1;
#else
#define KRevese 0
const int KMaxNodeItemSize = 8;
const int KMaxListItemSize = 64;
#endif


#define KListMaxItem 100000000// 4096

#define KFastObjCopy 1
#define KFastObjCopyLeftOrRight 0

int List::TEST_nodeCount = 0;
int List::TEST_nodeDeep = 0;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
////////////////                //////////////////////////////////////////
////////////////   ListHandle   //////////////////////////////////////////
////////////////                //////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////



class ListHandleBase: public XV::Handle {
public:
    virtual int sizeOf()=0;
	virtual void insert(const Object& obj, int pos)=0;
	virtual  int add(const Object& obj)=0;
	virtual void set(const Object& obj, int where)=0;
	virtual Object& get(int where) const=0;
	virtual int getSize() const=0;
	virtual bool remove( int pos)=0;
	virtual bool removeObj(const Object& object)=0;
	//virtual void destroy()=0;
	virtual void reset()=0;
    virtual String testInfo()const=0;
	virtual List mid(int pos, int length)=0;
    virtual int indexOfObject(const Object& object)=0;
	//virtual bool compare(Handle* handle)=0;
	virtual String toString()=0;
    virtual  Bytes toBytes()=0;
};





class ListHandle: public ListHandleBase {
public:
	Object* data;
	int size;
	int index;
	ObjectType getType() {
		return OT_List;
	}
    virtual int sizeOf(){
        return sizeof(ListHandle);
    }
	ListHandle(int aSize) :
	ListHandleBase() {
		size = aSize;
		data = NULL;
		//data = new Object[size];
		//index = 0;
		index = 0;
        if(size>0)
			allocMem(size);
	}
    
	virtual void insert(const Object& obj, int pos) {
		if(pos<0 || pos>=index)
			return;
		
		allocMem(0);
	//	if (pos < 0 || pos > index)
	//		pos = index;
#if KFastObjCopyLeftOrRight //performance is good
		if(index-pos>0){
			Object*from=data;
			Object*to=data;
			from+=pos;
			to+=pos+1;
			Util::memcpy(to,from,sizeof(Object)*(index-pos));
			data[pos].handle=NULL;
		}
#else
		for (int i = index; i > pos; i--) {
			data[i] = data[i - 1];
		}
#endif
		//data[pos] = Object();
		data[pos] = obj;
		index++;
	}
    
	virtual  int add(const Object& obj) {
		allocMem(0);
		data[index++] = obj;
        return index-1;
		//	 insert(obj, index);
	}
    
	virtual void set(const Object& obj, int where) {
		if (where >= 0 && where < index) {
			data[where] = obj;
			return;
		}
		LEAVE(ET_OutOfBounds)
	}
	
	virtual Object& get(int where) const {
		if (where < index && where >= 0) {
            return data[where];
		}
		NOTFOUND(ET_NotFound_ListItem)
	}
	
	virtual int getSize() const {
		return index;
	}
	
	virtual bool remove( int pos) {
		if (pos < index && pos >= 0) {
#if KFastObjCopyLeftOrRight//performance is good
			//printf("%d %d %d\n", pos, index, size);
			data[pos] = Object();
			if(index-1-pos > 0){
				Object*from=data;
				Object*to=data;
				to+=pos;
				from+=pos+1;
				Util::memcpy(to,from,sizeof(Object)*(index-1-pos));
			}
			//if(index>0)
            data[--index].handle=NULL;
#else
			data[pos] = Object();
			for (int k = pos; k + 1 < index; k++) {
				data[k] = data[k + 1];
			}
			data[--index] = Object();
#endif
			return true;
		}
		LEAVE(ET_OutOfBounds)
		return false;
	}
	
	virtual bool removeObj(const Object& object) {
		for (int i = 0; i < index; i++) {
			if (data[i] == object) {
				remove(i);
				return true;
			}
		}
		LEAVE(ET_OutOfBounds)
		return false;
	}
    
	void allocMem(int itemSize){
		if (index == size || itemSize>0) {
			if (size > KListMaxItem)
				size += KListMaxItem;
			else
				size *= 2;
			if(itemSize>0)
				size = itemSize;
#if KFastObjCopy
			Object* temp = (Object*)malloc(size*sizeof(Object)+1);
            if(!temp)
                LEAVE(ET_MemError_ListAllocMem);
			for (int i = index; i < size; i++)temp[i].handle=NULL;
			if(index>0){
				Util::memcpy(temp, data,sizeof(Object)*index);
				free(data);
			}
            data = temp;
            //            if(!data)
            //                data = (Object*)malloc(size*sizeof(Object)+1);
            //            else
            //                data = (Object*)realloc(data, size*sizeof(Object)+1);
#else
			Object* temp = new Object[size+1];
			if(index>0){
				for (int i = 0; i < index; i++) {
					temp[i] = data[i];
				}
				delete[] data;
			}
			data = temp;
#endif
		}
	}
    
	virtual void destroy() {
		
		if (data) {
#if KFastObjCopy
			for (int i = 0; i < index; i++){
				data[i].deRef();
				data[i].handle=NULL;
			}
			free(data), data = NULL;
#else
			delete[] data, data = NULL;
#endif
		}
		size = 0;
		index = 0;
	}
	
	virtual void reset() {
		destroy();
        size = KListInitSize;
		//	 data = new Object[size];
		//	 index = 0;
		index = 0;
		allocMem(size);
	}
	
    virtual String testInfo()const{
        return "";
    }
    
	virtual List mid(int pos, int length){
		List list;
		if(pos>=0 && length>0 && pos+length<=getSize()){
            for (int i = pos; i < pos+length; i++) {
                list.add(get(i));
            }
        }
		return list;
	}
	
    virtual int indexOfObject(const Object& object) {
        for (int i = 0; i < index; i++) {
            if (data[i] == object) {
                return i;
            }
        }
        return -1;
    }
	
	virtual bool compare(Handle* handle) {
        {
            ListHandle* temp = (ListHandle*) handle;
            if(temp->getSize()!=temp->getSize())
                return false;
            int size = temp->getSize();
            for (int i = 0; i < size; i++) {
                if (get(i) != temp->get(i))
                    return false;
            }
            return true;
        }
		ListHandle* temp = (ListHandle*) handle;
		if(index!=temp->index)
			return false;
        
        for (int i = 0; i < index; i++) {
            if (get(i) != temp->get(i))
                return false;
        }
        return true;
	}
	
	virtual String toString() {
        String str = String(toBytes().desc());
        return str;
    }
    
    virtual  Bytes toBytes(){
        Object object(this);
        List list(object);
        Bytes bytes = Util::tocJSONBytes(list);
        return bytes;
    }
};




//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
////////////////              ////////////////////////////////////////////
////////////////   ListDyna   ////////////////////////////////////////////
////////////////              ////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


#define EnableListDynaLast 1
class ListDyna{
public:
    Object data;
    ListDyna* next;
#if EnableListDynaLast
    ListDyna* last;
#endif
protected:
    ListDyna(){
        next=NULL;
        #if EnableListDynaLast
        last=NULL;
#endif
    }
public:
    virtual ~ListDyna(){
        if(next)
            delete next;
        next=NULL;
    }
    ListDyna* _getEndPtr(){
        ListDyna* tmp = this;
        while (tmp) {
            if(tmp->next)
                tmp=tmp->next;
            else break;
        }
        return tmp;
    }
    static ListDyna* genItem(){
        return new ListDyna();
		static int itemCount=0;
		static ListDyna* items=NULL;
		if(itemCount == 0){
			itemCount = 1000000;
			items = new ListDyna[itemCount];
		}
		return &items[--itemCount];
        //return new ListDyna();
    }
    static void delItem(ListDyna* item){
		item->next=NULL;
        delete item;
    }
    
    Object __Print(){
        List list;
        ListDyna* tmp = this;
        while (tmp) {
            list.add(tmp->data);
            tmp=tmp->next;
        }
        return list;
    }
    
};


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
////////////////              ////////////////////////////////////////////
////////////////   ListNode   ////////////////////////////////////////////
////////////////              ////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


class ListNode{
public:
	int size;
    ListDyna* listDyna;
    ListDyna* endListPtr;
	
public:
    void __Print(){
        if(listDyna==NULL)
            assert(endListPtr==NULL && size==0);
        if(endListPtr==NULL)
            assert(listDyna==NULL && size==0);
        if( size==0)
            assert(listDyna==NULL && endListPtr==NULL);
        
        int sizeTmp = 1;
#if EnableListDynaLast
        assert(listDyna->last==NULL);
#endif
        ListDyna* tmp =listDyna;
        while (tmp->next) {
            sizeTmp++;
#if EnableListDynaLast
            assert(tmp->next->last==tmp);
#endif
            tmp=tmp->next;
        }
        assert(sizeTmp == size);
        assert(tmp == endListPtr);
    }
    
    ListNode(){
        listDyna=NULL;
        endListPtr=NULL;
        size=0;
    }
	
    virtual ~ListNode(){
        if(listDyna)
            delete listDyna;
        listDyna=NULL;
		size = 0;
    }
    
    
    ListNode* getTail(){
        ListNode* tail = new ListNode();
        tail->size = this->size/2;
        
        int pos = tail->size;
#if ! EnableListDynaLast
        ListDyna* tmpLast=NULL;
#endif
        ListDyna* tmp = listDyna;
        while (pos-- > 0 && tmp){
#if ! EnableListDynaLast
            tmpLast=tmp;
#endif
            tmp=tmp->next;
        }
        this->size/=2;
        
#if EnableListDynaLast
        //set tail
        tail->endListPtr=this->endListPtr;
        this->endListPtr = tmp->last;
        this->endListPtr->next=NULL;
        
        //set this tail end
        tmp->last->next=NULL;
        tmp->last=NULL;
#else
        tmpLast->next=NULL;
        tail->endListPtr=this->endListPtr;
        this->endListPtr->next=tmpLast;
#endif
        tail->listDyna = tmp;
       
            //set tail
//        tail->endListPtr = tail->listDyna->_getEndPtr();
//        this->endListPtr = this->listDyna->_getEndPtr();

        return tail;
    }
    
    inline void insert(const Object& obj, int pos){
	
        if(pos == 0){
			size++;
            ListDyna* newItem = ListDyna::genItem();
            newItem->data = obj;
            newItem->next=listDyna;
            if(listDyna){
#if EnableListDynaLast
                listDyna->last=newItem;
#endif
            }
            else
                endListPtr=newItem;
			listDyna=newItem;
        }else{
#if ! EnableListDynaLast
            ListDyna* tmpLast=NULL;
#endif
			// printf("pos: %d \n", pos);
            ListDyna* tmp = listDyna;
       //     printf("insertPos: %d\n", pos);
            while (pos-- > 0){
#if ! EnableListDynaLast
                tmpLast=tmp;
#endif
                tmp=tmp->next;
            }
            if(tmp){//in range, the pos exist in link
				size++;
                ListDyna* newItem = ListDyna::genItem();
#if EnableListDynaLast
                newItem->last=tmp->last;
                tmp->last->next=newItem;
                newItem->next=tmp;
                tmp->last = newItem;
#else
                tmpLast->next=newItem;
                newItem->next=tmp;
#endif
                newItem->data = obj;
            }else{//out of range - not exist
                printf("insertOutOfRange\n");
            }
        }
    }
    
    inline int add(const Object& obj){
        ListDyna* newItem = ListDyna::genItem();
		newItem->data=obj;
		size++;
		
		if(!listDyna){
			listDyna=newItem;
            endListPtr=newItem;
			return size;
		}
#if 1
        ListDyna* tmp = endListPtr;
        endListPtr=newItem;
#else
        ListDyna* tmp = listDyna;
        while (tmp){
            if(tmp->next){
                tmp=tmp->next;
            }
            else
                break;
        }
#endif

#if EnableListDynaLast
        newItem->last = tmp;
#endif
        tmp->next = newItem;
		
        return size;
    }
    inline void set(const Object& obj, int pos){
        ListDyna* tmp = listDyna;
        while (pos-- > 0 && tmp)
            tmp=tmp->next;
        if(tmp){
            tmp->data = obj;
        }
    }
    virtual Object& get(int pos) {
        ListDyna* tmp = listDyna;
        while (pos-- > 0 && tmp)
            tmp=tmp->next;
        if(tmp){
		//	printf("type2: %d\n", tmp->data.getType());
            return tmp->data;
        }else{
			//  printf("get3: %d\n", pos);
        }
		NOTFOUND(ET_NotFound_ListItem)
	}
    bool remove(int pos){
		if(!listDyna)
			return false;
        if(pos==0){
			size--;
			ListDyna* next=listDyna->next;
			//listDyna->next=NULL;
			ListDyna::delItem(listDyna);
			listDyna=next;
            if(!listDyna)
                endListPtr=NULL;
			return true;
        }else{
#if ! EnableListDynaLast
            ListDyna* tmpLast=NULL;
#endif
            ListDyna* tmp = listDyna;
            while (pos-- > 0 && tmp){
#if ! EnableListDynaLast
                tmpLast=tmp;
#endif
                tmp=tmp->next;
            }
            if(tmp){
				size--;
#if EnableListDynaLast
                tmp->last->next = tmp->next;
				if(tmp->next)
					tmp->next->last = tmp->last;
                else  endListPtr=tmp->last;
#else
                 tmpLast->next=tmp->next;
                if(!tmp->next)
                    endListPtr=tmpLast;
#endif
				
               // tmp->next=NULL;
                ListDyna::delItem(tmp);
				//PRINT_DYNA(this);
                return true;
            }
        }
        return false;
    }
    bool removeObj(const Object& object){
		if(!listDyna)
			return false;
		
		if(listDyna->data == object){
			size--;
			ListDyna* next=listDyna->next;
			ListDyna::delItem(listDyna);
			listDyna=next;
            if(!listDyna)
                endListPtr=NULL;
			return true;
		}
#if ! EnableListDynaLast
        ListDyna* tmpLast=NULL;
#endif
		ListDyna* tmp = listDyna->next;
        while (tmp){
            if(tmp->data == object){
				size--;
#if EnableListDynaLast
				tmp->last->next = tmp->next;
				if(tmp->next)
                    tmp->next->last = tmp->last;
                else  endListPtr=tmp->last;
#else
                tmpLast->next=tmp->next;
                if(!tmp->next)
                    endListPtr=tmpLast;
#endif
                
                ListDyna::delItem(tmp);
                return true;
            }
#if ! EnableListDynaLast
            tmpLast=tmp;
#endif
            tmp=tmp->next;
        }
        return false;
    }
	int indexObj(const Object& object){
		if(!listDyna)
			return -1;
		
		if(listDyna->data == object){
			return 0;
		}
		int pos =0;
		ListDyna* tmp = listDyna->next;
        while (tmp){
			pos++;
            if(tmp->data == object)
				return pos; 
            tmp=tmp->next;
        }
        return -1;
	}
};



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
////////////////               ///////////////////////////////////////////
////////////////   IndexNode   ///////////////////////////////////////////
////////////////               ///////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////





class IndexNode{
public:
    IndexNode* son;
    IndexNode* father;
    IndexNode* last;
    IndexNode* next;
    IndexNode* endSonPtr;
    
	int deepSize;//for combin
    int sonSize;//for next node
    int listItems;//total of all list items
    ListNode* listPtr;//null when it have son
    
    int TEST_nodeId;
public:
    IndexNode(){
        son=NULL;
        father=NULL;
        last=NULL;
        next=NULL;
        endSonPtr=NULL;
        deepSize = 0;
        sonSize = 0;
        listItems=0;
        listPtr=NULL;
//        listPtr=new ListNode();
        TEST_nodeId = 0;
        List::TEST_nodeCount++;
        TEST_nodeId = List::TEST_nodeCount;
    }
    ~IndexNode() {
        List::TEST_nodeCount--;
        // Destroy();
	}
    void Destroy(){
        IndexNode* tmp=son;
		
		while (tmp) {
            tmp->Destroy();
            IndexNode* NEXT = tmp->next;
			delete tmp;
            tmp=NEXT;
        }
		tmp=next;
		while (tmp) {
            tmp->Destroy();
            IndexNode* NEXT = tmp->next;
			delete tmp;
            tmp=NEXT;
        }
		son=NULL;
		next = NULL;
        father = NULL;
        last = NULL;
        endSonPtr=NULL;
		deepSize=0;
		listItems=0;
		sonSize=0;
        delete listPtr;
        listPtr=NULL;
    }
	void reset() {
        Destroy();
        // listPtr=new ListNode();
	}
    
    Object __Print(){
        
		const bool KEnableNodeListTestInfo_Mid = true;
		
        Map map;
		map.put("ID", TEST_nodeId);
		if(KEnableNodeListTestInfo_Mid){
			
			map.put("listItem", listItems);
			map.put("deepSize", deepSize);
			map.put("sonSize", sonSize);
			
			
            
            
            
			{
                //son
                int listItemTmp=0;
				int sonSizeTmp = 0;
                if(this->son){
                    IndexNode* tmp = this->son;
                    assert(listPtr==NULL);//by_zouxu
                    assert(this->son->last==NULL);
                    
                    while (tmp) {
                        listItemTmp+=tmp->listItems;
                        assert(tmp->father ==this);
                        sonSizeTmp++;
                        // printf("1    %d==%d \r\n", sonSize, sonSizeTmp);
                        if(tmp->next){
                            assert(tmp == tmp->next->last);
                        }else{
                            assert(tmp == endSonPtr);
                        }
                        tmp=tmp->next;
                    }
                }else{
                    assert(endSonPtr==NULL);
                    // listItemTmp+=listItems;
                    listItemTmp+=listItems;
                    assert(listPtr!=NULL);
                    // if(listPtr){
                    listPtr->__Print();
                    //sonSizeTmp+=listPtr->size;
                    //   printf("2    %d==%d \r\n", sonSize, sonSizeTmp);
                    //}
                }
                
                assert(0 == 0);
                assert(1 == 1);
                // printf("3    %d==%d \r\n", sonSize, sonSizeTmp);
                assert(sonSize == sonSizeTmp);
                // printf("4    %d==%d \r\n", listItems, listItemTmp);
                assert(listItems == listItemTmp);
			}
			
			if(father)
				map.put("father", father->TEST_nodeId);
		}
		
        if(sonSize==0){
            if(KEnableNodeListTestInfo_Mid)map.put("TYPE", "list");
			if(listPtr){
				List listTmp;
				for (int i=0; i<listPtr->size; i++) {
					listTmp.add(listPtr->get(i));
				}
				map.put("value", listTmp);
			}
            //				map.put("value", listPtr->get(0));
        }else{
			if(KEnableNodeListTestInfo_Mid)
				map.put("TYPE", "node");
            if(endSonPtr) map.put("endSonPtr", endSonPtr->TEST_nodeId);
            else  map.put("endSonPtr", "NULL");
            Map mapSon;
            IndexNode* sonTmp = son;
            while (sonTmp) {
                mapSon.put(Integer(sonTmp->TEST_nodeId).toString(), sonTmp->__Print());
                sonTmp=sonTmp->next;
            }
            map.put("sons", mapSon);
        }
        return map;
    }
    
    
	inline IndexNode* _getNode(int& pos){
		
		if(pos>listItems || pos<0)return NULL;
		//by_zouxu, if pos > itemSize/2, pos-=itemSize, endPtr->startPtr
	
			IndexNode* node = this;
#if KRevese 
			if(  !node->endSonPtr || (node->endSonPtr && pos <= listItems/2) )
			{
#endif
			while(node){
				if(node->listItems<=pos){
					pos-=node->listItems;
					node=node->next;
				}else{
					if(node->son){
						node=node->son;
					}else{
						return node;
					}
				}
			}
#if KRevese
		}
            else{
        //         bool debugEnable = false;
                pos-=listItems;
                node = this->endSonPtr;
			//	  if(debugEnable)  printf("1    POS: %d ID: %d LISTITEM: %d \n",   pos, node->TEST_nodeId, node->listItems);
            while(node){
                if(node->listItems+pos>=0){
        //         if(debugEnable)   printf("2    POS: %d ID: %d LISTITEM: %d \n",   pos, node->TEST_nodeId, node->listItems);
                    if(node->endSonPtr)
                    node = node->endSonPtr;
                    else {
						if(pos == 0)return NULL;
						pos+=node->listItems;
		//					if(debugEnable)   printf("2.5    POS: %d ID: %d LISTITEM: %d \n",   pos, node->TEST_nodeId, node->listItems);
						return node;
					}
        //         if(debugEnable)   printf("3    POS: %d ID: %d LISTITEM: %d \n",   pos, node->TEST_nodeId, node->listItems);
                    continue;
                }
                pos+=node->listItems;
                if(node->last ){
         //       if(debugEnable)    printf("4    POS: %d ID: %d LISTITEM: %d \n",   pos, node->TEST_nodeId, node->listItems);
                   // if(node->last->listItems+pos<0)
                    node=node->last;
                    //else continue;
                }else if(node->endSonPtr){
          //        if(debugEnable)  printf("5    POS: %d ID: %d LISTITEM: %d \n",   pos, node->TEST_nodeId, node->listItems);
						node=node->endSonPtr;
					}else{
			//		if(debugEnable)	printf("6    POS: %d ID: %d LISTITEM: %d \n",   pos, node->TEST_nodeId, node->listItems);
						if(!node)printf("error 2 NULL");
						return node;
					}
			//if(debugEnable) printf("7    POS: %d ID: %d LISTITEM: %d \n",   pos, node->TEST_nodeId, node->listItems);
				
            }
            }
        
//			else{
//                bool debugEnable = true;
//                
//			pos-=listItems;
//			node = this->endSonPtr;
//			if(debugEnable)PRINT_NODE(this->_rootNode());
//			if(debugEnable)printf("1 %d\n", pos);
//			while(node){
//				if( pos+node->listItems>=0){
//					node=node->_lastNode();
//                    if(debugEnable)printf("1--- %d\n", pos);
//					//pos = 0;//node->listItems-1;
//					if(debugEnable)printf("2    POS: %d        ID: %d\n",    pos, node->TEST_nodeId);
//                    if(pos>=0)
//                        return node;
//				}else{
//                    pos+=node->listItems;
//                }
//                    if(node->last && node->last->listItems+pos<0){
//                        node=node->last;
//                    }else if(node->endSonPtr && node->endSonPtr->listItems+pos<0){
//						node=node->endSonPtr;
//					}else{
//						if(debugEnable)printf("5    POS: %d       ID: %d\n",   pos, node->TEST_nodeId);
//						 node=node->father;
//					}
//            
//			}
//		}
#endif
	//	printf("error NULL\n");
        return NULL;
    }
    
	IndexNode* _rootNode(){
		IndexNode* node = this;
        while(node){
            if(node->father)
				node=node->father;
			else
				return node;
        }
        return node;
	}
	
    inline IndexNode* _lastNode(){//always true
#if 1
            IndexNode* node = this;
            while(node){
                if(node->endSonPtr)
                    node = node->endSonPtr;
                else
                    return node;
            }
        return node;
#else
        IndexNode* node = this;
        while(node){
            if(node->next)
                node=node->next;
            else if(node->son)
                node=node->son;
            else
                return node;
        }
        
        return node;
#endif
    }
    
    void _deepSizeIncrease(int deep){
		IndexNode* node = this;
		while(node){
			if(node->deepSize<=deep){
				node->deepSize=deep;
				node=node->father;
				deep++;
				if(List::TEST_nodeDeep<deep)
					List::TEST_nodeDeep=deep;
			}else{
				break;
			}
        }
    }
    
    inline void listItem_AddSub(int num){
		IndexNode* node = this;
        while(node){
            node->listItems+=num;
            node=node->father;
        }
    }
	
	int _listItem(IndexNode* sonLink){
		int tmp = 0;
		while (sonLink) {
			tmp+=sonLink->listItems;
			sonLink = sonLink->next;
		}
		return tmp;
	}
	int _deepSize(IndexNode* sonLink){
		int tmp = 0;
		while (sonLink) {
			if(tmp<sonLink->deepSize)
				tmp = sonLink->deepSize;
			sonLink = sonLink->next;
		}
		return tmp;
	}
    IndexNode* _getEndSonPtr(){
        IndexNode* tmp = this->son;
        while (tmp) {
            if(tmp->next)
				tmp = tmp->next;
            else break;
        }
        return tmp;
    }
	
	IndexNode* new_removeRang(int start, int length){
		if(start+length<=sonSize && start>=0 && length>0){
			int lengthTMP = length;
			int startTMP = start;
			
			//get start ptr
			IndexNode* startRang = this->son;
			while (startTMP-->0) {
				startRang=startRang->next;
			}
			
			//get end ptr
			int listITEMS = 0;
			IndexNode* endRang=startRang;
			while (lengthTMP-->0) {
				listITEMS+=endRang->listItems;
				endRang=endRang->next;
			}
			
			//link head and tail
			if(startRang){
				if(startRang->last)
					startRang->last->next = endRang;
				startRang->last=NULL;
			}
			if(endRang){
				endRang->last->next=NULL;
				endRang->last = startRang->last;
			}
			this->sonSize-=length;
			this->listItem_AddSub(-listITEMS);
			if(start==0){
                this->son = endRang;
                if(this->son)
                    this->son->last=NULL;
                assert(this->listPtr==NULL);
               // assert(this->son->last==NULL);
            }
			this->deepSize=_deepSize(this->son)+1;
			if(!this->son)this->deepSize=0;
            this->endSonPtr = this->_getEndSonPtr();
			
			//add a new father node
			IndexNode* newNode = new IndexNode();
			newNode->sonSize=length;
			newNode->son=startRang;
			newNode->deepSize=_deepSize(newNode->son)+1;
			newNode->listItem_AddSub(listITEMS);
			
			//set father as new node
            IndexNode* tmp = newNode->son;
			while (tmp) {
                tmp->father = newNode;
                if(tmp->next)
				tmp = tmp->next;
                else break;
			}
            newNode->endSonPtr = tmp;
             newNode->endSonPtr = newNode->_getEndSonPtr();
            SAFE_DEL(newNode->listPtr);
           // newNode->listPtr=NULL;
			assert(newNode->listPtr==NULL);
            assert(newNode->son->last==NULL);
			return newNode;
		}
		return NULL;
	}
    void new_combin(IndexNode* node){
		
	}
	void new_Reorder(IndexNode* tailNode){
		//        tailNode->new_addNewIndex();
		while (true) {
			if(tailNode->father){
				if(tailNode->father->sonSize == 2*KMaxNodeItemSize){
					//	printf("INFO  ID: %d, SonSize: %d, DeepSize: %d\n", tailNode->TEST_nodeId, tailNode->sonSize, tailNode->deepSize);
					new_combin(tailNode->father);
					if(tailNode->father && tailNode->father->sonSize != 2*KMaxNodeItemSize)
						break;
					IndexNode* faTmp = tailNode->father;
					IndexNode* tailSection =tailNode->father->new_removeRang(KMaxNodeItemSize, KMaxNodeItemSize);
					if(faTmp->father){//add tail to the father's next position
						//	printf("faTmp ID: %d   sonSize: %d\n", faTmp->father->TEST_nodeId, faTmp->father->sonSize);
						faTmp->father->new_insert(tailSection,faTmp->next);
						tailNode=tailSection;
                        
                        faTmp->father->endSonPtr = faTmp->father->_getEndSonPtr();
                        
					}else{//split two section
						IndexNode* headSection =  faTmp->new_removeRang(0, KMaxNodeItemSize);
						faTmp->new_insert(headSection, NULL);
						faTmp->new_insert(tailSection, NULL);
                        
                        faTmp->endSonPtr = tailSection;
                        
                        //set value to the next loop
						tailNode=faTmp;
					}
				}else if(tailNode->father->sonSize > 2*KMaxNodeItemSize){
                    printf("Error ID: %d   sonSize: %d\n", tailNode->father->TEST_nodeId, tailNode->father->sonSize);
				}else{
					break;
				}
			}else{//root node
				//				printf("rootNode ID: %d   sonSize: %d\n", tailNode->TEST_nodeId, tailNode->sonSize);
				break;
				//PRINT_NODE(headSection);
				//tailNode->new_addNewIndex();
			}
		}
    }
	IndexNode* new_remove(IndexNode* aSon){
     //   printf("del: %d\n", aSon->TEST_nodeId);
        if(this->sonSize<=0)
            return NULL;
		IndexNode* newFa = new IndexNode();
		if(this->son == aSon){
			this->listItem_AddSub(-1* aSon->listItems);
			this->sonSize--;
			this->son = this->son->next;
            if(this->son){
            this->son->last=NULL; 
                assert(this->son->last==NULL);
            }
            assert(this->listPtr==NULL);
			this->deepSize = _deepSize(this->son)+1;//by_zouxu need set all fathers
            if(!aSon->next){
                this->endSonPtr = NULL;
            }
			aSon->next=NULL;
		}else{
			
			this->listItem_AddSub(-1* aSon->listItems);
			this->sonSize--;
			if(aSon->next)
				aSon->next->last=aSon->last;
            else this->endSonPtr = aSon->last;
            if(aSon->last)
			aSon->last->next=aSon->next;
			aSon->next=NULL;
			aSon->last=NULL;
		}
		
		
		newFa->son=aSon;
        newFa->endSonPtr=aSon;
		newFa->deepSize=_deepSize(aSon)+1;
		newFa->listItems=_listItem(aSon);
		newFa->sonSize = 1;
		newFa->listItems=aSon->listItems;
         assert(newFa->listPtr==NULL);
        assert(newFa->son->last==NULL);
		return newFa;
	}
	
	//when replace is null , it is add operation
	void new_insert(IndexNode* aSon, IndexNode* aReplace_MustExistInSonArray){
		if(this->son == aReplace_MustExistInSonArray){//change son ptr
			aSon->next = this->son;
			aSon->father = this;
			if(this->son)
				this->son->last = aSon;
			this->son = aSon;
            assert(this->listPtr==NULL);
            assert(this->son->last==NULL);
			this->sonSize++;
			
			aSon->_deepSizeIncrease(aSon->deepSize);
			aSon->father->listItem_AddSub(aSon->listItems);
			
		}else{
			IndexNode* node = aReplace_MustExistInSonArray;
			if(node == NULL){
				node = this->son;
				while(node){
					if(node->next )
						node=node->next;
					else break;
				}
				node->next = aSon;
				aSon->last = node;
			}else{
				if(node->last)
					node->last->next = aSon;
				aSon->last = node->last;
				aSon->next= node;
				node->last=aSon;
			}
			
			aSon->father = node->father;
			aSon->_deepSizeIncrease(aSon->deepSize);
			if(aSon->father){
				aSon->father->listItem_AddSub(aSon->listItems);
				aSon->father->sonSize++;
			}
		}
	}
    //	bool combin(IndexNode* aLeft, IndexNode* aRight){
	//		//same deepSize and total large with KMaxNodeItemSize
	//
	//		if(aLeft->deepSize==aRight->deepSize){
	//			if(aLeft->sonSize+aRight->sonSize >= KMaxNodeItemSize)
	//				return false;
	//			else{
	//				aRight->removeFromFather();
	//				link(aLeft, aRight, true);
	//                return true;
	//			}
	//		}
	//
	//		if(aLeft->deepSize>aRight->deepSize){
	//			int time = aLeft->deepSize-aRight->deepSize;
	//			while (time-->0) {
	//				aLeft = aLeft->son;
	//			}
	//			link(aLeft, aRight, true);
	//            return true;
	//		}else{
	//			int time = aRight->deepSize-aLeft->deepSize;
	//			while (time-->0) {
	//				aRight = aRight->son;
	//			}
	//			link(aLeft, aRight, false);
	//            return true;
	//		}
	//
	//		return false;
	//	}
	//
	//	void combinFatherNode(IndexNode* aFather){
	//		int deepSize = aFather->deepSize-1;
	//		IndexNode* tmp=aFather->son;
	//		while (tmp) {
	//			if(tmp->deepSize<deepSize){
	//				if(tmp->next){
	//                    if(DEBUGPRINT)
	//                        printf("deep: %d   %d", aFather->deepSize, tmp->deepSize);
	//					if(combin(tmp, tmp->next))
	//                        aFather->sonSize--;
	//                }
	//			}
	//			tmp = tmp->next;
	//		}
	//	}
	//PRINT_LIST
	IndexNode* new_addNewIndex(){
		if(this->son){
            
            ListNode*tail = this->endSonPtr->listPtr->getTail();
            this->endSonPtr->listItem_AddSub(-1*tail->size);
            
			IndexNode* newNode = new IndexNode();
            newNode->listPtr=tail;
            newNode->listItems=tail->size;
            
			this->new_insert(newNode, NULL);
            this->endSonPtr = newNode;
            
            return newNode;
        }else if(this->father){
            return this->father->new_addNewIndex();
		}else{
            
            IndexNode* copyFatherListNode = new IndexNode();
            delete copyFatherListNode->listPtr;
			
            IndexNode* newNode = new IndexNode();
            newNode->last = copyFatherListNode;
            delete newNode->listPtr;
            
          //   PRINT_DYNA(this->listPtr->listDyna);
            ListNode* firstList = this->listPtr;
            ListNode* secondList = firstList->getTail();
          //  PRINT_DYNA(firstList->listDyna);
          //  PRINT_DYNA(secondList->listDyna);
            
            this->listPtr=NULL;
			copyFatherListNode->listItems= firstList->size;
            copyFatherListNode->listPtr=firstList;
            
            
            
			newNode->listPtr = this->listPtr;
            this->listPtr = NULL;
			newNode->listItems= secondList->size;
            newNode->listPtr=secondList;
            
            newNode->father = this;
			copyFatherListNode->father = this;
			
            copyFatherListNode->next = newNode;
			this->sonSize=2;
			this->son = copyFatherListNode;
            assert(this->listPtr==NULL);
            assert(this->son->last==NULL);
			this->_deepSizeIncrease(1);
            
            this->endSonPtr = newNode;
            
            return copyFatherListNode;
            
            
//			IndexNode* copyFatherListNode = new IndexNode();
//			copyFatherListNode->father = this;
//			copyFatherListNode->listItems= this->listItems;
//			delete copyFatherListNode->listPtr;
//            copyFatherListNode->listPtr= this->listPtr;
//            this->listPtr = NULL;
//			
//            IndexNode* newNode = new IndexNode();
//            newNode->last = copyFatherListNode;
//            newNode->father = this;
//            copyFatherListNode->next = newNode;
//			this->sonSize=2;
//			this->son = copyFatherListNode;
//			this->_deepSizeIncrease(1);
//
//            this->endSonPtr = newNode;
//            
//            return newNode;
            
            
        }
        return NULL;
	}
	

	
	IndexNode* new_insertNewIndex(){
		if(this->father){
       //     PRINT_DYNA(this->listPtr->listDyna);
            
            ListNode*tail = this->listPtr->getTail();
        //    PRINT_DYNA(this->listPtr->listDyna);
        //    PRINT_DYNA(tail->listDyna);
            this->listItem_AddSub(-1*tail->size);
//            PRINT_LIST
            
			IndexNode* newNode = new IndexNode();
            newNode->listPtr=tail;
            newNode->listItem_AddSub(tail->size);
            
			this->father->new_insert(newNode, this->next);
			//if(!this->next)
			//	this->father->endSonPtr=newNode;
			
			this->father->endSonPtr = this->father->_getEndSonPtr();//need optimization, by_zouxu
			
			return newNode;
		}else{
             
           // printf("\nDYNA :%d\n%s\n", __LINE__, this->listPtr->listDyna->Print().toString().getBytes().des().source); 
        //    PRINT_DYNA(this->listPtr->listDyna);
            
            IndexNode* copyFatherListNode = new IndexNode();
            delete copyFatherListNode->listPtr;
			
            IndexNode* newNode = new IndexNode();
            newNode->last = copyFatherListNode;
            delete newNode->listPtr;
            
            ListNode* firstList = this->listPtr; 
            ListNode* secondList = firstList->getTail();
            
         //   PRINT_DYNA(firstList->listDyna);
         //   PRINT_DYNA(secondList->listDyna);
            
            this->listPtr=NULL;
			copyFatherListNode->listItems= firstList->size;
            copyFatherListNode->listPtr=firstList;
            
			newNode->listPtr = this->listPtr;
            this->listPtr = NULL;
			newNode->listItems= secondList->size;
            newNode->listPtr=secondList;
            
            newNode->father = this;
			copyFatherListNode->father = this;
			
            copyFatherListNode->next = newNode;
			this->sonSize=2;
			this->son = copyFatherListNode;
            assert(this->listPtr==NULL);
            assert(this->son->last==NULL);
			this->_deepSizeIncrease(1);
            
            this->endSonPtr = newNode;
            
            return copyFatherListNode;
            
//			IndexNode* copyFatherListNode = new IndexNode();
//			copyFatherListNode->father = this;
//			
//            IndexNode* newNode = new IndexNode();
//            newNode->last = copyFatherListNode;
//            delete newNode->listPtr;
//			newNode->listPtr = this->listPtr;
//            this->listPtr = NULL;
//            newNode->father = this;
//			newNode->listItems= this->listItems;
//			
//            copyFatherListNode->next = newNode;
//			this->sonSize=2;
//			this->son = copyFatherListNode;
//			this->_deepSizeIncrease(1);
//            
//            return copyFatherListNode;
        }
        return NULL;
	}
    
    bool new_removeObj(const Object& obj){
        IndexNode* tmp = this->son;
        while (tmp) {
            if(tmp->new_removeObj(obj))
                return true;
            tmp=tmp->next;
            if(!tmp)
                return false;
        }
        
        tmp = this;
        while (tmp) {
           // PRINT_OBJ(obj);
          //  PRINT_DYNA(tmp->listPtr->listDyna);
			if(tmp->listPtr && tmp->listPtr->removeObj(obj)) {
				tmp->listItem_AddSub(-1);
                if(tmp->listItems==0)
                    _deleteOrder(tmp);
				return true;
			}
            tmp=tmp->next;
        }
        return false;
    }
    int new_indexObj(const Object& obj){
        IndexNode* tmp = this->son;
        int nodeBefore=0; 
        while (tmp) {
            int cur = tmp->new_indexObj(obj);
            if(cur>-1){
                return nodeBefore+cur;
            }else{
                nodeBefore+=tmp->listItems;
            }
            tmp=tmp->next;
            if(!tmp)return -1;
        } 
        tmp = this;
        while (tmp && tmp->listPtr) {
            int cur = tmp->listPtr->indexObj(obj);
			if(cur>-1) { 
				return nodeBefore+cur;
			}else{
                  nodeBefore+=tmp->listItems;
            }
            tmp=tmp->next; 
            
        }
        return -1;
    }
//    int new_indexObj(const Object& obj){
//        IndexNode* tmp = this->son;
//		int current=0;
//        while (tmp) {
//            int index = tmp->new_indexObj(obj);
//			if(index>-1){
//                return current+index;
//			}
//			current+=tmp->listItems;
//            tmp=tmp->next;
//        }
//        
//        tmp = this;
//        while (tmp) {
//            PRINT_OBJ(obj);
//            PRINT_DYNA(tmp->listPtr->listDyna);
//			current = tmp->listPtr->indexObj(obj);
//
//			if(current>-1)
//				return current;
//            tmp=tmp->next;
//        }
//        return -1;
//    }
	
    void insert(const Object& obj, int pos) {
        
        IndexNode* node = _getNode(pos);
        if(node){
            
         //   printf("insert: %d    %d\n", node->TEST_nodeId, pos);
            node->listPtr->insert(obj, pos);
			node->listItem_AddSub(1);
            
			if(node->listItems == 2*KMaxListItemSize){
           //     PRINT_NODE(_rootNode());
          //      printf("thisNode: %d    %d", node->TEST_nodeId, pos);
				IndexNode* tmp =node->new_insertNewIndex();
				new_Reorder(tmp);
				node =tmp;
          //      printf("thisNode: %d    %d", node->TEST_nodeId, pos);
           //     PRINT_NODE(_rootNode());
			}

		//	PRINT_NODE(_rootNode());
        }
	}
	inline int add(const Object& obj) {
		IndexNode* last = _lastNode();
        if(!last->listPtr)
            last->listPtr = new ListNode();
        last->listPtr->add(obj);
		last->listItem_AddSub(1);
        
		if(last->listItems == 2*KMaxListItemSize){
			IndexNode* tmp =last->new_addNewIndex();
			new_Reorder(last);
			last =tmp;//_lastNode();
		}

        return listItems-1;
	}
	void set(const Object& obj, int pos) {
        IndexNode* node = _getNode(pos);
        if(node) node->listPtr->set(obj, pos);
	}
	virtual Object& get(int pos) {
        IndexNode* node = _getNode(pos);
       // printf("%d %d\n", node->TEST_nodeId, pos);
        if(node) return node->listPtr->get(pos);
		NOTFOUND(ET_NotFound_ListItem)
	}
	virtual int getSize() const {
        return listItems;
	}
    void _deleteOrder(IndexNode* node){
        IndexNode* tmp = node;
        while (tmp) {
            if(tmp->listItems==0 && tmp->father){
                //    printf("tmp1:  %d  del  %d\n", tmp->father->TEST_nodeId, tmp->TEST_nodeId);
                IndexNode* remove = tmp->father->new_remove(tmp);
                if(!remove)break;
//                PRINT_NODE(remove);
//                PRINT_NODE(tmp->_rootNode());
//                PRINT_NODE(tmp); 

                if(tmp->father->sonSize==0){
                    tmp = tmp->father; 
                }
                else
                    tmp = tmp->father;
                
                remove->Destroy();
                delete remove;
                // printf("tmp: %d\n", tmp->TEST_nodeId);
            }else
                tmp = tmp->father;
        }
        
    }
	bool remove(int pos) {
        IndexNode* node = _getNode(pos);
        if(node) {
       //     printf("remove: %d - %d", node->TEST_nodeId, pos);
			node->listPtr->remove(pos);
			node->listItem_AddSub(-1);
            if(node->listItems==0){
                _deleteOrder(node);
            }
            return true;
		}
        return false;
	}
	bool removeObj(const Object& object) { 
		return new_removeObj(object);
	}

	int indexOfObject(const Object& object) {
		return new_indexObj(object);
	}
};



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
////////////////                       ///////////////////////////////////
////////////////   ListDynamicHandle   ///////////////////////////////////
////////////////                       ///////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////



class ListDynamicHandle: public ListHandleBase {
public:
    IndexNode* node;
    
    ObjectType getType() {
		return OT_List;
    }
    
    virtual int sizeOf(){
        printf("Listsize: %ld %ld %ld %ld\n", sizeof(ListDynamicHandle),  sizeof(IndexNode),  sizeof(ListDyna),  sizeof(Object));
        printf("Listsize: %ld %ld %ld %ld\n", sizeof(this),  sizeof(this),  sizeof(this),  sizeof(this));
        return sizeof(ListDynamicHandle);
    }
    
    ListDynamicHandle() :
    ListHandleBase() {
        node  = new IndexNode();
    }
    
    void insert(const Object& obj, int pos) {
        node->insert(obj, pos);
    }
    
	int add(const Object& obj) {
        return node->add(obj);
    }
    
    void set(const Object& obj, int pos) {
        node->set(obj, pos);
    }
    
    virtual Object& get(int pos) const {
        return node->get(pos);
    }
    
    virtual int getSize() const {
        return node->getSize();
    }
    
//    Object& GET(int pos) {
//        return node->GET(pos);
//    }
//    
    bool remove(int pos) {
        return node->remove(pos);
    }
    
    bool removeObj(const Object& object) {
        return node->removeObj(object);
    }
    
    void destroy() {
		node->Destroy();
		delete node,node=NULL;
    }
    
    void reset() {
        destroy();
        node=new IndexNode;
    }
    
    int indexOfObject(const Object& object) {
        return node->indexOfObject(object);
    }
    
//    bool compare(Handle* handle) {
//        return true;
//    }
//    
    virtual String testInfo()const{
        return node->__Print().toString();
    }
    
    virtual String toString() {
      //  return node->Print().toString();
		//  return PRINT_ROOT;
        String str = String(toBytes().desc());
        return str;
    }
    
    Bytes toBytes(){
		//return toString().toBytes();
        Object object(this);
        List list(object);
        Bytes bytes = Util::tocJSONBytes(list);
        return bytes;
    }
    
//    int sizeOf(){return 0;};
//    void insert(const Object& obj, int pos){};
//	   int add(const Object& obj){return 0;};
//    void set(const Object& obj, int where){};
//	  Object& get(int where) const=0;
//	  int getSize() const{return 0;};
//	  bool remove( int pos){return 0;};
//	  bool removeObj(const Object& object){return 0;};
	//virtual void destroy()=0;
//	  void reset(){return 0;};
//      String testInfo()const{return 0;};
//	  List mid(int pos, int length){return 0;};
    virtual List mid(int pos, int length){
		List list;
		if(pos>=0 && length>0 && pos+length<=getSize()){
            for (int i = pos; i < pos+length; i++) {
                list.add(get(i));
            }
        }
      //  printf("dynMid: %d-%d  %d\n", pos, length, list.size());
		return list;
	}
    virtual bool compare(Handle* handle) {
            ListHandleBase* temp = (ListHandleBase*) handle;
            if(temp->getSize()!=temp->getSize())
                return false;
            int size = temp->getSize();
            for (int i = 0; i < size; i++) {
                if (get(i) != temp->get(i))
                    return false;
            }
            return true;
	}
	
//      int indexOfObject(const Object& object){return 0;};
//	//virtual bool compare(Handle* handle)=0;
//	  String toString(){return 0;};
//       Bytes toBytes(){return 0;};
    
};










List::List(int size ):
Object() {
	if(size)
		updateHandle(new ListHandle(size));
	else
        updateHandle(new ListDynamicHandle());
}

List::List(const Object& object) :
Object() {
	if (object.getType() == OT_List) {
		updateHandle(object.handle);
		incRef();
		return;
	}else{
        LEAVE(ET_TypeError_List);
        updateHandle(new ListHandle(KListInitSize));
    }
}

void List::set(const Object& obj, int index) {
	ListHandleBase* temp = (ListHandleBase*) handle;
    temp->set(obj, index);
}

Object& List::get(int index) const {
	ListHandleBase* temp = (ListHandleBase*) handle;
    return temp->get(index);
}

int List::add(const Object& obj) {
	ListHandleBase* temp = (ListHandleBase*) handle;
	return temp->add(obj);
}

void List::insert(const Object& obj, int index) {
	ListHandleBase* temp = (ListHandleBase*) handle;
    temp->insert(obj, index);
}

bool List::removeObj(const Object& obj) {
	ListHandleBase* temp = (ListHandleBase*) handle;
    return temp->removeObj(obj);
}

Object& List::operator[] (int index){
	return get(index);
}

int List::indexOfObject(const Object& object) {
	ListHandleBase* temp = (ListHandleBase*) handle;
    return temp->indexOfObject(object);
}

List List::excludeDuplicates() const {
	List list;
    for (int i = 0; i < size(); i++) {
        if (!list.contains(get(i)))
            list.add(get(i));
    }
	return list;
}

int List::size() const {
	ListHandleBase* temp = (ListHandleBase*) handle;
    return temp->getSize();
}

void List::reset() {
	ListHandleBase* temp = (ListHandleBase*) handle;
    temp->reset();
}

bool List::removeIndex(int index) {
	ListHandleBase* temp = (ListHandleBase*) handle;
    return temp->remove(index);
}

bool List::contains(const Object& object) {
    return indexOfObject(object)!=-1;
}

List List::mid(int pos, int length)const{
	ListHandleBase* temp = (ListHandleBase*) handle;
    return temp->mid(pos, length);
}
String List::testInfo()const{
    ListHandleBase* temp = (ListHandleBase*) handle;
    return temp->testInfo();
}

List List::operator+(const List& data) const {
	List list;
	for (int i = 0; i < size(); i++)
		list.add(get(i));
	
	for (int i = 0; i < data.size(); i++)
		list.add(data.get(i));
	
	return list;
}

void List::operator+=(const List& data) {
	*this = *this + data;
}

#ifdef __WINS__
void List::updateHandle(XV::Handle* value) {
	Object::updateHandle(value);
	internal = (ListHandleBase*) value;
}
#endif


class ListLinkHandle: public ListHandle {
public:
	List listA;
	List listB;
	ListLinkHandle(const List& alistA, const List& alistB) :
	ListHandle(0) {
		listA = alistA;
		listB = alistB;
	}
	virtual int getSize() const {
		return listA.size() + listB.size();
	}
	virtual Object& get(int where) const {
		if (where >= listA.size())
			return listB.get(where - listA.size());
		return listA.get(where);
	}
	bool compare(Handle* handle) {
		ListLinkHandle* temp = (ListLinkHandle*) handle;
		if(getSize()!=temp->getSize())
			return false;
		
		for (int i = 0; i < getSize(); i++) {
			if (get(i) != temp->get(i))
				return false;
		}
		
		return true;
	}
};

class ListLink: public Object {//this is list type
public:
	ListLink(const List& listA, const List& listB):Object() {
		updateHandle(new ListLinkHandle(listA, listB));
	}
};

List List::link(const List& listA, const List& listB){
    return List(ListLink(listA, listB));
};

