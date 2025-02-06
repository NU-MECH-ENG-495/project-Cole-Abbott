#include <iostream>
#include <string>
#include <thread>

void threadRoutine(int n)
{
    while (1)
    {
        std::cout << "*" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(n));
    }
}

int main()
{
    std::thread t1(threadRoutine, 1);
    std::thread t2(threadRoutine, 5);

    std::string input;
    std::cin >> input;
    t1.detach();
    t2.detach();
    return 1;
}
