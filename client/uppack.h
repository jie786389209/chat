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
void chatuser(datapack *buf, int friendid, int sock);

//群聊
void chatgroup(datapack *buf, int groupid, int sock);
