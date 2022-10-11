/*
 * @Author: linhaigoo linhaigoo2013@gmail.com
 * @Date: 2022-09-18 21:07:46
 * @LastEditors: linhaigoo linhaigoo2013@gmail.com
 * @LastEditTime: 2022-09-20 00:24:59
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

AWOV::~AWOV(){};

class derived : public AWOV {};

int main() {
  AWOV v1;
  //   const void *delfuncPtr = &v1.~AWOV;
  //   void (*delfuncPtr)(AWOV *) = &AWOV::func;
  void (AWOV::*delfuncPtr)() = &AWOV::func;

  printf("delfuncPtr %p \n", delfuncPtr);
  //   printf("delfuncPtr %p \n", (delfuncPtr)(&v1));
}