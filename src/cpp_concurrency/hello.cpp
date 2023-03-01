#include <iostream>
#include <thread>
#include <chrono>
#include <functional>

void f(uint i, std::string const &s)
{
    std::cout << "i: " << i << " " << s << std::endl;
}
void oops(uint some_param)
{
    char buffer[1024]; // 1
    sprintf(buffer, "%u", some_param);
    std::thread t(f, 3, buffer); // 2
    // std::thread t(f, 3, std::string(buffer)); // 2
    t.detach();
}

void update_data_f(uint i, std::string &s) // 1
{
    std::cout << "i: " << i << " " << s << std::endl;
    s = "update_data_f";
}

void oops_again(uint some_param)
{
    char buffer[1024]; // 1
    sprintf(buffer, "%u", some_param);
    ///////////
    auto str = std::string(buffer);
    std::thread t(update_data_f, 3, std::ref(str)); // 2
    ////////////
    // std::thread t(update_data_f, 3, buffer);           // 2
    t.join();
    std::cout << "str : " << str << std::endl;
}

class X
{
public:
    void do_lengthy_work()
    {
        std::cout << "str : "
                  << "do_lengthy_work" << std::endl;
    }
};
X my_x;

int main()
{
    oops(-1);
    oops_again(-2);

    std::thread t(&X::do_lengthy_work, &my_x); // 1
    t.join();

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);

    std::thread t1(f, 1, "txt1");
    std::thread t2(f, 2, "txt2");
    std::thread t3, t4;
    std::cout << "before move t2 to t3" << std::endl;
    t3 = std::move(t2);
    std::cout << "before move t1 to t2" << std::endl;
    t2 = std::move(t1);
    std::cout << "after move" << std::endl;
    std::this_thread::sleep_for(10ms);
    t2.join();
    t3.join();
    if (t4.joinable())
        t4.join();
    else
        std::cout << "unjionable " << std::endl;

    return 0;
}