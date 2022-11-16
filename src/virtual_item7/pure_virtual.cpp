/*
 * @Author: linhaigoo linhaigoo2013@gmail.com
 * @Date: 2022-09-18 21:07:46
 * @LastEditors: linhaigoo linhaigoo2013@gmail.com
 * @LastEditTime: 2022-11-17 00:09:10
 * @FilePath: /codeSnip/src/virtual_item7/pure_virtual.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 * 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#include <stdio.h>
#include <vector>

class AWOV {
public:
  virtual ~AWOV();
  void func() { printf("int\n"); }
};

class BWOV {
public:
  virtual void func() = 0;
  // virtual void func(){} = 0;
  // virtual void func() = 0 {};
};

void BWOV::func() {
  printf("BWOV::func()\n");
} // annotate it will let compile error!

class Bderived : public BWOV {
public:
  virtual void func() {
    printf("start BWOV::func()\n");
    BWOV::func();
    printf("end BWOV::func()\n");
  };
};

AWOV::~AWOV(){};

class derived : public AWOV {};

int main() {
  AWOV v1;
  //   const void *delfuncPtr = &v1.~AWOV;
  //   void (*delfuncPtr)(AWOV *) = &AWOV::func;
  void (AWOV::*delfuncPtr)() = &AWOV::func;
  printf("call member function by its member function pointer!!!\n");
  (v1.*delfuncPtr)(); // call member function by its member function pointer!!!
  v1.~AWOV();

  printf("delfuncPtr %p \n", delfuncPtr);
  //   printf("delfuncPtr %p \n", (delfuncPtr)(&v1));
  Bderived bdo;
  // BWOV bo;
  // bo.func();
  bdo.func();
}