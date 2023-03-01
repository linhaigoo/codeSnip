#include <algorithm>
#include <cctype>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

template<typename objType, typename returnType, typename functionType>
class PFMC
{public:
returnType operator()()
{
    return (pObj->*ptr)();
}

PFMC(objType* _pObj, functionType _ptr):pObj(_pObj),ptr(_ptr)
{
    
}
    private:
    functionType const ptr;
    objType *const pObj;
};

template <typename objType>
class sptr
{
    struct deleteor
    {
        virtual void delfunc(void *)=0;
    };

    template<typename subObjType>
    struct specDeleteor:public deleteor
    {
        void delfunc(void * ptr) override
        {
            delete (subObjType*)ptr; 
        }
    };

    public:
    

    template<typename subObjType>
    sptr(subObjType *_ptr)
    {
      ptr = _ptr;
      del = new specDeleteor<subObjType>();
    }

    ~sptr()
    {
        if(del)
        del->delfunc(ptr);
        // delete ptr;
        delete del;
    }

    template<typename returnType,typename basicObjType>
    PFMC<objType, returnType, returnType(basicObjType::*)()> operator->*(returnType(basicObjType::*funcPtr)())
    {
       return PFMC<objType, returnType, returnType(basicObjType::*)()>(ptr, funcPtr);
    }

    objType *operator->()
    {
      return ptr;
    }
    private:
    objType *ptr;
    struct deleteor *del = nullptr;

};

class wombat {
   public:
    void sleep() { std::cout << "sleep wombat" << std::endl; }
    void eat() { std::cout << "eat wombat" << std::endl; }
    virtual void dig() { std::cout << "dig wombat" << std::endl; }
    ~wombat()
    {
        std::cout<< __FUNCTION__ << std::endl;
    }
    int a;
};

class womwombat: public wombat
{
    public:
    void sleep() { std::cout << "sleep womwombat" << std::endl; }
    virtual void dig() { std::cout << "dig womwombat" << std::endl; }
    ~womwombat()
    {
        std::cout<< __FUNCTION__ << std::endl;
    }
};

int main() {
    typedef void (wombat::*PWMF)(void);
    PWMF pmf = &wombat::sleep;
    wombat* pObjROO = new womwombat;
    (pObjROO->*pmf)();

    typedef void (womwombat::*subPWMF)(void);
    subPWMF subpmf = &womwombat::sleep;
    sptr<womwombat> spOObj=new womwombat;
    womwombat* pOObjROO=new womwombat;

    // (pObj->*subpmf)();//type unamtch

    (pOObjROO->*subpmf)();
    (spOObj->*subpmf)();
    (pOObjROO->*pmf)();

    // PWMF pmfConv = &womwombat::sleep;//type unamtch
    // PWMF pmfConv = &womwombat::dig;//type unamtch
    PWMF pmfConv = &womwombat::eat;

    subPWMF subpmfOverlap = &womwombat::sleep;
    PWMF pmfOverlap = &wombat::sleep;
    ((womwombat*)pObjROO->*subpmfOverlap)();
    ((womwombat*)pObjROO->*pmfOverlap)();
    (pObjROO->*pmfOverlap)();

    (pObjROO->*pmfConv)();

    subPWMF subpmfVirtual = &womwombat::dig;
    (pOObjROO->*subpmfVirtual)();

    PWMF pmfVirtual = &wombat::dig;
    (pObjROO->*pmfVirtual)();
    (pOObjROO->*pmfVirtual)();
    wombat* pObjRO = new wombat;
    (pObjRO->*pmfVirtual)();


    // pmfConv = subpmf;//cannot conversion

    (spOObj->*pmf)();//compile success
    spOObj->sleep();
    std::cout<<"spOObj.a"<< spOObj->a<<std::endl;
}