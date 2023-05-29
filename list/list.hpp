#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include "exceptions.hpp"
#include "algorithm.hpp"

#include <climits>
#include <cstddef>

namespace sjtu {
/**
 * a data container like std::list
 * allocate random memory addresses for data and they are doubly-linked in a list.
 */
    template<typename T>
    class list {
    protected:
        class node {
        public:
            /**
             * add data members and constructors & destructor
             */
            T *_data;
            node *pre;
            node *next;
            std::allocator<T> alloc;

            node() : pre(nullptr), next(nullptr) { _data = alloc.allocate(1); }
            node(const T &value) : pre(nullptr), next(nullptr) {
                _data = alloc.allocate(1);
                alloc.construct(_data, value);
            }
            node(const node &copy) : pre(nullptr), next(nullptr) {
                _data = alloc.allocate(1);
                alloc.construct(_data, *(copy._data));
            }
            ~node() {
                if (pre != nullptr && next != nullptr) alloc.destroy(_data);
                alloc.deallocate(_data, 1);
            }
        };

    protected:
        /**
         * add data members for linked list as protected members
         */
        node *head;
        node *tail;
        int _size;
        /**
         * insert node cur before node pos
         * return the inserted node cur
         */
        node *insert(node *pos, node *cur) {
            pos->pre->next = cur;
            cur->pre = pos->pre;
            cur->next = pos;
            pos->pre = cur;
            _size++;
            return cur;
        }
        /**
         * remove node pos from list (no need to delete the node)
         * return the removed node pos
         */
        node *erase(node *pos) {
            pos->pre->next = pos->next;
            pos->next->pre = pos->pre;
            _size--;
            return pos;
        }
    public:
        class const_iterator;

        class iterator {
        private:
            /**
             * TODO add data members
             *   just add whatever you want.
             */
            node *it;
            node *_same;
        public:
            iterator(node *value, node *head) : it(value), _same(head) {}
            iterator(const iterator &copy) : it(copy.it), _same(copy._same) {}

            node *GetIt() const { return it; }
            node *GetHead() const { return _same; }

            iterator &operator=(const iterator &copy) {
                if (&copy == this) return *this;
                it = copy.it;
                _same = copy._same;
                return *this;
            }
            /**
             * iter++
             */
            iterator operator++(int) {
                if (it->next == nullptr) throw sjtu::invalid_iterator();
                it = it->next;
                return iterator(it->pre, _same);
            }
            /**
             * ++iter
             */
            iterator &operator++() {
                if (it->next == nullptr) throw sjtu::invalid_iterator();
                it = it->next;
                return *this;
            }
            /**
             * iter--
             */
            iterator operator--(int) {
                if (it->pre->pre == nullptr) throw sjtu::invalid_iterator();
                it = it->pre;
                return iterator(it->next, _same);
            }
            /**
             * --iter
             */
            iterator &operator--() {
                if (it->pre->pre == nullptr) throw sjtu::invalid_iterator();
                it = it->pre;
                return *this;
            }
            /**
             * TODO *it
             * remember to throw if iterator is invalid
             */
            T &operator*() const {
                if (it->pre == nullptr || it->next == nullptr) throw sjtu::invalid_iterator();
                return *(it->_data);
            }
            /**
             * TODO it->field
             * remember to throw if iterator is invalid
             */
            T *operator->() const noexcept {
                if (it->pre == nullptr || it->next == nullptr) throw sjtu::invalid_iterator();
                return it->_data;
            }
            /**
             * a operator to check whether two iterators are same (pointing to the same memory).
             */
            bool operator==(const iterator &rhs) const { return it == rhs.it; }
            bool operator==(const const_iterator &rhs) const { return it == rhs.GetIt(); }
            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const { return it != rhs.it; }
            bool operator!=(const const_iterator &rhs) const { return it != rhs.GetIt(); }
        };
        /**
         * TODO
         * has same function as iterator, just for a const object.
         * should be able to construct from an iterator.
         */
        class const_iterator {
        private:
            /**
             * TODO add data members
             *   just add whatever you want.
             */
            const node *it;
            const node *_same;
        public:
            const_iterator(const node *value, const node *head) : it(value), _same(head) {}
            const_iterator(const const_iterator &copy) : it(copy.it), _same(copy._same) {}
            const_iterator(const iterator &copy) : it(copy.GetIt()), _same(copy.GetHead()) {}

            const node *GetIt() const { return it; }
            const node *GetHead() const { return _same; }

            const_iterator &operator=(const const_iterator &copy) {
                if (&copy == this) return *this;
                it = copy.it;
                _same = copy._same;
                return *this;
            }
            const_iterator operator++(int) {
                if (it->next == nullptr) throw sjtu::invalid_iterator();
                it = it->next;
                return const_iterator(it->pre, _same);
            }
            const_iterator &operator++() {
                if (it->next == nullptr) throw sjtu::invalid_iterator();
                it = it->next;
                return *this;
            }
            const_iterator operator--(int) {
                if (it->pre == _same) throw sjtu::invalid_iterator();
                it = it->pre;
                return const_iterator(it->next, _same);
            }
            const_iterator &operator--() {
                if (it->pre == _same) throw sjtu::invalid_iterator();
                it = it->pre;
                return *this;
            }

            const T &operator*() const {
                if (it->pre == nullptr || it->next == nullptr) throw sjtu::invalid_iterator();
                return *(it->_data);
            }
            const T *operator->() const noexcept {
                if (it->pre == nullptr || it->next == nullptr) throw sjtu::invalid_iterator();
                return it->_data;
            }

            bool operator==(const iterator &rhs) const { return it == rhs.GetIt(); }
            bool operator==(const const_iterator &rhs) const { return it == rhs.it; }
            bool operator!=(const iterator &rhs) const { return it != rhs.GetIt(); }
            bool operator!=(const const_iterator &rhs) const { return it != rhs.it; }
        };

        /**
         * TODO Constructs
         * Atleast two: default constructor, copy constructor
         */
        list() {
            head = new node;
            tail = new node;
            head->next = tail;
            tail->pre = head;
            _size = 0;
        }
        list(const list &other) {
            head = new node;
            tail = new node;
            tail->pre = head;
            head->next = tail;
            node *copy = other.head->next;
            for (; copy != other.tail; copy = copy->next) {
                node *p = new node(*(copy->_data));
                tail->pre->next = p;
                p->pre = tail->pre;
                p->next = tail;
                tail->pre = p;
            }
            _size = other._size;
        }
        /**
         * TODO Destructor
         */
        virtual ~list() {
            for (node *p = head->next; p != nullptr; p = p->next) delete p->pre;
            delete tail;
            _size = 0;
        }
        /**
         * TODO Assignment operator
         */
        list &operator=(const list &other) {
            if (&other == this) return *this;
            clear();
            for (const_iterator it = other.cbegin(); it != other.cend(); it++) {
                node *p = new node(*it);
                insert(tail, p);
            }
            return *this;
        }
        /**
         * access the first / last element
         * throw container_is_empty when the container is empty.
         */
        const T &front() const {
            if (_size == 0) throw sjtu::container_is_empty();
            return *(head->next->_data);
        }
        const T &back() const {
            if (_size == 0) throw sjtu::container_is_empty();
            return *(tail->pre->_data);
        }
        /**
         * returns an iterator to the beginning.
         */
        iterator begin() { return iterator(head->next, head); }
        const_iterator cbegin() const { return const_iterator(head->next, head); }
        /**
         * returns an iterator to the end.
         */
        iterator end() { return iterator(tail, head); }
        const_iterator cend() const { return const_iterator(tail, head); }
        /**
         * checks whether the container is empty.
         */
        virtual bool empty() const { return _size == 0; }
        /**
         * returns the number of elements
         */
        virtual size_t size() const { return _size; }
        /**
         * clears the contents
         */
        virtual void clear() {
            for (node *p = head->next->next; p != nullptr; p = p->next) delete p->pre;
            head->next = tail;
            tail->pre = head;
            _size = 0;
        }
        /**
         * insert value before pos (pos may be the end() iterator)
         * return an iterator pointing to the inserted value
         * throw if the iterator is invalid
         */
        virtual iterator insert(iterator pos, const T &value) {
            if (pos.GetIt() == head || pos.GetHead() != head) throw sjtu::invalid_iterator();
            node *p = new node(value);
            return iterator(insert(pos.GetIt(), p), head);
        }
        /**
         * remove the element at pos (the end() iterator is invalid)
         * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
         * throw if the container is empty, the iterator is invalid
         */
        virtual iterator erase(iterator pos) {
            if (_size == 0) throw sjtu::container_is_empty();
            if (pos.GetIt() == tail || pos.GetIt() == head || pos.GetHead() != head) throw sjtu::invalid_iterator();
            iterator ret(pos.GetIt()->next, head);
            delete erase(pos.GetIt());
            return ret;
        }
        /**
         * adds an element to the end
         */
        void push_back(const T &value) {
            node *p = new node(value);
            insert(tail, p);
        }
        /**
         * removes the last element
         * throw when the container is empty.
         */
        void pop_back() {
            if (_size == 0) throw sjtu::container_is_empty();
            delete erase(tail->pre);
        }
        /**
         * inserts an element to the beginning.
         */
        void push_front(const T &value) {
            node *p = new node(value);
            insert(head->next, p);
        }
        /**
         * removes the first element.
         * throw when the container is empty.
         */
        void pop_front() {
            if (_size == 0) throw sjtu::container_is_empty();
            delete erase(head->next);
        }
        /**
         * sort the values in ascending order with operator< of T
         */
        void sort() {
            if (_size == 0 || _size == 1) return;
            node *p = head->next;
            std::allocator<T> alloc;
            T *move = alloc.allocate(_size + 1);
            for (int i = 0; i < _size; i++) {
                alloc.construct(move + i, *(p->_data));
                p = p->next;
            }
            sjtu::sort<T>(move, move + _size, [](const T &a, const T &b) -> bool { return a < b; });
            p = head->next;
            for (int i = 0; i < _size; i++) {
                p->alloc.destroy(p->_data);
                p->alloc.construct(p->_data, move[i]);
                p = p->next;
            }
            for (int i = 0; i < _size; i++) alloc.destroy(move + i);
            alloc.deallocate(move, _size + 1);
        }
        /**
         * merge two sorted lists into one (both in ascending order)
         * compare with operator< of T
         * container other becomes empty after the operation
         * for equivalent elements in the two lists, the elements from *this shall always precede the elements from other
         * the order of equivalent elements of *this and other does not change.
         * no elements are copied or moved
         */
        void merge(list &other) {
            node *p1 = head->next;
            node *p2 = other.head->next;
            while (p1 != tail && p2 != other.tail) {
                if (*(p2->_data) < *(p1->_data)) {
                    node *p = p2;
                    while (p2 != other.tail && *(p2->_data) < *(p1->_data)) p2 = p2->next;
                    p->pre = p1->pre;
                    p1->pre->next = p;
                    p2->pre->next = p1;
                    p1->pre = p2->pre;
                } else {
                    p1 = p1->next;
                }
            }
            if (p2 != other.tail) {
                tail->pre->next = p2;
                p2->pre = tail->pre;
                other.tail->pre->next = tail;
                tail->pre = other.tail->pre;
            }
            _size += other._size;
            other.head->next = other.tail;
            other.tail->pre = other.head;
            other._size = 0;
        }
        /**
         * reverse the order of the elements
         * no elements are copied or moved
         */
        void reverse() {
            node *p = head->next;
            node *tmp;
            while (p != nullptr) {
                p->pre->pre = p;
                tmp = p->next;
                p->next = p->pre;
                p = tmp;
            }
            tmp = head;
            head = tail;
            tail = tmp;
            head->pre = nullptr;
            tail->next = nullptr;
        }
        /**
         * remove all consecutive duplicate elements from the container
         * only the first element in each group of equal elements is left
         * use operator== of T to compare the elements.
         */
        void unique() {
            if (_size == 1 || _size == 0) return;
            node *l = head->next;
            node *r = l->next;
            while (r != tail) {
                if (*(l->_data) == *(r->_data)) {
                    while (r != tail && *(l->_data) == *(r->_data)) {
                        r = r->next;
                        _size--;
                        delete r->pre;
                    }
                    l->next = r;
                    r->pre = l;
                } else {
                    l = l->next;
                    r = r->next;
                }
            }
        }
    };
}

#endif //SJTU_LIST_HPP
