#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <string_view>
using namespace std;

class BB {
   public:
    BB() {}
    BB(int a) { Pub_mem = a; }

    void printC() { cout << "c:" << Pri_Static_mem << endl; }
    void AddC() { Pri_Static_mem = Pri_Static_mem + 1; }

   public:
    static void getC() {
        cout << "static method private member c:" << Pri_Static_mem << endl;
        cout << "static method public member c:" << Pub_Static_mem << endl;
        Private_Static_func();
        Public_Static_func();
    }

    static void Public_Static_func() {}

   private:
    static void Private_Static_func() {}

   public:
    int Pub_mem;
    static int Pub_Static_mem;

   private:
    int a;
    int b;
    static int Pri_Static_mem;

   protected:
};

int BB::Pri_Static_mem = 10;
int BB::Pub_Static_mem = 1;

int main(void) {
    BB b1, b2, b3;
    b1.printC();  // 10
    b2.AddC();    // 11
    b3.printC();  // 11

    b3.getC();
    BB::getC();

    cout << "hello..." << endl;

    BB cs(7);

    for (int i = 0; i < 5; i++) {
        BB::Pub_Static_mem += i;
        cout << BB::Pub_Static_mem << '\t';
    }
    cout << endl;

    return 0;
}