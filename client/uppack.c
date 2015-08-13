/*******************************************************************************
 * Author	 : zhanggen
 * Email	 : zhanggen.jung@gmail.com
 * Last modified : 2015-08-12 18:12
 * Filename	 : uppack.c
 * Description	 : 将数据包分功能进行打包并发送数据
 * *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <ctype.h>
#include "message.h"

//发送私聊、群聊消息线程函数
void sendmessage(sendarg *arg)
{
	while(1){
		memset(arg->buf->data, 0, 1024);
		printf("请输入消息(输入!@#quit结束本次聊天):");
		setbuf(stdin,NULL);
		fgets(arg->buf->data, 1024, stdin);
		if (strcmp(arg->buf->data, "!@#quit\n") == 0){
			pthread_cancel(arg->recvthid);
			pthread_exit(0);
		}
		if (send(arg->sock, arg->buf, sizeof(datapack), 0) < 0){
			perror("消息发送失败");
		}
	}
}

//接收私聊、群聊消息线程函数
void recvmessage(int *sock)
{
	datapack buf;
	int len;
	while(1){
		memset(buf.data, 0, sizeof(datapack));
		len = recv(*sock, &buf, sizeof(datapack), 0);
		if (len < 0){
			perror("消息接收失败");
		}
		else if (len == 0){
			printf("与服务器连接断开\n");
			exit(-1);
		}
		else{
			printf("\n来自:%s(%d) ",buf.name,buf.source_id);
			printf("消息:%s\n",buf.data);
		}
	}
}

//输入密码显示**
char getch(void)
{
	char ch;
	system("stty -echo");
	system("stty -icanon");
	ch = getchar();
	system("stty icanon");
	system("stty echo");
	if (ch != '\n')
		printf("*");
	return ch;
}

//用户的登录,将数据按照格式打包后发送至服务器,并接收服务器回复
int lognin_sev(int sock)
{
	datapack buf;
	char temp;
	int i,count = 0,j,flag;

	strcpy(buf.flag, "logn in");
	buf.source_id = 0;
	buf.target_id = 0;

	for (count = 0; count < 3; count++){
		flag = 0;
		setbuf(stdin,NULL);
		printf("账号:");
		temp = getchar();
		for (i = 0; temp != '\n'; i++){
			buf.data[i] = temp;
			temp = getchar();
		}
		buf.data[i] = '\n';
		for (j = 0; buf.data[j] != '\n'; j++){
			if (!isdigit(buf.data[j])){
				flag = 1;
			}
		}
		if (flag == 1){
			printf("账号输入有误,账号应为纯数字,请检查后再登录\n");
			setbuf(stdin,NULL);
			continue;
		}

		setbuf(stdin,NULL);
		printf("密码:");
		temp = getch();
		for (i = i + 1; temp != '\n'; i++){
			buf.data[i] = temp + 1;
			temp = getch();
		}
		buf.data[i] = '\0';
		printf("\n");
		if (send(sock, &buf, sizeof(datapack), 0) < 0){
			printf("网络故障,登录失败\n");
			exit(-1);
		}
		if (recv(sock, &buf, sizeof(datapack), 0) <= 0){
			printf("网络故障,登录失败\n");
			exit(-1);
		}
		if (strcmp(buf.flag, "logn in yes") == 0){
			printf("%s", buf.data);
			return buf.source_id;
		}
		else{
			printf("%s", buf.data);
		}
	}
	printf("您的三次输入机会已到\n");
	exit(-1);
}

//用户的注册,将数据打包发送并接收服务器的回复
void signup_sev(int sock)
{
	char temp;
	datapack buf;
	int i,j;

	strcpy(buf.flag, "sign up");
	buf.source_id = 0;
	buf.target_id = 0;

	printf("请输入昵称：");
	setbuf(stdin, NULL);
	temp = getchar();
	for (i = 0; temp != '\n'; i++){
		buf.data[i] = temp;
		temp = getchar();
	}
	buf.data[i] = '\n';
	
	setbuf(stdin, NULL);
	printf("请输入密码：");
	temp = getch();
	for (i = i + 1; temp != '\n'; i++){
		buf.data[i] = temp + 1;
		temp = getch();
	}
	buf.data[i] = '\0';
	printf("\n");

	if (send(sock, &buf, sizeof(datapack), 0) < 0){
		printf("网络故障,注册失败\n");
		exit(-1);
	}
	if (recv(sock, &buf, sizeof(datapack), 0) <= 0){
		printf("网络故障,注册失败\n");
		exit(-1);
	}
	if (strcmp(buf.flag, "sign up yes") == 0){
		printf("%s", buf.data);
	}
	else{
		printf("%s", buf.data);
	}

	return ;
}

//私聊
void chatuser(datapack *buf,int sock)
{
	sendarg arg;
	pthread_t recvthid,sendthid;
	strcpy(buf->flag, "user");
	printf("请输入好友ID：");
	scanf("%u",&buf->target_id);
	setbuf(stdin, NULL);
	pthread_create(&recvthid, NULL, (void *)recvmessage, &sock);
	arg.sock = sock;
	arg.recvthid = recvthid;
	arg.buf = buf;
	pthread_create(&sendthid, NULL, (void *)sendmessage, &arg);
	pthread_join(recvthid, NULL);
	pthread_join(sendthid, NULL);
}
//群聊
void chatgroup(datapack *buf, int sock)
{
	sendarg arg;
	pthread_t recvthid,sendthid;
	strcpy(buf->flag, "group");
	printf("请输入群ID：");
	setbuf(stdin, NULL);
	scanf("%u",&buf->target_id);
	pthread_create(&recvthid, NULL, (void *)recvmessage, &sock);
	arg.sock = sock;
	arg.recvthid = recvthid;
	arg.buf = buf;
	pthread_create(&sendthid, NULL, (void *)sendmessage, &arg);
	pthread_join(recvthid, NULL);
	pthread_join(sendthid, NULL);
}
//加好友
void addnewfriend(datapack *buf, int sock)
{
	strcpy(buf->flag, "addfriend");
	printf("请输入对方ID：");
	setbuf(stdin, NULL);
	scanf("%d",&buf->target_id);

	if (send(sock, buf, sizeof(datapack), 0) < 0){
		printf("网络故障,加好友失败\n");
	}
	if (recv(sock, buf, sizeof(datapack), 0) <= 0){
		printf("网络故障,加好友失败\n");
	}
	else{
		printf("%s\n",buf->data);
	}
}
//建群
void bulidgroup(datapack *buf, int sock)
{
	strcpy(buf->flag, "bulidgroup");
	buf->target_id = 111111111;
	printf("请输入群名称:");
	setbuf(stdin, NULL);
	fgets(buf->data, 1024, stdin);

	if (send(sock, buf, sizeof(datapack), 0) < 0){
		printf("网络故障,建群失败\n");
	}
	if (recv(sock, buf, sizeof(datapack), 0) <= 0){
		printf("网络故障,建群失败\n");
	}
	else{
		printf("%s\n",buf->data);
	}
}
//加群
void joingroup(datapack *buf, int sock)
{
	printf("请输入群号码:");
	setbuf(stdin,NULL);
	scanf("%u",&(buf->target_id));
	strcpy(buf->flag, "addgroup");
	if (send(sock, buf, sizeof(datapack), 0) < 0){
		printf("网络故障,加群失败\n");
	}
	if (recv(sock, buf, sizeof(datapack), 0) <= 0){
		printf("网络故障,加群失败\n");
	}
	else{
		printf("%s\n",buf->data);
	}
}
//退群
void quitgroup(datapack *buf, int sock)
{
	printf("请输入群号码:");
	setbuf(stdin,NULL);
	scanf("%u",&(buf->target_id));
	strcpy(buf->flag, "quitgroup");
	if (send(sock, buf, sizeof(datapack), 0) < 0){
		printf("网络故障,退群失败\n");
	}
	if (recv(sock, buf, sizeof(datapack), 0) <= 0){
		printf("网络故障,退群失败\n");
	}
	else{
		printf("%s\n",buf->data);
	}
}
//解散群
//删除好友
//获取群成员列表
//获取好友列表
//获取在线好友列表
//修改好友备注
//修改群备注
