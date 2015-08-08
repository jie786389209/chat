/*******************************************************************************
 * Author	 : zhanggen
 * Email	 : zhanggen.jung@gmail.com
 * Last modified : 2015-08-07 13:54
 * Filename	 : list.h
 * Description	 : 链表相关操作
 * *****************************************************************************/
#include <stdlib.h>

//创建一个链表,传进来头指针和结点类型
#define list_creat(pHead, node_type){						\
	pHead = (node_type *)malloc(sizeof(node_type));	\
	(pHead)->pNext = NULL;							\
}

//尾插一个结点,传进来头结点和新结点
#define add_tial(pHead,new_node, node_type){	\
	node_type *pTemp = pHead;				\
	while(pTemp ->pNext != NULL)	\
		pTemp = pTemp->pNext;		\
	(pTemp)->pNext = new_node;		\
	(new_node)->pNext = NULL;			\
}

//删除一个结点,传进来头结点和待删除结点,结点类型
#define del_node(pHead,node,node_type){		\
	node_type *pTemp;		\
	node_type *pTar;		\
	pTemp = pHead;			\
	while(pTemp->pNext != node)	\
		pTemp = pTemp->pNext;	\
	pTar = pTemp->pNext;	\
	pTemp->pNext = pTar->pNext;	\
	free(pTar);		\
}

//释放链表
#define list_free(pHead, node_type){	\
	node_type *pTemp1;	\
	node_type *pTemp2;	\
	pTemp2 = pTemp1 = pHead->pNext;		\
	while(pTemp1 != NULL){			\
		pTemp1 = pTemp2->pNext;	\
		free(pTemp2);	\
		pTemp2 = pTemp1;	\
	}\
	pHead->pNext = NULL;	\
}

//销毁链表
#define list_destroy(pHead, node_type){	\
	list_free(pHead, node_type);	\
	free(pHead);	\
}
