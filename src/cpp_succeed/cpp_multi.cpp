/*
 * @Author: linhaigoo linhaigoo2013@gmail.com
 * @Date: 2022-10-18 00:13:44
 * @LastEditors: linhaigoo linhaigoo2013@gmail.com
 * @LastEditTime: 2022-10-18 01:09:45
 * @FilePath: /codeSnip/src/cpp_succeed/cpp_multi.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 * 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#include "stdio.h"
#include <fstream>
#include <locale>
class bbase
{
  public:
    void set() const
    {
        printf("bbase %s\n", __FUNCTION__);
    }
    void bset() const
    {
        printf("bbase %s\n", __FUNCTION__);
    }
    int val3 = 3;

  private:
    int val1 = 1;
    int val2 = 2;
};

class base1 : private bbase
{
  private:
    int check()
    {
        printf("base1 %s\n", __FUNCTION__);
        return 0;
    }
    void set(float value)
    {
        value = value;
        printf("base1 %s\n", __FUNCTION__);
        bbase::set();
        bset();
    };
    int val1 = 4;
    int val2 = 5;

  public:
    int get()
    {
        printf("base1 %s\n", __FUNCTION__);
        return 0;
    }
    int val3 = 6;
};

class base2 : public bbase
{
  private:
    int check()
    {
        printf("base2 %s\n", __FUNCTION__);
        return 0;
    }

    int val1 = 7;
    int val2 = 8;

  public:
    int get()
    {
        printf("base2 %s\n", __FUNCTION__);
        return 0;
    }
    void set(int value) const
    {
        value = value;
        printf("base2 %s\n", __FUNCTION__);
        bbase::set();
        bset();
    };
    int val3 = 9;
};

class derived : public base1, public base2
{
  public:
    void dset()
    {
        base2::set(1.0);
    }
};

int main()
{
    derived d1;
    d1.base2::set(1);
    //   d1.set(1);
    d1.dset();
    //   d1.bset();
    d1.base2::bset();
    //   d1.val3;
    printf("d1.base1::val3 %d\n", d1.base1::val3);
    //   printf("d1.base1::val3 %d\n", d1.base2::bbase::val3);
}