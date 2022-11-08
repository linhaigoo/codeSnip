/*
 * @Author: linhaigoo linhaigoo2013@gmail.com
 * @Date: 2022-10-19 23:49:48
 * @LastEditors: linhaigoo linhaigoo2013@gmail.com
 * @LastEditTime: 2022-10-30 11:28:51
 * @FilePath: /codeSnip/src/cpp_template/cpp_bloat.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 * 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdint.h>
#include <stdio.h>
#include <string>

template <typename T> class SquareMatrixBase {
protected:
  void invert(std::size_t matrixSize) { printf("%s", __FUNCTION__); };
};
template <typename T, std::size_t n>
class SquareMatrix : private SquareMatrixBase<T> {
private:
  using SquareMatrixBase<T>::invert; // must using SquareMatrixBase<T>::invert;

public:
  void invert() { this->invert(n); }
  void invert2() { invert(n); }
};

int main() {
  SquareMatrix<float, 5> mat;
  mat.invert();
  mat.invert2();
}