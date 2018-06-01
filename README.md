暑假学习最后完成的一个小小聊天软件,用到了网络编程、线程、文件、管道等，也算是对暑假学习的总结

链表的形式存储sock信息，pHead->server->client->client->...->NULL；
每上线一个客户，和server建立联系（accept）后，就添加一个节点，下线就删除相应节点；
每个节点存放ip，sock，id；
select轮询各个client，一旦有数据可读就解除阻塞，接受数据datapack并分析；
每次登录或登出，出错等都记录文件


Client：（user_history.dat）
pipe管道传输

Server：（id.dat,user.dat,group.dat,user_list.dat）
注册：
在id.dat里寻找最大用户id，没有就默认创建10000，群一样操作；
用户注册成功就保存更新相应的user.dat和  user_list.dat(用户朋友列表和群列表)

登录：
验证user.dat是否存在该用户（账号和密码），存在就返回登录成功；
并且链表增加一个client，

私聊：
select会查询client节点有没有可读数据，有就recv过来并分析datapack；
分析出私聊后，查找user_list.dat看是否是好友，是再检查是否是在线，是就直接聊天；
获取聊天对象的sock（while循环查询sock==buf->target_id），然后就发送聊天内容给该对象的sock;
就是用户先和服务器交互完成，在和聊天对象进行交互聊天；

获取好友列表：
使用到文件传输sendfile（），该函数比一般的send（）函数快，原因在于他只需要在内核之间传输，不涉及用户缓冲区；


相应linux函数:
send();
recv();
sendfile();
splice();
pipe();
system();

网络编程：
sock();
setsockopt();
bind();
listen();
select();
accept();

库函数：
fscanf();
fread();
memset();
fgets();
pthread_creat();
setbuf();
