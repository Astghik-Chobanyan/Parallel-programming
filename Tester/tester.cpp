#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

enum testres
{
    success,
    fail,
    unresolved
};
const int test_count = 2;
testres (*testfunctions[test_count])();

testres test1()
{
    std::cout << "From test1 \n";
    //abort();
    return fail;
}

testres test2()
{
    std::cout << "From test2 \n";
    return success;
}

int main()
{
    testfunctions[0] = test1;
    testfunctions[1] = test2;

    pid_t child_id;

    for(int i = 0; i < test_count; i++)
    {
        int fds[2];

        if(pipe(fds) == -1)
        {
            std::cerr << "Can't create pipe";
            return 1;
        }
        child_id = fork();
        if (child_id == -1)
        {
            std::cerr << "Can't fork" << std::endl;
            return 1;
        }
        if (child_id == 0)
        {
            close(fds[0]);
            close(1);
            dup(fds[1]);
            close(fds[1]);
            return testfunctions[i]();  
        }
        else
        {
            close(fds[1]);
            int wstat;
            int read_res;
            const int buff_size = 1000;
            std::string output;
            char buff[buff_size];
            while ((read_res = read(fds[0], buff, buff_size)) > 0)
            {
                buff[read_res] = '\0';
                output += buff;
            }
            close(fds[0]);
            waitpid(child_id, &wstat, 0);
            testres res;
            if (WIFEXITED(wstat))
            {
                res = (testres)WEXITSTATUS(wstat);
            }
            std::cout << "Test result: " << res << std::endl << "Test output: " << output << std::endl;
        }
    }
}