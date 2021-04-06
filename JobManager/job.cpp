#include <iostream>
#include <unistd.h>

int main(int argc, char **argv)
{
    int num = atoi(argv[0] + 2);
    for(int i = 0; i < num; i++)
    {
        std::cout << "doing " << i << " from " << num << std::endl;
        sleep(1);
    }
    std::cout << "Given parametr is : " << argv[1] <<  std::endl;
    return 0;
}