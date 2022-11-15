/*
 * @Author: linhaigoo linhaigoo2013@gmail.com
 * @Date: 2022-11-13 21:51:01
 * @LastEditors: linhaigoo linhaigoo2013@gmail.com
 * @LastEditTime: 2022-11-13 22:41:37
 * @FilePath: /codeSnip/src/cpp_static/objcectCount.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 * 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <atomic>
#include <iostream>

template <typename T>
class countObj {
    static std::atomic<int> count;
    int obj_id;

   public:
    countObj(){ obj_id = count.fetch_add(1); }
    countObj(int i) : countObj() { throw std::bad_alloc(); }
    countObj(const countObj &obj) { obj_id = count.fetch_add(1); }
    countObj &operator=(const countObj &obj) { return *this; }
    ~countObj() { count.fetch_add(-1); };
    int getCount() { return obj_id; }

    // private: //uncomment here all new operator invalid when compile even if constructor nnonexcept
    static void operator delete(void *mem) {
        ::operator delete(mem);
        // ::delete mem;//delete void * : compile error
    }
};

template <typename T>
std::atomic<int> countObj<T>::count{0};

class widget : public countObj<widget> {
   public:
    widget(int i): countObj(i){};
    widget() = default;
};

class cwidget : private countObj<widget> {
   public:
    cwidget(int i): countObj(i){};
    cwidget() = default;
};

static widget w1;
static widget wArray[2];

int main() {
    widget statckw1;
    widget copyCountw(statckw1);

    // try {
    //     widget statckw1Exception(1);
    // } catch (...) {
    // }

    // try {
    //     widget *pwHeapException = new widget(1);
    // } catch (...) {
    // }

    widget statckw3;
    widget *pw = new widget;

    std::cout << "w1 count " << w1.getCount() << std::endl;
    std::cout << "wArray[0] count " << wArray[0].getCount() << std::endl;
    std::cout << "wArray[1] count " << wArray[1].getCount() << std::endl;
    std::cout << "statckw1 count " << statckw1.getCount() << std::endl;
    std::cout << "copyCountw count " << copyCountw.getCount() << std::endl;
    std::cout << "statckw3 count " << statckw3.getCount() << std::endl;
    statckw3 = statckw1;
    std::cout << "statckw3 count after copy " << statckw3.getCount()
              << std::endl;
    std::cout << "pw count " << pw->getCount() << std::endl;
}