// Online C++ compiler to run C++ program online
#include <stdio.h>

#include <iostream>
template <typename T>
struct list_node {
    list_node *prev;
    list_node *next;
    T _var;

    list_node(T &&var) : _var(std::move(var)) {}
};

template <typename T>
class list {
    using node_t = list_node<T>;
    node_t *head;

   public:
    list() { head = nullptr; }

    bool insert(int index, T &&var) {
        node_t *node = head;
        node_t *prev_node = nullptr;
        for (int i = 0; i < index; i++) {
            if (node) {
                prev_node = node;
                node = node->next;
                continue;
            } else {
                return false;
            }
        }
        node_t *new_node = new node_t(std::forward<T>(var));
        std::cout << "new list_node" << std::endl;
        new_node->prev = prev_node;
        new_node->next = node;

        if (node) {
            node->prev = new_node;
        }
        if (prev_node) {
            prev_node->next = new_node;
        } else {
            head = new_node;
        }

        std::cout << "insert finish" << std::endl;
        return true;
    }

    bool pop(int index) {
        node_t *node = head;
        for (int i = 0; i < index; i++) {
            if (node) {
                node = node->next;
                continue;
            } else {
                return false;
            }
        }
        if (node && node->next) {
            node->next->prev = node->prev;
            if (node->prev) {
                node->prev->next = node->next;
            }
        }

        if (node == head && node) {
            head = node->next;
        }
        free(node);
        return true;
    }

    ~list() {
        node_t *node = head;
        while (node) {
            node_t *cur = node;
            node = cur->next;
            free(cur);
        }
    }

    void span() {
        node_t *node = head;
        int i = 0;
        while (node) {
            printf(
                "node %6d var : %6d, prev : %10jx, cur: %10jx, next: %10jx\n",
                i, node->_var, node->prev, node, node->next);
            node = node->next;
            i++;
        }
    }
};

int main() {
    list<int> li;
    li.insert(0, -1);
    li.pop(0);
    li.pop(0);
    li.pop(1);
    li.pop(2);
    li.insert(0, -2);
    std::cout << "pop finish" << std::endl;
    li.span();
    li.insert(0, -2);
    li.insert(3, 3);
    li.insert(2, 2);
    li.insert(3, 3);
    li.insert(0, -3);
    li.insert(0, -4);
    li.span();
    li.pop(0);
    std::cout << "pop finish" << std::endl;
    li.span();

    return 0;
}