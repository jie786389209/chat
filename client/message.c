/*******************************************************************************
 * Author	 : zhanggen
 * Email	 : zhanggen.jung@gmail.com
 * Last modified : 2015-08-12 14:44
 * Filename	 : message.c
 * Description	 : 客户端消息的收发
 * *****************************************************************************/
#include <stdio.h> 
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include "message.h"

//发送数据包
void sendpack(arg *data)
{
	while(1){
		if (send(data->sock, data->buf, sizeof(datapack), 0) < 0){
			printf("发送失败,请检查网络是否正常=、=\n");
		}
	}
}

//接收数据包
void recvpack(arg *data)
{
	int len;

	while(1){
		len = recv(data->sock, data->buf, sizeof(datapack), 0);
		if (len > 0){
			//待补充
		}
		else if (len == 0){
			printf("网络连接失败=、=\n");
		}
		else{
			printf("服务器繁忙,请稍后重试=、=");
		}
	}
}


