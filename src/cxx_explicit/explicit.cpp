/*
 * @Author: Kian Liu
 * @Date: 2021-09-01 15:54:30
 * @LastEditTime: 2021-09-01 16:16:55
 * @LastEditors: Kian Liu
 * @Description:
 * @FilePath: /codeSnip/src/cxx_newfeature/explicit.cpp
 */
#include <iostream>
using namespace std;

class Point
{
  public:
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y)
    {
    }
};

class PointExplicit
{
  public:
    int x, y;
    explicit PointExplicit(int x = 0, int y = 0) : x(x), y(y)
    {
    }
};

template <typename T> void displayPoint(const T &p)
{
    cout << "(" << p.x << "," << p.y << ")" << endl;
}

int main()
{
    displayPoint<Point>(1);
    Point p = 1;
    displayPoint<PointExplicit>(PointExplicit(1));
    PointExplicit pExplicit = PointExplicit(1);
    decltype(p) pDecl;
    displayPoint<decltype(p)>(pDecl);
}
