/*******************************************************************************
 * Author	 : zhanggen
 * Email	 : zhanggen.jung@gmail.com
 * Last modified : 2015-08-08 16:22
 * Filename	 : logn.c
 * Description	 : 登录和注册用户,创建群、解散群接口
 * *****************************************************************************/
#include "log_sev.h"
#include "logn.h"
#include <string.h>

//注册账号
int signup_sev(char name[], char passwd[])
{
	userifo user;
	char *info;
	int userid = sev_distri_userid();
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
	return userid;
}

//创建群
int buildgroup_sev(int userid, char name[])
{
	groupifo group;
	char *info;
	int groupid = sev_distri_groupid();
	if (groupid == 0){
		info = "The new group can not distribution group id";
		add_errorlog_sev(info);
		return 0;
	}
	//将传进来的群名称和所属者ID存进结构体并保存进文件
	group.userid = userid;
	group.groupid = groupid;
	strcpy(group.name, name);
	if (save_groupifo_Per(&group) == 0){
		info = "The new group can not save info";
		add_errorlog_sev(info);
		return 0;
	}

	return groupid;
}

//解散群
inline int delgroup_sev(int groupid)
{
	char *ifo;
	if (delgroup_Per(groupid) == 0){
		ifo = "Can not del the group form grouplist.dat";
		add_errorlog_sev(ifo);
		return 0;
	}
	return 1;
}

//登录接口
int userlogn_sev()
