/*
 * @Author: linhaigoo linhaigoo2013@gmail.com
 * @Date: 2022-10-29 11:01:09
 * @LastEditors: linhaigoo linhaigoo2013@gmail.com
 * @LastEditTime: 2022-10-29 11:11:03
 * @FilePath: /codeSnip/src/primer/cpp_overload.cpp
 */
#include <stdio.h>
void test_over(const char a) {}

// void test_over(char a) {} // redifition

void test_over(char *a) {
  printf("%s, char*a, %d\n", __FUNCTION__, *a);
  *a = 1;
}
void test_over(const char *a) {
  printf("%s, const char *a, %d\n", __FUNCTION__, *a);
}

int test() {
  char a;
  test_over(&a);
  test_over("cs");
  //   test_over((char *)"cs"); // invoke test_over(char *a) let Bus error: 10
  const char b = 1;
  const char *cpb = &b;
  char *pb = (char *)cpb;
  test_over(&b);
  return 0;
}

int main() { return test(); }