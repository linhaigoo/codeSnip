#include <algorithm>
#include <cctype>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

struct FILE_S {
    const char* fileName;
    FILE_S(const char* _fileName) { fileName = _fileName; }
};

FILE_S* fopen_ss(const char* _fileName) { return new FILE_S(_fileName); }

void fclose_ss(FILE_S* fp) {
    std::cout << "fclose(FILE_S* fp) fp: " << fp << std::endl;
}

struct FileCloserStruct {
    void operator()(FILE_S* fp) const {
        if (fp != nullptr) {
            fclose_ss(fp);
        }
    }
};

void FileCloserFunc(FILE_S* fp) {
    if (fp != nullptr) {
        fclose_ss(fp);
    }
}

auto FileCloserLambda = [](FILE_S* fp) {
    if (fp != nullptr) {
        fclose_ss(fp);
    }
};

int main() {
    std::unique_ptr<FILE_S, FileCloserStruct> uptr1(fopen_ss("test_file.txt"));
    std::cout << sizeof(uptr1) << std::endl;

    std::cout << "sizeof(uptr1) " << sizeof(uptr1) << std::endl;
    std::unique_ptr<FILE_S, void (*)(FILE_S*)> uptr2(fopen_ss("test_file1.txt"),
                                                     FileCloserFunc);

    std::cout << "sizeof(uptr2) " << sizeof(uptr2) << std::endl;

    std::unique_ptr<FILE_S, std::function<void(FILE_S*)>> uptr3(
        fopen_ss("test_file2.txt"), FileCloserLambda);

    std::cout << "sizeof(std::function<void(FILE_S*)>) "
              << sizeof(std::function<void(FILE_S*)>) << std::endl;
    std::cout << "sizeof(uptr3) " << sizeof(uptr3) << std::endl;
    std::unique_ptr<FILE_S, decltype(FileCloserLambda)> uptr4(
        fopen_ss("test_file3.txt"), FileCloserLambda);
    std::cout << "sizeof(uptr4) " << sizeof(uptr4) << std::endl;

    return 0;
}