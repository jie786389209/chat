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
#include <time.h>
#include <unistd.h>

//获取当前系统时间
void date_now(char now_time[])
{
	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	strcpy (now_time,asctime (timeinfo));
	now_time[24] = '\0';
}

//添加聊天记录
void addhistory(unsigned int userid, datapack *buf, char time[])
{
	char *path = getcwd(NULL,0);
	char id[32];
	FILE *fp;
	history mag;
	mag.buf = *buf;
	strcpy(mag.time, time);
	if (chdir("./data") == -1){
		system("mkdir data");
		if (chdir("./data") == -1){
			return ;
		}
	}
	
	sprintf(id, "%d", userid);
	strcat(id, "_history.dat");
	if ((fp = fopen(id, "ab+")) == NULL){
		chdir(path);
		return ;
	}

	fwrite(&mag, sizeof(history), 1, fp);
	fclose(fp);
	chdir(path);
}

//提取聊天记录
void selecthistory(unsigned int userid)
{
	char *path = getcwd(NULL,0);
	char id[32];
	int flag = 0;
	FILE *fp;
	history mag;
	char name[32];
	unsigned int targetid;
	
	printf("请输入要查找的好友ID：");
	setbuf(stdin, NULL);
	scanf("%d",&targetid);
	if (chdir("./data") == -1){
		system("mkdir data");
		if (chdir("./data") == -1){
			return ;
		}
	}
	
	sprintf(id, "%d", userid);
	strcat(id, "_history.dat");
	if ((fp = fopen(id, "rb")) == NULL){
		printf("暂无聊天记录\n");
		chdir(path);
		return ;
	}

	while(!feof(fp)){
		if (fread(&mag, sizeof(history), 1, fp)){
			if (mag.buf.source_id == userid){
				printf("时间：%s\n",mag.time);
				printf("我对(%d)说：%s\n", targetid, mag.buf.data);
			}
			else if (mag.buf.source_id == targetid){
				printf("时间：%s\n",mag.time);
				printf("%s(%d)对我说：%s\n",mag.buf.name, targetid, mag.buf.data);
			}
		}
	}
	
	fclose(fp);
	chdir(path);
}

//发送私聊、群聊消息线程函数
void sendmessage(sendarg *arg)
{
	char time[25];
	date_now(time);
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
		else{
			date_now(time);
			if (strcmp(arg->buf->flag, "user") == 0){
				addhistory(arg->buf->source_id, arg->buf, time);
			}
		}
	}
}

//接收私聊、群聊消息线程函数
void recvmessage(int *sock)
{
	datapack buf;
	int len;
	char time[25];
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
			date_now(time);
			printf("\n时间：%s\n来自:%s(%d) ",time,buf.name,buf.source_id);
			printf("消息:%s",buf.data);
			if ((strcmp(buf.flag, "user") == 0) && (strcmp(buf.flag, "yes") == 0)){
				addhistory(buf.target_id, &buf, time);
			}
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
		printf("\t\t账号:");
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
			printf("\n\t\t\t\t\t账号输入有误,账号应为纯数字,请检查后再登录\n");
			setbuf(stdin,NULL);
			continue;
		}

		setbuf(stdin,NULL);
		printf("\n\t\t密码:");
		temp = getch();
		for (i = i + 1; temp != '\n'; i++){
			buf.data[i] = temp + 1;
			temp = getch();
		}
		buf.data[i] = '\0';
		printf("\n");
		if (send(sock, &buf, sizeof(datapack), 0) < 0){
			printf("\n\t\t\t网络故障,登录失败\n");
			exit(-1);
		}
		if (recv(sock, &buf, sizeof(datapack), 0) <= 0){
			printf("\n\t\t\t网络故障,登录失败\n");
			exit(-1);
		}
		if (strcmp(buf.flag, "logn in yes") == 0){
			printf("\n\033[0;31m\t\t\t\t\t%s\003[0m", buf.data);
			return buf.source_id;
		}
		else{
			printf("\n\t\t\t\t\t\033[0;32m%s\033[0m", buf.data);
		}
	}
	printf("\n\t\t\t\t\t您的三次输入机会已到\n");
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
void delgroup(datapack *buf, int sock)
{
	strcpy(buf->flag, "delgroup");
	printf("请输入群号码:");
	setbuf(stdin,NULL);
	scanf("%u",&(buf->target_id));
	
	if (send(sock, buf, sizeof(datapack), 0) < 0){
		printf("网络故障,解散群失败\n");
	}
	if (recv(sock, buf, sizeof(datapack), 0) <= 0){
		printf("网络故障,解散群失败\n");
	}
	else{
		printf("%s\n",buf->data);
	}
}
//删除好友
void delfriend(datapack *buf, int sock)
{
	strcpy(buf->flag, "delfriend");
	printf("请输入好友ID:");
	setbuf(stdin,NULL);
	scanf("%u",&(buf->target_id));
	
	if (send(sock, buf, sizeof(datapack), 0) < 0){
		printf("网络故障,删除好友失败\n");
	}
	if (recv(sock, buf, sizeof(datapack), 0) <= 0){
		printf("网络故障,删除好友失败\n");
	}
	else{
		printf("%s\n",buf->data);
	}
}
//获取好友列表
void selcetfriend(datapack *buf, int sock)
{
	char temp[1024];
	int len;
	FILE *fp;
	userlist tempdata;
	strcpy(buf->flag, "selcetfriend");
	buf->target_id = 1111111;
	if ((fp = fopen("temp.dat","wb")) == NULL){
		printf("系统故障，获取失败=、=");
		return ;
	}
	if (send(sock, buf, sizeof(datapack), 0) < 0){
		printf("网络故障,获取好友列表失败\n");
	}
	memset(temp, 0, 1024);
	if ((len = recv(sock, temp, 1024, 0)) <= 0){
		printf("网络故障,获取好友列表失败\n");
	}
	fwrite(temp, len, 1, fp);
	fclose(fp);
	if ((fp = fopen("temp.dat","rb")) == NULL){
		printf("系统故障，获取失败=、=");
		remove("temp.dat");
		return ;
	}
	while(!feof(fp)){
		if (fread(&tempdata, sizeof(userlist), 1, fp)){
			printf("%d\t\t",tempdata.id);
			printf("%s\t\t\t",tempdata.name);
			if (tempdata.type == 0)
				printf("(好友)\n");
			else
				printf("(群组)\n");
		}
	}
	
	fclose(fp);
	remove("temp.dat");
}
//获取群成员列表
void selcetgroup(datapack *buf, int sock)
{
	char temp[1024];
	int len;
	FILE *fp;
	userlist tempdata;
	strcpy(buf->flag, "selectgroup");
	printf("请输入待获取群号：");
	setbuf(stdin,NULL);
	scanf("%d",&buf->target_id);
	printf("群号：%d\n",buf->target_id);
	if ((fp = fopen("temp.dat","wb")) == NULL){
		printf("系统故障，获取失败=、=");
		return ;
	}
	printf("群号：%d\n",buf->target_id);
	if (send(sock, buf, sizeof(datapack), 0) < 0){
		printf("网络故障,获取群成员列表失败\n");
	}
	memset(temp, 0, 1024);
	if ((len = recv(sock, temp, 1024, 0)) <= 0){
		printf("网络故障,获取群成员列表失败\n");
	}
	if (strcmp(temp, "系统繁忙,请稍后重试=、=") == 0){
		printf("系统繁忙,请稍后重试=、=\n");
		fclose(fp);
		remove("temp.dat");
		return ;
	}
	fwrite(temp, len, 1, fp);
	fclose(fp);
	if ((fp = fopen("temp.dat","rb")) == NULL){
		printf("系统故障，获取失败=、=");
		remove("temp.dat");
		return ;
	}
	while(!feof(fp)){
		if (fread(&tempdata, sizeof(userlist), 1, fp)){
			printf("%d\t\t",tempdata.id);
			printf("%s\n",tempdata.name);
		}
	}
	
	fclose(fp);
	remove("temp.dat");
}
//获取在线好友列表
void selcetonlie(datapack *buf, int sock)
{
	int flag = 0;
	strcpy(buf->flag, "onlinefriend");
	buf->target_id = 1111111;
	if (send(sock, buf, sizeof(datapack), 0) < 0){
		printf("网络故障,获取列表失败\n");
	}
	while(1){
		if (recv(sock, buf, sizeof(datapack), 0) <= 0){
			printf("网络故障,获取列表失败\n");
		}
		else{
			if (buf->target_id == 0){
				if (flag == 0)
					printf("暂无好友在线~~\n");
				return ;
			}
			else{
				flag++;
				printf("%s\n",buf->data);
			}
		}
	}
}
//修改好友备注
void renamefriend(datapack *buf, int sock)
{
	char temp;
	int i;
	strcpy(buf->flag, "renamefriend");
	printf("请输入好友待修改备注的ID:");
	setbuf(stdin,NULL);
	scanf("%u",&(buf->target_id));
	printf("请输入好友的新备注：");
	setbuf(stdin, NULL);
	temp = getchar();
	for (i = 0; temp != '\n'; i++){
		buf->data[i] = temp;
		temp = getchar();
	}
	buf->data[i] = '\0';
	printf("%s\n",buf->flag);
	printf("%s\n",buf->data);
	if (send(sock, buf, sizeof(datapack), 0) < 0){
		printf("网络故障,修改失败\n");
	}
	if (recv(sock, buf, sizeof(datapack), 0) <= 0){
		printf("网络故障,修改失败\n");
	}
	else{
		printf("%s\n",buf->data);
	}
}
//修改群备注
void renamgroup(datapack *buf, int sock)
{
	char temp;
	int i;
	strcpy(buf->flag, "renamgroup");
	printf("请输入待修改群名片的群ID:");
	setbuf(stdin,NULL);
	scanf("%u",&(buf->target_id));
	printf("请输入新的群备注：");
	setbuf(stdin, NULL);
	temp = getchar();
	for (i = 0; temp != '\n'; i++){
		buf->data[i] = temp;
		temp = getchar();
	}
	buf->data[i] = '\0';
	printf("%s\n",buf->flag);
	printf("%s\n",buf->data);
	if (send(sock, buf, sizeof(datapack), 0) < 0){
		printf("网络故障,修改失败\n");
	}
	if (recv(sock, buf, sizeof(datapack), 0) <= 0){
		printf("网络故障,修改失败\n");
	}
	else{
		printf("%s\n",buf->data);
	}
}
