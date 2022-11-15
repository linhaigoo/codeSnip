#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <vector>
#include <iterator>


int main(void) {
    std::vector<int> v = {1, 2, 3, 4};
    std::for_each(v.begin(), v.end(), [&v](auto const& elem) {
        auto i = &elem - &v[0];
        // auto i = std::distance(elem, v.begin());//cannot compiled
        if (i == 2) return;
        std::cout << "i: " << i << std::endl;
        // cannot continue, break or return out of the loop
    });

    int ii = &v[1] - &v[0];
    std::cout << "ii: " << ii << std::endl;
    return 0;
}