#include <stdint.h>
#include <stdio.h>
#include <string>
#include <iostream>

template<unsigned n>
struct factorial
{
    enum{value =n*factorial<n-1>::value };
};

template<>
struct factorial<0>
{
    enum{value =1 };
};

template<unsigned n>
struct ffactorial
{
    // enum{value =n*factorial<n-1>::value };
    const unsigned value = n* ffactorial<n-1>().value;
};

template<>
struct ffactorial<0>
{
    const unsigned value = 1;
};

int main()
{
    std::cout<< ffactorial<10>().value<<std::endl;//cannot TMP
    std::cout<< factorial<10>::value<<std::endl;
    return 0;
}