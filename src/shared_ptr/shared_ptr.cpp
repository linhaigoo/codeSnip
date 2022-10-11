/*
 * @Author: linhaigoo linhaigoo2013@gmail.com
 * @Date: 2022-09-20 00:03:14
 * @LastEditors: linhaigoo linhaigoo2013@gmail.com
 * @LastEditTime: 2022-09-20 00:42:34
 * @FilePath: /codeSnip/src/shared_ptr/shared_ptr.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 * 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <iostream>
#include <memory>
#include <type_traits>

class B {
public:
  ~B() { std::cout << "~B\n"; }
};

class D : public B {
public:
  ~D() { std::cout << "~D\n"; }
};

class E : public B {
public:
  ~E() { std::cout << "~E\n"; }
};

int main() {
  std::cout << "raw_pointer\n";
  B *raw_pointer = new D();
  delete raw_pointer;

  std::cout << "unique_pointer\n";
  std::unique_ptr<B> unique_pointer = std::unique_ptr<B>(new D());
  unique_pointer = nullptr;
  std::cout << "make_unique\n";
  //   std::unique_ptr<B> unique_pointer_use_make_unique =
  //   std::make_unique<D>(); unique_pointer_use_make_unique = nullptr;
  std::cout << "shared_ptr\n";
  std::shared_ptr<B> shared_pointerD = std::shared_ptr<B>(new D());
  std::shared_ptr<B> shared_pointerE = std::shared_ptr<B>(new E());
  swap(shared_pointerD, shared_pointerE);
  shared_pointerD = nullptr;
  shared_pointerE = nullptr;
  std::cout << "make_shared\n";
  std::shared_ptr<B> shared_pointer_use_make_shared = std::make_shared<D>();
  shared_pointer_use_make_shared = nullptr;

  return 0;
}
