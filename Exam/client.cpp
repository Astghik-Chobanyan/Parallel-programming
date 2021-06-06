#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <iostream>

void    logError(std::string message)
{
    std::cerr << " error: " << message << " " << strerror(errno) << std::endl;
    exit(0);
}

int main()
{

   int sockfd, portno, n;
   sockaddr_in serv_addr;
   memset(&serv_addr, 0, sizeof(serv_addr));
   serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_port = htons(2000);

   char buffer[256];
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if(sockfd < 0)
   {
      logError("create socket failed");
   }
   
   if(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
   {
      logError("connecting error\n");
   }

   bzero(buffer, 256);
   n = read(sockfd, buffer, 255);
   if(n < 0)
   {
      logError("error: reading from socket\n");
   }
   printf("%s\n", buffer);
   while(1)
   {
      printf("please enter message: ");
      bzero(buffer, 256);
      fgets(buffer, 255, stdin);
      n = write(sockfd, buffer, strlen(buffer));
      if(n < 0)
      {
         logError("error: to write in socket\n");
      }
      bzero(buffer, 256);
      n = read(sockfd, buffer, 255);
      if(n < 0)
      {
        logError("error: reading from socket\n");
      }
      printf("%s\n", buffer);
   }
   close(sockfd);
   return 0;
}
