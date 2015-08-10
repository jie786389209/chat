/*******************************************************************************
 * Author	 : zhanggen
 * Email	 : zhanggen.jung@gmail.com
 * Last modified : 2015-08-10 10:25
 * Filename	 : user_Per.h
 * Description	 : 用户相关文件操作的函数声明
 * *****************************************************************************/
#include "../Server/user_sev.h"

#ifndef USER_PER_H_
#define USER_PER_H_

//将新注册用户的信息保存进文件,并建立该账号的列表文件
int  save_userifo_Per(userifo *user);

#endif
