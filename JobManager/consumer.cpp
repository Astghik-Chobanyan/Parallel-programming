#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

void    LogError(const char *message)
{
    std::cerr << getpid() << " - Error : " << message << " : " << strerror(errno) << std::endl;
}

int main()
{
    const char *filename = "folder";
    int filedes;
    if ((filedes = open(filename, O_WRONLY)) == -1)
    {
        LogError("Can't open");
        return 1;
    }
    int num = 0;
    int param = 0;
    std::cout << "Type member" << std::endl;
    std::cin >> num;
    std::cout << "Type parametr " << std::endl;
    std::cin >> param;
    if (write(filedes, &num, sizeof(int)) != sizeof(int))
    {
        LogError("Can't write file");
        close(filedes);
        return 1;
    }
    if (write(filedes, &param, sizeof(int)) != sizeof(int))
    {
        LogError("Can't write file");
        close(filedes);
        return 1;
    }
    close(filedes);
    const char *resfile = "output";
    int output_des;
    while(1)
    {
        if ((output_des = open(resfile, O_RDONLY)) == -1)
        {
            sleep(1);
        }
        else
            break;
    }
    const int buff_size = 1000;
    char buff[buff_size];
    int read_res;
    std::string output;
    while ((read_res = read(output_des, buff, buff_size)) > 0)
    {
        buff[read_res] = '\0';
        output += buff;
    }
    close(output_des);
    std::cout << "My output: " << output << std::endl;
    return 0;
}