/*
 * @Author: linhaigoo linhaigoo2013@gmail.com
 * @Date: 2023-01-03 23:22:36
 * @LastEditors: linhaigoo linhaigoo2013@gmail.com
 * @LastEditTime: 2023-01-05 23:35:19
 * @FilePath: /codeSnip/src/cpp_stl/insert.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 * 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

int main() {
  std::vector<float> values = {1.2, 2.3, 3.4, 4.5, 5.6};
  std::vector<float> results;
  std::transform(values.begin(), values.end(), std::back_inserter(results),
                 [](int value) { return value + 2; });
  for (auto &item : results) {
    std::cout << item << std::endl;
  }
}