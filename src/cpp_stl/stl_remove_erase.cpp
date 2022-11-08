#include <algorithm>
#include <string>
#include <string_view>
#include <iostream>
#include <cctype>
 
int main()
{
    std::string str1 = "Text with some   spaces";
 
    auto noSpaceEnd = std::remove(str1.begin(), str1.end(), ' ');
 
    // The spaces are removed from the string only logically.
    // Note, we use view, the original string is still not shrunk:
    std::cout <<  str1.data()
              << " size: " << str1.size() << '\n';
 
    str1.erase(noSpaceEnd, str1.end());
 
    // The spaces are removed from the string physically.
    std::cout << str1 << " size: " << str1.size() << '\n';
 
    std::string str2 = "Text\n with\tsome \t  whitespaces\n\n";
    str2.erase(std::remove_if(str2.begin(), 
                              str2.end(),
                              [](unsigned char x){return std::isspace(x);}),
               str2.end());
    std::cout << str2 << '\n';
}