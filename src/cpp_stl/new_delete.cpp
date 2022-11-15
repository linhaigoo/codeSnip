/*
 * @Author: linhaigoo linhaigoo2013@gmail.com
 * @Date: 2022-10-29 01:40:45
 * @LastEditors: linhaigoo linhaigoo2013@gmail.com
 * @LastEditTime: 2022-11-12 21:08:02
 * @FilePath: /codeSnip/src/cpp_stl/stl_new_delete.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 * 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <algorithm>
#include <list>
#include <vector>
template <typename T> struct DeleteT {
  void operator()(const T *ptr)
      const // const T * then can be delete ?? refer to
            // https://stackoverflow.com/questions/755196/deleting-a-pointer-to-const-t-const
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
    list1.push_back(new obj);
  }

  for_each(vec0.begin(), vec0.end(),
           DeleteT<obj>()); // pvs cannot detect mem leak if annotate here
  for_each(list1.begin(), list1.end(),
           DeleteT<obj>()); // pvs cannot detect mem leak if annotate here
  //"()" in DeleteT<obj>() just define a object of which the type is
  // DeleteT<obj>
  return 0;
}

int main() { return test(); }