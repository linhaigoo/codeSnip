/*
 * @Author: linhaigoo linhaigoo2013@gmail.com
 * @Date: 2022-09-18 10:55:27
 * @LastEditors: linhaigoo linhaigoo2013@gmail.com
 * @LastEditTime: 2022-09-18 15:42:59
 * @FilePath: /codeSnip/src/constchar/constchar.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 * 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>

int64_t fun1() {
  char *str1 = "strtest";
  printf("func %s, str %s ptr %lld\n", __FUNCTION__, str1, int64_t(str1));
  return int64_t(str1);
}

int64_t fun2() {
  char *str1 = "strtest";
  printf("func %s, str %s ptr %lld\n", __FUNCTION__, str1, int64_t(str1));
  return int64_t(str1);
}

int main() {
  char *str1 = "strtest";
  char *str2 = "strtest";
  // str1[0] = '0'; // bus error on MAC : Apple clang version 11.0.3
  printf("func %s, str %s ptr %lld\n", __FUNCTION__, str1, int64_t(str1));
  printf("func %s, str %s ptr %lld\n", __FUNCTION__, str2, int64_t(str2));
  printf("check str1 == str2 %s\n", str1 == str2 ? "true" : "false");
  printf("check func call str1 == str2 %s\n",
         fun1() == fun2() ? "true" : "false");
  return 0;
}