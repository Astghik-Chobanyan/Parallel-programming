#include <iostream>
#include <string>
#include "messagequeue.h"

void    message_handler1(std::string message)
{
    std::cout << message << std::endl;
}
int main()
{
    message_queue mess_queue;
    mess_queue.register_handler(message_handler1);
    mess_queue.post_message("hello");
    mess_queue.post_message("world");
    sleep(1);
    mess_queue.post_message("this");
    int n;
    std::cin >> n;
}