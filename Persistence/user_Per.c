/*******************************************************************************
 * Author	 : zhanggen
 * Email	 : zhanggen.jung@gmail.com
 * Last modified : 2015-08-10 10:09
 * Filename	 : user_Per.c
 * Description	 : 用户相关操作的文件操作
 * *****************************************************************************/
#include <string.h>
#include "../Common/common.h"
#include "../Server/user_sev.h"
#include <stdio.h>
#include <unistd.h>

static const char USER_FILE[] = "user.dat";

//将新注册用户的信息保存进文件,并建立该账号的列表文件
int  save_userifo_Per(userifo *user)
{
	char *path = get_pwd();
	char id[25];
	char cmd[25];
	FILE *fp;

	if (chdir("./data") == -1){
		system("mkdir data");
		if (chdir("./data") == -1){
			rerurn 0;
		}
	}
	if ((fp = fopen(USER_FILE, "ab+")) == NULL){
		chdir(path);
		return 0;
	}
	if (fwrite(user, sizeof(userifo), 1, fp) == 0){
		fclose(fp);
		chdir(path);
		return 0;
	}

	strcpy(cmd, "touch ");
	sprintf(id, "%d", user->id);
	strcat(id, "_list.dat");
	strcat(cmd, id);
	system(cmd);
	fclose(fp);
	chdir(path);

	return 1;
}

//从文件里通过ID获取账号信息
int selectuserid_Per(unsigned int userid, userifo *user)
{
	FILE *fp;
	char *path = get_pwd();

	if (chdir("./data") == -1){
		system("mkdir data");
		if (chdir("./data") == -1){
			return -1;
		}	
	}
	if ((fp = fopen(USER_FILE, "rb")) == NULL){
		chdir(path);
		return -1;
	}
	while(!feof(fp)){
		if (fread(user, sizeof(userifo), 1, fp)){
			if (user->id == userid){
				fclose(fp);
				chdir(path);
				return 1;
			}
		}
	}
	fclose(fp);
	chdir(path);
	return 0;
}

//向userid所指的列表文件中添加friendid为好友

