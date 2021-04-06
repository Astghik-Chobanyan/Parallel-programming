#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

void    LogError(const char *message)
{
    std::cerr << getpid() << " - Error : " << message << " : " << strerror(errno) << std::endl;
}

int main()
{
   const char *filename = "folder";
   if (mkfifo(filename, 0666))
   {
       LogError("Can't fifo");
   }
   while (1)
   {
       int filedes;
       if ((filedes = open(filename, O_RDONLY)) == -1)
       {
            LogError("Can't open");
            return 1;
       }
       int num = 0;
       if (read(filedes, &num, sizeof(int)) != sizeof(int))
       {
           LogError("Can't read file");
           close(filedes);
           return 1;
       }
       int param = 0;
       if (read(filedes, &param, sizeof(int)) != sizeof(int))
       {
           LogError("Can't read file");
           close(filedes);
           return 1;
       }
       close(filedes);
       pid_t child_id = fork();
       if (child_id == -1)
       {
           LogError("Can't fork");
           return 1;
       }
       if (child_id == 0)
       {
           const char *resfile = "output";
           if (mkfifo(resfile, 0666) == -1)
           {
               LogError("Can't fifo");
               return 1;
           }
           int output_des;
           if ((output_des = open(resfile, O_WRONLY)) == -1)
           {
               LogError("Can't open12");
               return 1;
           }
           close(1);
           dup(output_des);
           char buff[4];
           sprintf(buff, "./%d", num);
           char arg[8];
           sprintf(arg, "%d", param);
           if (execlp(buff, buff, arg, NULL) == -1)
           {
               LogError("Can't exec");
               return 1;
           }
           close(output_des);
       }
   }
   return 0;
}