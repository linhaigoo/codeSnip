#include <algorithm>
#include <cctype>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

struct probe {
    probe(const char*) { std::cout << "ctr " << std::endl; }
    probe(const probe&) { std::cout << "copy" << std::endl; }
    probe(probe&&) { std::cout << "move" << std::endl; }
};

void f_constRef(const probe& p) {
    probe q(p);  // copy
}

void f_value(probe p) {
    // probe q(p);
}

int main() {
    f_constRef("foo1");  // ctr
    f_value("foo2");     // ctr: implict conversion don't need copy again.
    probe foo3("foo3");  // ctr
    f_value(foo3);       // copy
}