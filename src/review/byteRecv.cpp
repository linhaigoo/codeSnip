// Type your code here, or load an example.
/*
int square(int num) {
    return num * num;
}
*/

#include <stdlib.h>

#include <iostream>
#include <string>

using namespace std;

void get_char(const char c) {
    static char* tmp = (char*)malloc(100);
    static int index = 0;
    static int state = 0;
    if (c != '\r' && c != '\n') tmp[index++] = c;
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
        tmp[index + 1] = '\0';
        printf("%s\r\n", tmp);
        state = 0;
        index = 0;
    }
}

int main() {
    const char chr[] = "1234\n\r6578\r\n1234\r\n";
    for (char c : chr) {
        get_char(c);
    }
}