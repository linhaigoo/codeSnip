/*
 * @Author: Kian Liu
 * @Date: 2021-08-07 21:41:26
 * @LastEditTime: 2021-08-07 21:45:57
 * @LastEditors: Kian Liu
 * @Description:
 * @FilePath: /codeSnip/src/cpp_success/cpp_new_delete.cpp
 */
#include "stdint.h"
#include "stdio.h"
#include <memory>
#include <unistd.h>

class inst
{
  private:
    int a[1000];
    double b[1000];
};

class par
{
  public:
    // virtual ~par()
    ~par()
    {
        printf("par deconstruct\n");
    }
};

class sub : public par
{
  private:
    inst *pInst = NULL;
    std::shared_ptr<inst> pSInst;
    int a[1000];
    double b[1000];

  public:
    sub()
    {
        // pInst = new inst();
        pSInst = std::make_shared<inst>();
    };

    ~sub()
    // virtual ~sub()
    {
        if (pInst != NULL)
        {
            delete pInst;
        }
        printf("sub deconstruct\n");
    };
};

int main()
{
    for (int i = 0; i < 10000000; i++)
    {
        par *pPar = new sub();
        delete pPar;
        printf("i %d\n", i);
        sleep(0.01);
    }
}
