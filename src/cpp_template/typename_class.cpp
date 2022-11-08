/*
 * @Author: linhaigoo linhaigoo2013@gmail.com
 * @Date: 2022-10-19 00:55:35
 * @LastEditors: linhaigoo linhaigoo2013@gmail.com
 * @LastEditTime: 2022-10-19 01:18:39
 * @FilePath: /codeSnip/src/cpp_template/typename_class.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 * 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
// Type your code here, or load an example.
#include <iostream>
#include <vector>

template <class c> void printd2nd(const c &container) {
  if (container.size() >= 2) {
    typename c::const_iterator iter(
        container.begin()); // typename must declare here
    ++iter;
    int value = *iter;
    std::cout << value << std::endl;
  }
}

template <class c> void printd2nds(const c &container) {
  if (container.size() >= 2) {
    typename c::const_iterator iter(
        container.begin()); // typename must declare here
                            // typename c::w *(2);     // defualt no type
    c::w *= (2);            // defualt no type
    ++iter;
    int value = *iter;
    std::cout << value << std::endl;
  }
}

class testVect : public std::vector<float> {
public:
  static int w;
};

int testVect::w = 2;

int main() {
  std::vector<float> a{2, 1};
  printd2nd<std::vector<float>>(a);
  testVect b;
  b.push_back(1);
  b.push_back(2);
  printd2nds(b);
  std::cout << testVect::w << std::endl;
}