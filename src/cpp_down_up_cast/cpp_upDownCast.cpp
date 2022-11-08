#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <string_view>

class Parent {
   protected:
    int var1 = 1;

   public:
    virtual void sleep() { std::cout << "sleep " << var1 << std::endl; }
};

class Child : public Parent {
    int var2 = 2;

   public:
    void gotoSchool() {
        std::cout << "gotoSchool " << var2 << std::endl;
        std::cout << "var1    address :" << &var1 << std::endl;
        std::cout << "var2    address :" << &var2 << std::endl;
    }
};

int main() {
    int var3 = 3;//stack in down allocated
    Parent parent;
    Child child;//members in object is up allocated

    std::cout << "var3    address :" << &var3 << std::endl;//must remain to force the var3 complier optimization
    std::cout << "parent address :" << &parent << std::endl;
    std::cout << "child  address :" << &child << std::endl;
    std::cout << "sizeof(parent) : " << sizeof(parent)
              << "; sizeof(child) : " << sizeof(parent) << std::endl;

    // upcast - implicit type cast allowed
    Parent *pParent = &child;

    // downcast - explicit type case required
    //   Child *pChild =  (Child *) &parent;
    //   Child *pChild =  (Child *) pParent;
    // Child *pChild = static_cast<Child *>(&parent);
    // Child *pChild = dynamic_cast<Child *>(&parent);
    // pParent = &parent;
    Child *pChild = dynamic_cast<Child *>(pParent);

    //   void * nakepChild = pChild;
    //   void * nakepParent = pParent;

    // void *nakepChild = static_cast<void *>(pChild);
    // void *nakepParent = static_cast<void *>(pParent);

       void * nakepChild = dynamic_cast<void*>(pChild);
       void * nakepParent = dynamic_cast<void*>(pParent);

    std::cout << "pointer value:\n "
              << "pChild " << pChild << ",\n pParent  " << pParent
              << ",\n nakepParent " << nakepParent << ",\n nakepChild  "
              << nakepChild << std::endl;
    if (nakepChild != nakepParent) {
        std::cout << "nake pointer not equal:\n "
                  << "nakepParent " << nakepParent << ",\n nakepChild  "
                  << nakepChild << std::endl;
    }

    pParent->sleep();
    pChild->gotoSchool();

    return 0;
}