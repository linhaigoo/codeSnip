/*
 * @Author: linhaigoo linhaigoo2013@gmail.com
 * @Date: 2022-09-18 15:43:50
 * @LastEditors: linhaigoo linhaigoo2013@gmail.com
 * @LastEditTime: 2022-09-18 16:29:17
 * @FilePath: /codeSnip/src/const_item3/const_member.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 * 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdint.h>
#include <stdio.h>
#include <string>

class obj {
public:
  const char &operator[](size_t pos) const {
    printf("const\n");
    return ptr[pos];
  }
  char &operator[](size_t pos) {
    printf("non const\n");
    return ptr[pos];
  }

  const char &operator()(size_t pos) const {
    printf("const invoke other []\n");
    // return this->[pos];
    // return (*this)[pos];//const []
    // return this->operator[](pos); // const []
    return const_cast<obj *>(this)->operator[](pos); // non const []
  }

  obj(char *pText) : ptr(pText) {}

private:
  char *const ptr;
};

int main() {
  std::string str("Hello");
  obj obj1((char *)str.data());
  (static_cast<obj>(obj1))[0] = 'j';
  printf("Obj %s\n", &obj1[0]);
  printf("Obj () %s\n", &obj1(0));
  return 0;
}