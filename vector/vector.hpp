#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>

namespace sjtu {
    template<typename T>
    class vector {
    private:
        std::allocator<T> alloc;
        T *_data;
        int _size;
        int _capacity;
    public:
        class const_iterator;

        class iterator {
        private:
            T *_p;
            T *_head;
        public:

            iterator() : _p(nullptr) {}

            iterator(T * des, T * head) : _p(des), _head(head) {}

            T *Get() { return _p; }

            iterator operator+(const int &n) const {
                return iterator(this->_p+n, this->_head);
            }

            iterator operator-(const int &n) const {
                return iterator(this->_p-n, this->_head);
            }

            int operator-(const iterator &rhs) const {
                if (_head != rhs._head) throw sjtu::invalid_iterator();
                return _p - rhs._p;
            }

            iterator &operator+=(const int &n) {
                _p += n;
                return *this;
            }

            iterator &operator-=(const int &n) {
                _p -= n;
                return *this;
            }

            iterator operator++(int) {
                iterator tmp = *this;
                _p++;
                return tmp;
            }

            iterator &operator++() {
                _p++;
                return *this;
            }

            iterator operator--(int) {
                iterator tmp = *this;
                _p--;
                return tmp;
            }

            iterator &operator--() {
                _p--;
                return *this;
            }

            T &operator*() const {
                return *_p;
            }

            bool operator==(const iterator &rhs) const {
                return _p == rhs._p;
            }

            bool operator==(const const_iterator &rhs) const {
                return _p == rhs.Get();
            }

            bool operator!=(const iterator &rhs) const {
                return _p != rhs._p;
            }

            bool operator!=(const const_iterator &rhs) const {
                return _p != rhs.Get();
            }
        };

        class const_iterator {
        private:
            const T *_p;
            const T *_head;
        public:
            const_iterator() : _p(nullptr) {}

            const_iterator(const T * des, const T * head) : _p(des), _head(head) {}

            T *Get() { return _p; }
            const_iterator operator+(const int &n) const {
                return const_iterator(this->_p + n, this->_head);
            }

            const_iterator operator-(const int &n) const {
                return const_iterator(this->_p - n, this->_head);
            }

            int operator-(const const_iterator &rhs) const {
                if (_head != rhs._head) throw sjtu::invalid_iterator();
                return _p - rhs._p;
            }

            const_iterator &operator+=(const int &n) {
                _p += n;
                return *this;
            }

            const_iterator &operator-=(const int &n) {
                _p -= n;
                return *this;
            }

            const_iterator operator++(int) {
                const_iterator tmp = *this;
                _p++;
                return tmp;
            }

            const_iterator &operator++() {
                _p++;
                return *this;
            }

            const_iterator operator--(int) {
                const_iterator tmp = *this;
                _p--;
                return tmp;
            }

            const_iterator &operator--() {
                _p--;
                return *this;
            }

            const T &operator*() const {
                return *_p;
            }

            bool operator==(const iterator &rhs) const {
                return _p == rhs.Get();
            }

            bool operator==(const const_iterator &rhs) const {
                return _p == rhs._p;
            }

            bool operator!=(const iterator &rhs) const {
                return _p != rhs.Get();
            }

            bool operator!=(const const_iterator &rhs) const {
                return _p != rhs._p;
            }
        };

        vector() : _data(nullptr), _size(0), _capacity(0) {}

        vector(const vector &other) {
            if (other._size == 0) {
                _size = 0;
                _capacity = 0;
                _data = nullptr;
            } else {
                _size = other._size;
                _capacity = other._capacity;
                _data = alloc.allocate(_capacity + 1);
                for (int i = 0; i < _size; i++) alloc.construct(_data + i, other._data[i]);
            }
        }

        ~vector() {
            if (_data != nullptr) {
                for (int i = 0; i < _size; i++) alloc.destroy(_data + i);
                alloc.deallocate(_data, _capacity + 1);
                _data = nullptr;
            }
            _size = 0;
            _capacity = 0;
        }

        vector &operator=(const vector &other) {
            if (this == &other) return *this;
            if (_capacity < other._size) {
                if (_data != nullptr) {
                    for (int i = 0; i < _size; i++) alloc.destroy(_data + i);
                    alloc.deallocate(_data, _capacity + 1);
                    _data = nullptr;
                }
                _capacity = other._capacity;
                _data = alloc.allocate(_capacity + 1);
                _size = other._size;
            } else if (_size > other._size) {
                for (int i = 0; i < _size; i++) alloc.destroy(_data + i);
                _size = other._size;
            }
            for (int i = 0; i < _size; i++) alloc.construct(_data + i, other._data[i]);
            return *this;
        }

        T &at(const size_t &pos) {
            if (pos < 0 || pos >= _size) throw sjtu::index_out_of_bound();
            return _data[pos];
        }

        const T &at(const size_t &pos) const {
            if (pos < 0 || pos >= _size) throw sjtu::index_out_of_bound();
            return _data[pos];
        }

        T &operator[](const size_t &pos) {
            if (pos < 0 || pos >= _size) throw sjtu::index_out_of_bound();
            return _data[pos];
        }

        const T &operator[](const size_t &pos) const {
            if (pos < 0 || pos >= _size) throw sjtu::index_out_of_bound();
            return _data[pos];
        }

        const T &front() const {
            if (_size == 0) throw sjtu::container_is_empty();
            return _data[0];
        }

        const T &back() const {
            if (_size == 0) throw sjtu::container_is_empty();
            return _data[_size - 1];
        }

        iterator begin() {
            return iterator(_data, _data);
        }

        const_iterator cbegin() const {
            return const_iterator(_data, _data);
        }

        iterator end() {
            return iterator(_data + _size, _data);
        }

        const_iterator cend() const {
            return const_iterator(_data + _size, _data);
        }

        bool empty() const {
            if (_size > 0) return false;
            return true;
        }

        size_t size() const {
            return _size;
        }

        void clear() {
            for (int i = 0; i < _size; i++) alloc.destroy(_data + i);
            _size = 0;
        }

        iterator insert(iterator pos, const T &value) {
            int index = pos - begin();
            return insert(index,value);
        }

        iterator insert(const size_t &ind, const T &value) {
            if (ind > _size || ind < 0) throw sjtu::index_out_of_bound();
            if (_size == _capacity) {
                int LastCap = _capacity;
                if (_capacity == 0) {
                    _capacity = 5;
                } else {
                    _capacity *= 2;
                }
                T *new_data = alloc.allocate(_capacity + 1);
                for (int i = 0, j = 0; i < _size||_size == 0; i++) {
                    if (i == ind) {
                        alloc.construct(new_data + i, value);
                        _size++;
                    } else {
                        alloc.construct(new_data + i, _data[j]);
                        j++;
                    }
                }
                if (_data != nullptr) {
                    for (int i = 0; i < _size-1; i++) alloc.destroy(_data + i);
                    alloc.deallocate(_data, LastCap + 1);
                    _data = nullptr;
                }
                _data = new_data;
                return begin() + ind;
            }
            if (ind == _size) {
                alloc.construct(_data + _size, value);
            } else {
                alloc.construct(_data + _size, _data[_size - 1]);
                for (int i = _size - 1; i > ind; i--) _data[i] = _data[i - 1];
                _data[ind] = value;
            }
            _size++;
            return begin() + ind;
        }

        iterator erase(iterator pos) {
            int index = pos - begin();
            return erase(index);
        }

        iterator erase(const size_t &ind) {
            if (ind >= _size || ind < 0) throw sjtu::index_out_of_bound();
            _size--;
            for (size_t i = ind; i < _size; i++) _data[i] = _data[i + 1];
            alloc.destroy(_data + _size);
            return begin() + ind;
        }

        void push_back(const T &value) {
            if (_size == _capacity) {
                int LastCap = _capacity;
                if (_capacity == 0) {
                    _capacity = 5;
                } else {
                    _capacity *= 2;
                }
                T *new_data = alloc.allocate(_capacity + 1);
                for (int i = 0; i < _size; i++) alloc.construct(new_data + i, _data[i]);
                if (_data != nullptr) {
                    for (int i = 0; i < _size; i++) alloc.destroy(_data + i);
                    alloc.deallocate(_data, LastCap + 1);
                    _data = nullptr;
                }
                _data = new_data;
            }
            alloc.construct(_data + _size, value);
            _size++;
        }

        void pop_back() {
            if (_size == 0) {
                throw sjtu::container_is_empty();
            } else {
                _size--;
                alloc.destroy(_data + _size);
            }
        }
    };
}
#endif