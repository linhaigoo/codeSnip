// Type your code here, or load an example.
/*
int square(int num) {
    return num * num;
}
*/

#include <stdio.h>
#include <stdlib.h>

using namespace std;

void pack_char(const char c) {
    static char* tmp = (char*)malloc(100);
    static int index = 0;
    static int state = 0;
    if (c != '\r' && c != '\n')
     tmp[index++] = c;
    switch (state) {
        case 0:
            if (c == '\r') {
                state = 1;
            }
            break;
        case 1:
            if (c == '\n') {
                state = 2;
            } else {
                state = 0;
            }
            break;
    }

    if (state == 2) {
        tmp[index] = '\0';
        printf("%s\r\n", tmp);
        state = 0;
        index = 0;
    }
}




int main()
{
  const char *str1 = "12345678\r\n910\n\r1234\r\n5678\r\n";//example 1
  char chr;
  while(( chr = *(str1++))!= '\0')
  {
    pack_char(chr);
  }
}