#include <initializer_list>
#include <iostream>
#include <span>
#include <string>
#include <type_traits>
#include <vector>

using std::cout;
using std::string;

class gstring : public string {
   public:
    using string::string;
    ~gstring() { std::cout << "desctructor string " <<*this<< std::endl; }
};

const gstring& foo(const gstring& a, const gstring& b) {
    const gstring& ret= a.empty() ? b : a;
    std::cout << "foo function" <<std::endl;
    return ret;
}

template <typename T>
const T& wrapper(const T& t) {
    return t;
}

int main() {
    auto& s = foo("foo1", "foo2");
    foo("foo3", "foo4");
    cout << s << "  " << typeid(s).name() << '\n';

    for (auto i : std::vector{1, 2, 3, 4})  // 正确1
    {
        cout << i << '\n';
    }

    for (auto i : wrapper(std::vector{1, 2, 3, 4}))  // 翻车现场1
    {
        cout << i << '\n';
    }
}