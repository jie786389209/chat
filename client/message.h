/*******************************************************************************
 * Author	 : zhanggen
 * Email	 : zhanggen.jung@gmail.com
 * Last modified : 2015-08-12 14:59
 * Filename	 : message.h
 * Description	 : 数据包定义,函数声明
 *******************************************************************************/
#ifndef MESSAGE_H_
#define MESSAGE_H_

//数据包定义
typedef struct datapack{
	char flag[12];
	char data[1024];
	char name[64];		//发的人的昵称或备注
	unsigned int source_id;
	unsigned int target_id;
}datapack;

//发送线程函数参数定义
typedef struct sendarg{
	int sock;
	int recvthid;
	datapack *buf;
}sendarg;

//接收线程函数参数定义
typedef struct recvarg{
	int sock;
	int recvthid;
	int sendthid;
}recvarg;

//输入不回显
char getch(void);

#endif
