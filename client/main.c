#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include "uppack.h"

int main(void)
{
	int sock;
	int key; //选项
	datapack buf; //数据包
	struct sockaddr_in addr;
	sock = socket(AF_INET,SOCK_STREAM,0);
	if (sock < 0){
		perror("Socket Error");	return EXIT_FAILURE;
	}
	addr.sin_family = AF_INET;	//IPv4
	addr.sin_port = htons(2489);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(addr.sin_zero,0,sizeof(addr.sin_zero));	//连接
	if (connect(sock,(struct sockaddr *)&addr,sizeof(struct sockaddr_in)) == -1){
		perror("Connect Error");	return EXIT_FAILURE;
	}
loop:printf("请选择:");	
	scanf("%d", &key);
	if (key == 1)
		buf.source_id = lognin_sev(sock);
	else{
		signup_sev(sock);
		goto loop;
	}
	addnewfriend(&buf,sock);
	chatuser(&buf,sock);
//	bulidgroup(&buf,sock);
//	joingroup(&buf,sock);
//	quitgroup(&buf,sock);
//	chatgroup(&buf,sock);
	
}
