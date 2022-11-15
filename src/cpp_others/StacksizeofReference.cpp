#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <string_view>
using namespace std;

class meta
{
    int& intern;
    public:
    meta(int&a ):intern(a)
    {
    }
};

class zero
{
};

int func(int x[], int num) {
    for (int i = 0; i < num; i++) {
        std::cout << "val : " << x[i] << "addr : "<< &x[i] <<std::endl;
    }
    return 0;
}

int main(void) {
    int a[] = {1, 2, 3};
    func(a,3);
    int b=1;
    int c=2;
    int d=3;
    std::cout << "####################" << std::endl;
    func(&b,1);
    func(&c,1);
    func(&d,1);
    std::cout << "####################" << std::endl;
    std::cout << "sizeof(meta)" <<sizeof(meta)<< std::endl;
    std::cout << "sizeof(zero)" <<sizeof(zero)<< std::endl;
    return 0;
}