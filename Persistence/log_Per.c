/*******************************************************************************
 * Author	 : zhanggen
 * Email	 : zhanggen.jung@gmail.com
 * Last modified : 2015-08-08 14:32
 * Filename	 : log_Per.c
 * Description	 : 日志相关的写入文件操作
 * *****************************************************************************/
#include <stdio.h>
#include "../Common/common.h"
static const char ERROR_FILE[] = "chat_errno.log";

//添加错误日志
int add_errorlog_Per(char ifo[], char time[])
{
	FILE *fp;
	char *path = get_pwd();
	if (chdir("/var/log/") == -1)
		return 0;
	fp = fopen(ERROR_FILE, "a+");
	if (fp == NULL)
		return 0;
	fprintf(fp,"%s  %s\n",ifo,time);
	fclose(fp);
	chdir(path);
	return 1;
}
