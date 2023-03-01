#include <algorithm>
#include <cctype>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <vector>


class number
{
    private:
    void * operator new(size_t size){return reinterpret_cast<void*> (1);}
    void  operator delete(void * ptr){};

};

class pureNumber:private number
{
public:
    void * operator new(size_t size){return ::operator new(size);}
// private:
    void  operator delete(void * ptr){return ::operator delete(ptr);};
};

int main() {
    void *pPureNumber = new pureNumber();
    std::shared_ptr<pureNumber> sptr = std::make_shared<pureNumber>();
    const pureNumber* nakePtr =  sptr.get();
    delete nakePtr;
    return 0;
}