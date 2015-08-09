/*******************************************************************************
 * Author	 : zhanggen
 * Email	 : zhanggen.jung@gmail.com
 * Last modified : 2015-08-08 17:01
 * Filename	 : logn.h
 * Description	 : 用户登录接口和数据结构
 * *****************************************************************************/

//用户信息数据结构
typedef struct userifo{
	char name[16];		//用户昵称
	char passwd[32];	//用户密码
	int id;				//用户ID
}userifo;

//群组信息数据结构
typedef struct groupifo{
	char name[32];		//群组名称
	int userid;			//所属者ID
	int groupid;		//群组ID
}groupifo;

//注册账号
int signup_sev(char name[], char passwd[]);
//创建群
int buildgroup_sev(int userid, char name[]);

