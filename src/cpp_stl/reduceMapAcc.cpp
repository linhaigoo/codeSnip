/*
 * @Author: linhaigoo linhaigoo2013@gmail.com
 * @Date: 2023-01-05 23:35:56
 * @LastEditors: linhaigoo linhaigoo2013@gmail.com
 * @LastEditTime: 2023-02-18 19:56:11
 * @FilePath: /codeSnip/src/cpp_stl/reduceMapAcc.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 * 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>
float func(float a, float b) { return a + b + 0.7; }

int main() {
  std::vector<float> values = {1.2, 2.3, 3.4, 4.5, 5.6};
  auto intRet = std::accumulate(values.begin(), values.end(), 0);
  std::cout << " std::accumulate(values.begin(), values.end(), 0) :" << intRet
            << std::endl;
  auto Floatret = std::accumulate(values.begin(), values.end(), 0.0);
  std::cout << " std::accumulate(values.begin(), values.end(), 0.0) :"
            << Floatret << std::endl;

  auto accFuncRet = std::accumulate(values.begin(), values.end(), 0, func);
  auto accptrFuncRet =
      std::accumulate(values.begin(), values.end(), 0, std::ptr_fun(func));
  std::cout << "std::accumulate(values.begin(), values.end(), 0, "
               "std::ptr_fun(func)) :"
            << accptrFuncRet << std::endl;
  auto accLambdaFuncRet =
      std::accumulate(values.begin(), values.end(), 0,
                      [](float a, float b) { return a + b + 0.6; });
  std::cout << "std::accumulate(values.begin(), values.end(), 0, [](float a, "
               "float b) {return a + b; }) :"
            << accLambdaFuncRet << std::endl;

  std::cout << "The sum of the ints on the standard input is\n"
            << std::accumulate(std::istream_iterator<int>(std::cin),
                               std::istream_iterator<int>(), 0);
}