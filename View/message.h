/*******************************************************************************
 * Author	 : zhanggen
 * Email	 : zhanggen.jung@gmail.com
 * Last modified : 2015-08-11 14:39
 * Filename	 : message.h
 * Description	 : 数据包和在线链表数据结构的定义,及相关操作函数声明
 * *****************************************************************************/

#ifndef MESSAGE_H_
#define MESSAGE_H_

//在线用户链表
typedef struct onlinelist{
	char ip[16];				//IP地址
	int sock;					//用户的套接字描述符
	unsigned int id;			//用户的ID
	struct onlinelist *pNext;	//指针域
}onlinelist;

//数据包定义
typedef struct datapack{
	char flag[12];
	char data[1024];
	char name[32];
	unsigned int source_id;
	unsigned int target_id;
}datapack;

#endif
