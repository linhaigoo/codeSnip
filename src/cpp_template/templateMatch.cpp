/*
 * @Author: linhaigoo linhaigoo2013@gmail.com
 * @Date: 2022-11-13 00:55:14
 * @LastEditors: linhaigoo linhaigoo2013@gmail.com
 * @LastEditTime: 2022-11-13 00:58:09
 * @FilePath: /codeSnip/src/cpp_template/templateMatch.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 * 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <algorithm>
#include <cctype>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <string>
#include <type_traits>

using namespace std;

class comparePtrContentMethod {
public:
  template <typename T>
  bool operator()(const T *ptr1,
                  const T *ptr2) const // const method here to supress warning
  {
    std::cout << "use const T * version" << std::endl;
    return *ptr1 < *ptr2;
  }

  template <typename T,
            typename X = std::enable_if_t<!std::is_pointer<T>::value>> // SFINE
  bool operator()(const T ptr1,
                  const T ptr2) const // const method here to supress warning
  {
    std::cout << "use const T version" << std::endl;
    return *ptr1 < *ptr2;
  }

  // template <typename T> //if uncomment here, this has prior adaptive
  // bool operator()(const T ptr1,
  //                 const T ptr2) const // const method here to supress warning
  // {
  //   std::cout << "use const T version" << std::endl;
  //   return *ptr1 < *ptr2;
  // }
};

int main() {
  typedef set<string *, comparePtrContentMethod> smethodTemplateSet;
  smethodTemplateSet myset;
  myset.insert(new string("ice5"));
  myset.insert(new string("judge5"));
  myset.insert(new string("hello5"));
  myset.insert(new string("kity5"));
  transform(myset.begin(), myset.end(), ostream_iterator<string>(cout, "\n"),
            [](const string *ptr) { return *ptr; });
}