#include<stdlib.h>
#include<string.h>
#include"JsList.h"
#include"JsSys.h"
#include"JsDebug.h"
struct JsNodeHead{
	struct JsNode* first;
	int size;
};

struct JsNode{
	struct JsNode * next;
	void* data;
};

static void JsGcMarkNodeHead(void* mp, int ms);
static void JsGcMarkNode(void* mp, int ms);

JsList JsCreateList(){
	return (JsList)JsGcMalloc(sizeof(struct JsNodeHead),&JsGcMarkNodeHead,NULL);
}


void JsListPush(void* head0,void* data){
	int i;
	struct JsNode** npp;//指向最末端
	struct JsNode* np;
	struct JsNodeHead* head ;
	
	JsAssert(head0 != NULL);
	
	head = head0;
	npp = &head->first;
	for( i = 0 ; i < head->size; ++i){
		npp = &(*npp)->next;
	}
	np = JsGcMalloc(sizeof(struct JsNode),&JsGcMarkNode,NULL);
	np->data = data;
	np->next = NULL;
	(*npp) = np;
	(head)->size++;
}


void* JsListGet(void* head0,int pos){
	int i;
	struct JsNode* np;
	struct JsNodeHead* head;
	
	JsAssert(head0 != NULL);
	
	head =  head0;
	//没有空间或者超出范围
	if(head->size == 0||pos >= (head)->size)
		return NULL;
	if(pos  == JS_LIST_END){
		//指向最后一个
		pos = (head)->size - 1;
	}
	np = (head)->first;
	//np已经指向第一个node -> i = 1
	for( i = 0 ; i < pos ;++i){
		np = np->next;
	}
	return np->data;
}

int JsListSize(void* head0){
	struct JsNodeHead* head ;
	
	JsAssert(head0 != NULL);
	
	head = head0;
	return (head)->size;
}
int JsListRemove(void* head0,int pos){
	
	int i;
	struct JsNode* np;
	struct JsNodeHead* head ;
	
	JsAssert(head0 != NULL);
	
	head = head0;
	if( head->size == 0 || pos >= (head)->size)
		return FALSE;
	if(pos  == JS_LIST_END){
		//指向最后一个
		pos = (head)->size - 1;
	}
	(head)->size--;
	if(pos == 0){
		(head)->first = (head)->first->next;
		return TRUE;
	}
	np = (head)->first;
	for(i = 1 ; i < pos; ++i){
		np = np->next;
	}
	(np->next) = (np->next->next);
	
	return TRUE;
}

JsList JsListCopy(JsList dst,JsList src){
	int i;
	struct JsNode** npp1;
	struct JsNode** npp2;
	struct JsNode* np;
	struct JsNodeHead* head;
	struct JsNodeHead* p ;

	JsAssert(dst != NULL && src != NULL);
	head =  src;
	p = dst;
	//没有空间或者超出范围
	if(head->size == 0){
		p->size = 0;
		return p;
	}
	npp1 = &head->first;
	npp2 = &p->first;
	
	p->size = head->size;
	
	for( i = 0 ; i < head->size ;++i){
	
		np = JsGcMalloc(sizeof(struct JsNode),&JsGcMarkNode,NULL);
		np->data = (*npp1)->data;
		np->next = NULL;
		(*npp2) = np;
		
		npp1 = &(*npp1)->next;
		npp2 = &(*npp2)->next;
	}
	return p;

}


static void JsGcMarkNodeHead(void* mp, int ms){
	struct JsNodeHead* p = (struct JsNodeHead*) mp;
	JsGcMark(p->first);
}
static void JsGcMarkNode(void* mp, int ms){
	struct JsNode* p = (struct JsNode*)mp;
	JsGcMark(p->data);
	JsGcMark(p->next);
}
