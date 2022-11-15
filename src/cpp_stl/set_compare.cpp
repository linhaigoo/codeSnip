/*
 * @Author: linhaigoo linhaigoo2013@gmail.com
 * @Date: 2022-11-12 20:05:10
 * @LastEditors: linhaigoo linhaigoo2013@gmail.com
 * @LastEditTime: 2022-11-13 00:55:37
 * @FilePath: /codeSnip/src/cpp_stl/setCompareTest.cpp
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
bool comparePtrString(const string *ptr1, const string *ptr2) {
  return *ptr1 < *ptr2;
}

auto comparePtrStringLambda = [](const string *ptr1, const string *ptr2) {
  return comparePtrString(ptr1, ptr2);
};

template <typename T> class comparePtrContent {
public:
  bool operator()(const T *ptr1,
                  const T *ptr2) const // const method here to supress warning
  {
    return *ptr1 < *ptr2;
  }
};

class comparePtrContentMethod {
public:
  template <typename T>
  bool operator()(const T *ptr1,
                  const T *ptr2) const // const method here to supress warning
  {
    std::cout << "use const T * version" << std::endl;
    return *ptr1 < *ptr2;
  }
};

class comparePtrStringAndRetReferenceClass {
public:
  bool
  operator()(const string *ptr1,
             const string *ptr2) const // const method here to supress warning
  {
    return comparePtrString(ptr1, ptr2);
  }

  const string &operator()(const string *ptr) { return *ptr; }
};

class RetReferenceClass {
public:
  const string &operator()(const string *ptr) { return *ptr; }
};

int main() {
  //   typedef set<string *, decltype(comparePtrString)> sSet;
  typedef set<string *, decltype(comparePtrStringLambda)> slambdaSet;
  typedef set<string *, comparePtrStringAndRetReferenceClass> sclassSet;

  // sSet myyyset(comparePtrString);
  slambdaSet myyset(comparePtrStringLambda);
  myyset.insert(new string("ice2"));
  myyset.insert(new string("judge2"));
  myyset.insert(new string("hello2"));
  myyset.insert(new string("kity2"));

  transform(myyset.begin(), myyset.end(), ostream_iterator<string>(cout, "\n"),
            [](const string *ptr) { return *ptr; });

  sclassSet myset;
  myset.insert(new string("ice1"));
  myset.insert(new string("judge1"));
  myset.insert(new string("hello1"));
  myset.insert(new string("kity1"));

  transform(myset.begin(), myset.end(), ostream_iterator<string>(cout, "\n"),
            comparePtrStringAndRetReferenceClass());
  transform(myset.begin(), myset.end(), ostream_iterator<string>(cout, "\n"),
            RetReferenceClass());

  typedef set<string *, comparePtrContent<string>> sclassTemplateSet;
  sclassTemplateSet myyyyset;
  myyyyset.insert(new string("ice4"));
  myyyyset.insert(new string("judge4"));
  myyyyset.insert(new string("hello4"));
  myyyyset.insert(new string("kity4"));
  transform(myyyyset.begin(), myyyyset.end(),
            ostream_iterator<string>(cout, "\n"), RetReferenceClass());

  typedef set<string *, comparePtrContentMethod> smethodTemplateSet;
  smethodTemplateSet myyyyyset;
  myyyyyset.insert(new string("ice5"));
  myyyyyset.insert(new string("judge5"));
  myyyyyset.insert(new string("hello5"));
  myyyyyset.insert(new string("kity5"));
  transform(myyyyyset.begin(), myyyyyset.end(),
            ostream_iterator<string>(cout, "\n"), RetReferenceClass());
}
