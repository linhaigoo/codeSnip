/*
 * @Author: linhaigoo linhaigoo2013@gmail.com
 * @Date: 2022-11-13 21:51:01
 * @LastEditors: linhaigoo linhaigoo2013@gmail.com
 * @LastEditTime: 2022-11-13 22:41:37
 * @FilePath: /codeSnip/src/cpp_static/objcectCount.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 * 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <atomic>
#include <iostream>

class countObj {
  static std::atomic<int> count;
  int obj_id;

public:
  countObj() { obj_id = count.fetch_add(1); }
  countObj(const countObj &obj) { obj_id = count.fetch_add(1); }
  countObj &operator=(const countObj &obj) { return *this; }
  int getCount() { return obj_id; }
};

std::atomic<int> countObj::count{0};

static countObj obj1;
static countObj objArray[2];

int main() {
  countObj statckObj1;
  countObj copyCountObj(statckObj1);
  countObj statckObj3;

  std::cout << "obj1 count " << obj1.getCount() << std::endl;
  std::cout << "objArray[0] count " << objArray[0].getCount() << std::endl;
  std::cout << "objArray[1] count " << objArray[1].getCount() << std::endl;
  std::cout << "statckObj1 count " << statckObj1.getCount() << std::endl;
  std::cout << "copyCountObj count " << copyCountObj.getCount() << std::endl;
  std::cout << "statckObj3 count " << statckObj3.getCount() << std::endl;
  statckObj3 = statckObj1;
  std::cout << "statckObj3 count after copy " << statckObj3.getCount()
            << std::endl;
}