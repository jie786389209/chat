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
			return 0;
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

//向userid所指的列表文件中添加好友或群组
int add_friend_Per(unsigned int userid, userlist *newfriend)
{
	char *path = get_pwd();
	char id[25];
	FILE *fp;

	if (chdir("./data") == -1){
		system("mkdir data");
		if (chdir("./data") == -1){
			return 0;
		}
	}
	
	sprintf(id, "%d", userid);
	strcat(id, "_list.dat");
	if ((fp = fopen(id, "ab+")) == NULL){
		chdir(path);
		return 0;
	}

	if (fwrite(newfriend, sizeof(userlist), 1, fp) == 0){
		fclose(fp);
		chdir(path);
		return 0;
	}

	return 1;
}

//删除userid列表文件中的friendid好友或群组
int del_friend_Per(unsigned int friendid, unsigned int userid)
{
	char *path = get_pwd();
	char id[25],temp[30];
	FILE *fp1, *fp;
	userlist user;
	int flag = 0;

	if (chdir("./data") == -1){
		system("mkdir data");
		if (chdir("./data") == -1){
			return 0;
		}
	}
	
	sprintf(id, "%d", userid);
	strcat(id, "_list.dat");
	strcpy(temp, id);
	strcat(temp, "_temp");
	rename(temp, id);
	if ((fp = fopen(id, "wb")) == NULL){
		chdir(path);
		return -1;
	}
	if ((fp1 = fopen(temp, "rb")) == NULL){
		chdir(path);
		return -1;
	}
	while(!feof(fp1)){
		if (fread(&user, sizeof(userlist), 1, fp1)){
			if (user.id == friendid){
				flag = 1;
				continue;
			}
			fwrite(&user, sizeof(userlist), 1, fp);
		}
	}
	remove(temp);
	chdir(path);
	if (flag == 0)
		return 0;

	return 1;
}

//修改好友备注
int update_friend_Per(unsigned int userid, unsigned int friendid, char name[], int type)
{
	char *path = get_pwd();
	char id[25];
	int flag = 0;
	userlist user;
	FILE *fp;

	if (chdir("./data") == -1){
		system("mkdir data");
		if (chdir("./data") == -1){
			return 0;
		}
	}
	sprintf(id, "%d", userid);
	strcat(id, "_list.dat");
	if ((fp = fopen(id, "rb+")) == NULL){
		chdir(path);
		return -1;
	}
	while(!feof(fp)){
		if (fread(&user, sizeof(userlist), 1, fp)){
			if ((user.id == friendid) && (user.type == type)){
				strcpy(user.name, name);
				fseek(fp, -sizeof(userlist), SEEK_CUR);
				fwrite(&user, sizeof(userlist), 1, fp);
				flag = 1;
				break;
			}
		}
	}
	fclose(fp);
	chdir(path);
	if (flag == 0)
		return 0;

	return 1;	
}
