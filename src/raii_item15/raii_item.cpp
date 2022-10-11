/*
 * @Author: linhaigoo linhaigoo2013@gmail.com
 * @Date: 2022-09-26 23:53:01
 * @LastEditors: linhaigoo linhaigoo2013@gmail.com
 * @LastEditTime: 2022-09-27 23:47:25
 * @FilePath: /codeSnip/src/raii_item15/raii_item.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 * 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
typedef void *FontHandle;
int font = 0;
void releaseFont(FontHandle fh) { printf("releaseFont"); }
FontHandle getFont() {
  printf("getFont");
  return &font;
}
class Font {
public:
  explicit Font(FontHandle fh) : f(fh) {}
  ~Font() { releaseFont(f); }
  //   explict operator FontHandle() const { return f; }
  operator FontHandle() const { return f; }

private:
  FontHandle f;
};

FontHandle getFontHandle(FontHandle para) { return para; }

int main() {
  Font f1(getFont());
  FontHandle f2 = getFontHandle(f1);
  return 0;
}