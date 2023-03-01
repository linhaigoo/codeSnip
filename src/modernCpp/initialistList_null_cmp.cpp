#include <iostream>
#include <type_traits>
void foo(char *);
void foo(int);

#include <initializer_list>
#include <iostream>
#include <vector>
class MagicFoo {
   public:
    std::vector<int> vec;
    MagicFoo(const std::initializer_list<int> &list) {
        for (std::initializer_list<int>::iterator it = list.begin();
             it != list.end(); ++it)
            vec.push_back(*it);
    }
};

template <typename... Ts>
class Magic {
   public:
    Magic() { std::cout << "Magic()" << std::endl; }
};

int main() {
    if (std::is_same<decltype(NULL), decltype(0)>::value)
        std::cout << "NULL == 0" << std::endl;
    if (std::is_same<decltype(NULL), decltype((void *)0)>::value)
        std::cout << "NULL == (void *)0" << std::endl;
    if (std::is_same<decltype(NULL), std::nullptr_t>::value)
        std::cout << "NULL == nullptr" << std::endl;
    foo(0);
    // foo(NULL);
    foo(nullptr);

    // after C++11
    MagicFoo magicFoo = {1, 2, 3, 4, 5};
    std::cout << "magicFoo: ";
    for (std::vector<int>::iterator it = magicFoo.vec.begin();
         it != magicFoo.vec.end(); ++it)
        std::cout << *it << std::endl;

    Magic<int, std::vector<int>> darkMagic;
    Magic<> darkMagic2;

    return 0;
}
void foo(char *) { std::cout << "foo(char*) is called" << std::endl; }
void foo(int i) { std::cout << "foo(int) is called" << std::endl; }