/*
 * @Author: Kian Liu
 * @Date: 2021-08-07 13:21:02
 * @LastEditTime: 2021-08-07 22:09:29
 * @LastEditors: Kian Liu
 * @Description:
 * @FilePath: /codeSnip/src/cpp_succeed/cpp_virtual_call.cpp
 */
#include "stdint.h"
#include "stdio.h"
#include <memory>
#include <unistd.h>

class par
{
  public:
    // virtual ~par()
    ~par()
    {
        printf("par deconstruct\n");
    }

    // void mem_func()
    virtual void mem_func()
    {
        printf("par mem_func\n");
    }

    void mem_func_call()
    // virtual void mem_func_call()
    {
        printf("par mem_func call\n");
        // this->mem_func();
        mem_func();
    }
};

class sub : public par
{
  private:
    par par1;

  public:
    sub(){

    };

    ~sub()
    // virtual ~sub()
    {
        printf("sub deconstruct\n");
    };

    void mem_func()
    {
        printf("sub mem_func\n");
    }
};

int main()
{
    par *pPar = new sub();
    pPar->mem_func_call();
    delete pPar;
    sleep(0.01);
}
