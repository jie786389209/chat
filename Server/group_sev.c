/*******************************************************************************
 * Author	 : zhanggen
 * Email	 : zhanggen.jung@gmail.com
 * Last modified : 2015-08-10 09:32
 * Filename	 : group_sev.c
 * Description	 : 创建群、查找群、加群、退群、解散群操作
 * *****************************************************************************/
#include <string.h>
#include "log_sev.h"
#include "../Persistence/group_Per.h"
#include <stdio.h>

//创建群
unsigned int buildgroup_sev(unsigned int userid, char name[])
{
	groupifo group;
	char *info;
	unsigned int groupid = sev_distri_groupid();
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
inline int delgroup_sev(unsigned int groupid)
{
	char *ifo;
	if (delgroup_Per(groupid) == 0){
		ifo = "Can not del the group form grouplist.dat";
		add_errorlog_sev(ifo);
		return 0;
	}
	return 1;
}
