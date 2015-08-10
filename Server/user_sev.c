/*******************************************************************************
 * Author	 : zhanggen
 * Email	 : zhanggen.jung@gmail.com
 * Last modified : 2015-08-10 09:53
 * Filename	 : user_sev.c
 * Description	 : 用户的注册、加好友、删除好友、修改好友、修改群名称操作
 * *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include "../Persistence/user_Per.h"
#include "log_sev.h"

//注册账号
unsigned int signup_sev(char name[], char passwd[])
{
	userifo user;
	char info[80];
	char a[20];
	unsigned int userid = sev_distri_userid();
	if (userid == 0){
		info = "The new user can not distribution user id";
		add_errorlog_sev(info);
		return 0;
	}
	//将传进来的昵称和密码存进结构体并保存进文件
	user.id = userid;
	strcpy(user.name, name);
	strcpy(user.passwd, passwd);
	if (save_userifo_Per(&user) == 0){
		info = "The new user can not save info";
		add_errorlog_sev(info);
		return 0;
	}
	ifo = "The new user sign up   ";
	sprintf(a, "%d", userid);
	strcat(ifo, a);
	add_syslog_sev(ifo);
	return userid;
}
