#include <stdio.h>  
#include <string.h>  
#include <errno.h>  
#include <sys/socket.h>  
#include <resolv.h>  
#include <stdlib.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <unistd.h>  
#define MAXBUF 1024  


int main(int argc, char **argv)  
{  
    int sockfd, len;  
    /* struct sockaddr_in dest; */ // IPv4  
    struct sockaddr_in6 dest;      // IPv6  
    char buffer[MAXBUF + 1];
    FILE *stream;

    if (argc != 3) {  
        printf  
            ("参数格式错误！正确用法如下：\n\t\t%s IP地址 端口\n\t比如:\t%s 127.0.0.1 80\n此程序用来从某个 IP 地址的服务器某个端口接收最多 MAXBUF 个字节的消息",  
             argv[0], argv[0]);  
        exit(0);  
    }  
    /* 创建一个 socket 用于 tcp 通信 */  
    /* if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { */ // IPv4  
    if ((sockfd = socket(AF_INET6, SOCK_STREAM, 0)) < 0) {      // IPv6  
        perror("Socket");  
        exit(errno);  
    }  
    printf("socket created\n");  
  
    /* 初始化服务器端（对方）的地址和端口信息 */  
    bzero(&dest, sizeof(dest));  
    /* dest.sin_family = AF_INET; */  // IPv4  
    dest.sin6_family = AF_INET6;     // IPv6  
    /* dest.sin_port = htons(atoi(argv[2])); */ // IPv4  
    dest.sin6_port = htons(atoi(argv[2]));     // IPv6  
    /* if (inet_aton(argv[1], (struct in_addr *) &dest.sin_addr.s_addr) == 0) { */ // IPv4  
    if ( inet_pton(AF_INET6, argv[1], &dest.sin6_addr) < 0 ) {                 // IPv6  
        perror(argv[1]);  
        exit(errno);  
    }  
    printf("address created\n");  
  
    /* 连接服务器 */  
    if (connect(sockfd, (struct sockaddr *) &dest, sizeof(dest)) != 0) {  
        perror("Connect ");  
        exit(errno);  
    }  
    printf("server connected\n");  
  
    /* 接收对方发过来的文件*/
    bzero(buffer, MAXBUF);
    FILE * fp = fopen("2.mp4","wb");
    if(NULL == fp )
    {
        printf("File:\t%s Can Not Open To Write\n", "2.mp4");
        exit(1);
    }
    int length=0;
    while (length=recv(sockfd,buffer,MAXBUF,0))
    {
        if(length < 0)
        {
            printf("Recieve Data From Server Failed!\n");
            break;
        }
        int write_length = fwrite(buffer,sizeof(char),length,fp);
        if (write_length<length)
        {
            printf("File:\t%s Write Failed\n", "2.mp4");
            break;
        }
        bzero(buffer,MAXBUF);

    }
    printf("Receive file finished\n");
    fclose(fp);
    /* 关闭连接 */  
    close(sockfd);  
    return 0;  
}  
