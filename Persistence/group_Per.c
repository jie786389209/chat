/*******************************************************************************
 * Author	 : zhanggen
 * Email	 : zhanggen.jung@gmail.com
 * Last modified : 2015-08-10 09:39
 * Filename	 : group_Per.c
 * Description	 : 群操作的相关文件操作
 * *****************************************************************************/
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "../Common/common.h"
#include "../Server/group_sev.h"
#include "group_Per.h"

static const char GROUP_FILE[] = "group.dat";
static const char GROUP_FILE_TEMP[] = "grouptemp.dat";

//保存新创建群进文件,并创建群列表文件
int save_groupifo_Per(groupifo *group)
{
	char *path = get_pwd();
	FILE *fp;
	char id[25];
	char cmd[25];

	if (chdir("./data") == -1)
		system("mkdir data");
	if ((fp = fopen(GROUP_FILE, "ab+")) == NULL){
		chdir(path);
		return 0;
	}
	if (fwrite(group, sizeof(groupifo), 1, fp) == 0)
		return 0;
	
	strcpy(cmd, "touch ");
	sprintf(id, "%d", group->groupid);
	strcat(id, "_list.dat");
	strcat(cmd, id);
	system(cmd);
	fclose(fp);
	chdir(path);

	return 1;
}

//解散群
int delgroup_Per(unsigned int groupid)
{
	FILE *fp, *fp1;
	char *path = get_pwd();
	groupifo group;
	
	//删除群列表文件中的群信息
	if (chdir("./data") == -1){
		system("mkdir data");
	}
	if (rename(GROUP_FILE, GROUP_FILE_TEMP) < 0){
		chdir(path);
		return 0;
	}
	if ((fp = fopen(GROUP_FILE, "wb")) == NULL){
		chdir(path);
		return 0;
	}
	if ((fp1 = fopen(GROUP_FILE_TEMP, "rb")) == NULL){
		chdir(path);
		return 0;
	}
	while(!feof(fp1)){
		if (fread(&group, sizeof(groupifo), 1, fp1)){
			if (group.groupid == groupid){
				continue;
			}
			fwrite(&group, sizeof(groupifo), 1, fp);
		}
	}
	fclose(fp);
	fclose(fp1);
	chdir(path);
	remove(GROUP_FILE_TEMP);

	return 1;
}
