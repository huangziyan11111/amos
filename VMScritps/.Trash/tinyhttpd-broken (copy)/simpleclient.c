#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int main(int argc, char *argv[])
{
 int sockfd, bytes;
 int len;
 struct sockaddr_in address;
 int result;
 char ch = 'A';
 char senddata[1024], recvdata[1024];

 sockfd = socket(AF_INET, SOCK_STREAM, 0);
 address.sin_family = AF_INET;
 address.sin_addr.s_addr = inet_addr("127.0.0.1");
 address.sin_port = htons(527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774527745277452774
