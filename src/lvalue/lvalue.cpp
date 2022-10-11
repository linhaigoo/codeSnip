/*
 * @Author: linhaigoo linhaigoo2013@gmail.com
 * @Date: 2022-09-27 23:47:51
 * @LastEditors: linhaigoo linhaigoo2013@gmail.com
 * @LastEditTime: 2022-09-29 00:11:04
 * @FilePath: /codeSnip/src/lvalue/lvalue.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 * 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
#include <type_traits>
#include <utility>
class oobj {
private:
  int a;

public:
  oobj(oobj &&oo) {
    a = oo.a;
    printf("oobj &&oo construct\n");
  }

  oobj(const oobj &oo) {
    a = oo.a;
    printf("oobj &oo construct\n");
  }

  oobj() {
    a = 1;
    printf("oobj construct\n");
  }

  oobj &operator=(const oobj &oo) {
    a = oo.a;
    printf("= const oobj &oo\n");
    return *this;
  }

  // oobj &operator=(oobj &&oo) {
  //   a = oo.a;
  //   printf("= oobj &&oo\n");
  //   return *this;
  // }
  friend const oobj operator*(const oobj &lhs,
                              const oobj &rhs); // friend is must

  oobj operator+(const oobj &rhs) {
    oobj o;
    o.a = this->a * rhs.a;
    return o;
  }
};

void func(const oobj &&oo) {
  printf("func(const oobj &&oo)\n");
  printf("oobj o2 = oo\n");
  oobj o2 = oo;
}

const oobj operator*(const oobj &lhs, const oobj &rhs) {
  oobj o;
  o.a = lhs.a * lhs.a;
  return o;
}

// rvalue will be accept as para
// void func(const oobj &oo) {
//   printf("func(const oobj &oo)\n");
//   printf("oobj o2 = oo\n");
//   oobj o2 = oo;
// }

void func_forward(oobj &&oo) {
  printf("oobj o2 = std::forward<oobj>(oo)\n");
  oobj o2 = std::forward<oobj>(oo);
}

int main() {
  printf("oobj o1\n");
  oobj o1;
  //   printf("oobj o2(oobj)\n");
  //   oobj o2(oobj);
  printf("oobj o2(std::move(o1))\n");
  oobj o2(std::move(o1));
  printf("oobj o3 = o2\n");
  oobj o3 = o2;
  printf("oobj o4 = std::move(o2)\n");
  oobj o4 = std::move(o2);
  printf("o4=o2\n");
  o4 = o2;
  printf("o4 = std::move(o2)\n");
  o4 = std::move(o2);
  printf("func test\n");
  //   func(o4);
  func(std::move(o4));

  printf("func forward test\n");
  //   func(o4);
  func_forward(std::move(o4));

  printf("const ret operator test\n");
  const oobj &o5 = o3 * o1;
  //   o3 *o1 = o4; const ret
  printf("non ret operator test\n");
  // oobj &o6 = o3 + o1; //temporary value cannot bind to non const reference
  o3 + o1 = o4; // non const ret
  printf("call desctruct func then copy assign\n");
  o4.~oobj();
  o4 = o3 + o1;
  oobj *pO6 = new oobj();
  printf("call desctruct func heap then copy assign\n");
  pO6->~oobj();
  *pO6 = o3 + o1;
  printf("delete heap then copy assign\n");
  delete pO6;
  *pO6 = o3 + o1;
  printf("rvo test\n");
  oobj o7 = o3 + o1;
  printf("rvo test copy to existing obj\n");
  o7 = o2 + o1;
  return 0;
}
