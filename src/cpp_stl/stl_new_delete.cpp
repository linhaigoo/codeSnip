/*
 * @Author: linhaigoo linhaigoo2013@gmail.com
 * @Date: 2022-10-29 01:40:45
 * @LastEditors: linhaigoo linhaigoo2013@gmail.com
 * @LastEditTime: 2022-11-07 00:22:26
 * @FilePath: /codeSnip/src/cpp_stl/stl_new_delete.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 * 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <algorithm>
#include <list>
#include <vector>
template <typename T> struct DeleteT {
  void operator()(const T *ptr) const // const T * then can be delete ????
  {
    delete ptr;
  }
};

class obj {};

int test() {
  obj *o1 = new obj; // PVS will detect mem leak
  std::vector<obj *> vec0;
  std::vector<obj *> vec1;
  std::list<obj *> list1;
  for (int i = 0; i < 10; i++) {
    vec0.push_back(new obj);
  }
  for (int i = 0; i < 10; i++) {
    vec1.push_back(new obj);
  }

  for_each(vec0.begin(), vec0.end(), DeleteT<obj>());
  // for_each(vec1.begin(),vec1.end(),DeleteT<obj>);//pvs cannot detect mem leak
  return 0;
}

int main() { return test(); }