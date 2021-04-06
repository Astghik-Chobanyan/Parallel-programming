#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <string>
#include <queue>
#include <vector>



typedef void (*message_handler)(std::string);
class message_queue
{
private:
    std::queue<std::string> _queue;
    std::vector<message_handler> _functions;
public:
    friend void *message_broadcaster(void *param);

    void post_message(std::string str);
    void register_handler(message_handler);
    message_queue();
    ~message_queue();
};
