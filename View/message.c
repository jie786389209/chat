/*******************************************************************************
 * Author	 : zhanggen
 * Email	 : zhanggen.jung@gmail.com
 * Last modified : 2015-08-11 17:26
 * Filename	 : message.c
 * Description	 : 数据包的解析及执行相关命令
 * *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "message.h"
#include "../Common/common.h"
#include "../Common/list.h"
#include "../Server/group_sev.h"
#include "../Server/id_sev.h"
#include "../Server/logn_sev.h"
#include "../Server/log_sev.h"
#include "../Server/user_sev.h"

//数据包的解析,并执行相应的命令,传给客户端
int analyzedatapack(datapack *buf, int sock, char ip[], onlinelist *pHead)
{
	//源ID和目标如果为都0,flag为sign up时表示注册用户
	if ((buf->source_id == 0) && (buf->target_id == 0) && (strcmp(buf->flag, "sign up") == 0)){
		int i = 0,j = 0;
		char name[16];
		char passwd[32];
		unsigned int userid;
		for (i = 0; buf->data[i] != '\n'; i++){
			name[i] = buf->data[i];
		}
		i++;
		while(buf->data[i] != '\0'){
			passwd[j] = buf->data[i] - 1;
		}
		if ((userid = signup_sev(name, passwd)) > 0){
			strcpy(buf->data,"恭喜您注册成功,您的账号为:");
			sprintf(name, "%u", userid);
			strcat(buf->data, name);
			strcat(buf->data, "\n请您牢记您的账号和密码(打死也不要将密码告诉别人呦~~)\n");
			strcpy(buf->flag, "yes");
		}
		else{
			strcpy(buf->data, "亲~~现在系统繁忙,请您稍后重试~~\n");
			strcpy(buf->flag, "no");
		}
	}	//登录
	else if ((buf->source_id == 0) && (buf->target_id == 0) && (strcmp(buf->flag, "logn in") == 0)){
		int i = 0,j = 0,userid;
		char user[16];
		char passwd[32];
		unsigned int userid;
		for (i = 0; buf->data[i] != '\n'; i++){
			user[i] = buf->data[i];
		}
		i++;
		while(buf->data[i] != '\0'){
			passwd[j] = buf->data[i] - 1;
		}
		userid = atoi(user);
		if (userlognin_sev(userid, passwd, ip) == 0){
			strcpy(buf->flag, "yes");
			strcpy(buf->data, "亲~~您的账号或者密码错了呦~~\n");
		}
		else{
			strcpy(buf->data, "登陆成功,正在努力跳转~~\n");
			strcpy(buf->flag, "yes");
		}
	}	//私聊
	else if ((buf->sourece_id > 0) && (buf->target_id > 0) && (strcmp(buf->flag, "user") == 0)){
		int flag = 0;
		userlist my_friend;
		onlinelist *pTemp = pHead->pNext->pNext;
		if (finduser_sev(buf->source_id, buf->target_id, 0, &my_friend) == 0){
			strcpy(buf->data, "您输入的用户暂时还不是您的好友哦~~")
		}
		else{
			if (finduser_sev(buf->target_id, buf->source_id, 0, &my_friend)){
				strcpy(buf->name, my_friend.name);
			}
			while(pTemp != NULL){
				if (pTemp->id == buf->target_id){
					sock = buf->target_id;
					flag = 1;
				}
			}
			if (flag == 0){
				strcpy(buf->data, "您发送的用户不在线,请您稍后重试~~");
			}
		}
	}	//群聊
	else if ((buf->sourece_id > 0) && (buf->target_id > 0) && (strcmp(buf->flag, "group") == 0)){
		
		userlist my_friend;
		onlinelist *pTemp = pHead->pNext->pNext;
		if (finduser_sev(buf->source_id, buf->target_id, 1, &my_friend) == 0){
			strcpy(buf->data, "您暂时还未加此群哦~~")
		}
		else{
			if (finduser_sev(buf->target_id, buf->source_id, 0, &my_friend)){
				strcpy(buf->name, my_friend.name);
			}
			while(pTemp != NULL){
				//待补充
			}
		}
	}
	send(sock, buf, sizeof(datapack), 0);
}
