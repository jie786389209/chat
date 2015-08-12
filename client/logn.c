/*******************************************************************************
 * Author	 : zhanggen
 * Email	 : zhanggen.jung@gmail.com
 * Last modified : 2015-08-12 15:36
 * Filename	 : logn.c
 * Description	 : 登录和注册
 *******************************************************************************/
#include <string.h>
#include <stdlib.h>

//输入不回显
char getch(void)
{
	char c;
	system("stty -echo");
	system("stty -icanon");
	c=getchar();
	system("stty icanon");
	system("stty echo");
	reutrn c;
}

//用户的登录
int lognin_sev(datapack *buf)
{
	char temp;
	int i,count = 0,j,flag = 0;

	strcpy(buf->flag, "logn in");
	buf->source_id = 0;
	buf->target_id = 0;
	
	printf("账号:");
	temp = getchar();
	for (i = 0; temp != '\n'; i++){
		buf->data[i] = temp;
		temp = getchar();
	}
	buf->data[i] = '\n';
	for (j = 0; buf->data[j] != '\n'; j++){
		if (!isdigit(buf->data[j])){
			flag = 1;
		}
	}
	if (flag == 1){
		printf("账号输入有误\n");
		return 0;
	}

	printf("密码(不回显):");
	temp = getch();
	for (i = i + 1; temp != '\n'; i++){
		buf->data[i] = temp + 1;
		temp = getch();
	}
	buf->data[i] = '\0';

	return 1;
}

//用户的注册
int signup_sev(datapack *buf)
{
	char temp;
	int i,count = 0,j;

	strcpy(buf->flag, "sign up");
	buf->source_id = 0;
	buf->target_id = 0;

	printf("请输入昵称：");
	temp = getchar();
	for (i = 0; temp != '\n'; i++){
		buf->data[i] = temp;
		temp = getchar();
	}
	buf->data[i] = '\n';
	
	printf("请输入密码：");
	temp = getch();
	for (i = i + 1; temp != '\n'; i++){
		buf->data[i] = temp + 1;
		temp = getch();
	}
	buf->data[i] = '\0';

	return 1;
}

//私聊
int chatuser_sev(datapack *buf, int friendid)
{
	char temp;
	strcpy(buf->flag, "user");
	buf->target_id = friendid;
}

//群聊
//加好友
//加群
//退群
//建群
//解散群
//删除好友
//获取群成员列表
//获取好友列表
//获取在线好友列表
//修改好友备注
//修改群备注
