/*******************************************************************************
 * Author	 : zhanggen
 * Email	 : zhanggen.jung@gmail.com
 * Last modified : 2015-08-10 10:04
 * Filename	 : user_sev.h
 * Description	 : 用户数据结构和函数声明
 * *****************************************************************************/
#ifndef USER_SEV_H_
#define USER_SEV_H_

//用户信息数据结构
typedef struct userifo{
	char name[16];		//用户昵称
	char passwd[32];	//用户密码
	unsigned int id;				//用户ID
}userifo;

//注册账号
unsigned int signup_sev(char name[], char passwd[]);

#endif
