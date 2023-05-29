#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>
#include "exceptions.hpp"

namespace sjtu {

/**
 * a container like std::priority_queue which is a heap internal.
 */
    template<typename T, class Compare = std::less<T>>
    class priority_queue {
    private:
        struct pq_node {
            pq_node *l_son = nullptr, *r_son = nullptr;
            T *data;
            int npl = 0;
            pq_node(const T &e) : data(new T(e)) {}
            ~pq_node() { delete data; }
        };

        pq_node *root;
        size_t _size;

        pq_node *copy(pq_node *other) {
            if (other == nullptr) return nullptr;
            pq_node *p = new pq_node(*(other->data));
            p->l_son = copy(other->l_son);
            p->r_son = copy(other->r_son);
            return p;
        }

        void clear(pq_node *p) {
            if (p == nullptr) return;
            clear(p->l_son);
            clear(p->r_son);
            delete p;
        }

        pq_node *merge(pq_node *a, pq_node *b) {
            if (a == nullptr) return b;
            if (b == nullptr) return a;
            if (Compare()(*(a->data), *(b->data))) {
                pq_node *tmp = a;
                a = b;
                b = tmp;
            }
            a->r_son = merge(a->r_son, b);
            if (a->l_son == nullptr || a->l_son->npl < a->r_son->npl) {
                pq_node *tmp = a->l_son;
                a->l_son = a->r_son;
                a->r_son = tmp;
            }
            if (a->l_son == nullptr || a->r_son == nullptr) a->npl = 0;
            else a->npl = std::min(a->l_son->npl, a->r_son->npl) + 1;
            return a;
        }

    public:
        /**
         * TODO constructors
         */
        priority_queue() : root(nullptr), _size(0) {}

        priority_queue(const priority_queue &other) {
            _size = other._size;
            root = copy(other.root);
        }

        /**
         * TODO deconstructor
         */
        ~priority_queue() {
            clear(root);
            root = nullptr;
            _size = 0;
        }

        /**
         * TODO Assignment operator
         */
        priority_queue &operator=(const priority_queue &other) {
            if (this == &other) return *this;
            clear(root);
            _size = other._size;
            root = copy(other.root);
            return *this;
        }

        /**
         * get the top of the queue.
         * @return a reference of the top element.
         * throw container_is_empty if empty() returns true;
         */
        const T &top() const {
            if (empty()) throw sjtu::container_is_empty();
            return *(root->data);
        }

        /**
         * TODO
         * push new element to the priority queue.
         */
        void push(const T &e) {
            pq_node *p = new pq_node(e);
            root = merge(root, p);
            _size++;
        }

        /**
         * TODO
         * delete the top element.
         * throw container_is_empty if empty() returns true;
         */
        void pop() {
            if (empty()) throw sjtu::container_is_empty();
            pq_node *p = root;
            root = merge(root->l_son, root->r_son);
            delete p;
            _size--;
        }

        /**
         * return the number of the elements.
         */
        size_t size() const { return _size; }

        /**
         * check if the container has at least an element.
         * @return true if it is empty, false if it has at least an element.
         */
        bool empty() const { return _size == 0; }

        /**
         * merge two priority_queues with at least O(logn) complexity.
         * clear the other priority_queue.
         */
        void merge(priority_queue &other) {
            root = merge(root, other.root);
            _size += other._size;
            other.root = nullptr;
            other._size = 0;
        }
    };

}

#endif