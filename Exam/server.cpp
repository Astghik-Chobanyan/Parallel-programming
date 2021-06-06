#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h> 
#include <pthread.h>
#include <fcntl.h>
#include <sys/time.h>
#include <iostream>
#include <errno.h>

#define MaxSize 1024

void    logError(std::string message)
{
    std::cerr << " error: " << message << " " << strerror(errno) << std::endl;
    exit(1);
}

int main()
{

   int connectlist[MaxSize]; 
   fd_set socks;
   int highsock;
   int readsocks; //number of sockets ready for reading
   int socket_desc, client_sock, c;

   sockaddr_in server; 
   memset((char*)&server, 0, sizeof(server));
   server.sin_family = AF_INET;
   server.sin_addr.s_addr =  inet_addr("0.0.0.0");
   server.sin_port = htons(2000);
   socket_desc = socket(AF_INET, SOCK_STREAM, 0);
   if(socket_desc == -1)
   {
      logError("creating socket failed!");
   }
   if(bind(socket_desc, (struct sockaddr*)&server, sizeof(server)) < 0)
   {
      logError("bind failed error");
   }

   c = sizeof(struct sockaddr_in); 
   //listen
   if (listen(socket_desc, MaxSize) == -1)
   {
      logError("Can't listen");
   }

   highsock = socket_desc;
   memset((char*)&connectlist, 0, sizeof(connectlist));

   while(1)
   {
      FD_ZERO(&socks);
      FD_SET(socket_desc, &socks);//add file desc sock to the fd_set, so that select()
      for(int i = 0; i < MaxSize; i++)
      {
         if(connectlist[i] != 0)
         {
            FD_SET(connectlist[i], &socks);
            if(connectlist[i] > highsock)
               highsock = connectlist[i];
         }
      } //add all to the fd_set
      
      readsocks = select(highsock + 1, &socks, NULL, NULL, NULL);
      if(readsocks < 0)
      {
          logError("select exception");
          exit(EXIT_FAILURE);
      }

      if(readsocks == 0)//number of connection
      {
         std::cout << "Nothing to read" << std::endl;
      }
      else
      {
         //handle connection here
         if(FD_ISSET(socket_desc, &socks)) //any connection accepted
         {
            client_sock = accept(socket_desc, NULL, NULL);
            if(client_sock < 0)
            {
               logError("accept failed");
               break;
            }
            for(int i = 0; i < MaxSize && client_sock != -1; i++)
            {
               if(connectlist[i] == 0)
               {
                  connectlist[i]=client_sock;
                  char temp[] = "server: i initialize the talk!\n";
                  write(client_sock, temp, sizeof(temp));
                  client_sock = -1;
               }
            }
            if(client_sock != -1) 
            {
              logError("no room available for client connection\n");
            }
         }

         for(int i = 0; i < MaxSize; i++)
         {
            if(FD_ISSET(connectlist[i], &socks))
            { //deal with data
               char buffer[256];
               int numch = 0;      
               if((numch = recv(connectlist[i], buffer, 256, 0))<0)
               {
                  std::cout << "Connection lost: socket id = " << connectlist[i] << ", " << i << std::endl;
                  close(connectlist[i]);
                  connectlist[i]=0;
               }
               else
               {
                  buffer[numch]='\0';
                  std::cout << "CLient " << i << ": " << buffer << std::endl;
                  write(connectlist[i], buffer, strlen(buffer));
               }
            }
          }
        }
   }
}