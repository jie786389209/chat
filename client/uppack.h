/*******************************************************************************
 * Author	 : zhanggen
 * Email	 : zhanggen.jung@gmail.com
 * Last modified : 2015-08-13 09:02
 * Filename	 : uppack.h
 * Description	 :消息数据包的打包
 * *****************************************************************************/
#ifndef UPPACK_H_
#define UPPACK_H_

#include "message.h"

//发送私聊、群聊消息线程函数
void sendmessage(sendarg *arg);

//接收私聊、群聊消息线程函数
void recvmessage(int *sock);

//输入不回显
char getch(void);

//用户的登录,将数据按照格式打包后发送至服务器,并接收服务器回复
int lognin_sev(int sock);

//用户的注册,将数据打包发送并接收服务器的回复
void signup_sev(int sock);

//私聊
void chatuser(datapack *buf, int sock);

//群聊
void chatgroup(datapack *buf, int sock);

#endif
