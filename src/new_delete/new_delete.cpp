// Type your code here, or load an example.
#include <stdio.h>

#include <iostream>
#include <memory>
#include <new>
typedef void (*new_hanlder)(void);

class widget {
   public:
    void* operator new(size_t size) {
        std::cout << "new size_t " << size << std::endl;
        // if (size == 0) {
        //     size = 1;
        // }
        if (size != sizeof(widget)) return ::operator new(size);
        while (true) {
            if (void* ptr = malloc(size)) {
                std::cout << "new ptr " << ptr << std::endl;
                return ptr;
            }
            new_hanlder prehandler = std::set_new_handler(nullptr);
            std::set_new_handler(prehandler);

            if (prehandler) {
                (*prehandler)();
            } else {
                throw std::bad_alloc();
            }
        }
    }

    void operator delete(void* mem) noexcept {
        std::cout << "delete(void* mem)" << mem << std::endl;
        if (mem != nullptr) {
            free(mem);
        }
    }

    void* operator new(size_t size, void* mem) {
        std::cout << "new(size_t size, void* mem)"
                  << "size " << size << " mem " << mem << std::endl;
        return mem;
    }

    // void operator delete(void* memo, void* mem) noexcept {
    //     std::cout << "delete(void* memo, void* mem)" << memo << std::endl;
    // }
};

class widgetA : public widget {
   public:
    using widget::operator new;//unhide all new even if name overlapped
    void* operator new(size_t size) {
        return widget::operator new(size);
    }  // if uncomment will hide new(size_t, void *)
    widgetA() {
        std::cout << "widgetA exception" << std::endl;
        throw std::bad_alloc();
    }
};

int main() {
    char a[2000];
    char b[2000];
    std::cout << "a[2000]" << &a << std::endl;
    std::cout << "b[2000]" << &b << std::endl;
    auto wp = new (a) widget;
    auto w = new widget;
    try {
        auto wap = new (b) widgetA;
    } catch (...) {
        std::cout << "new constructor exception catched" << std::endl;
    }

    delete w;
    // delete wp;
}