#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>

namespace sjtu {
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
    template<typename T>
    class vector {
    private:
        T *_data;
        size_t _size;
        size_t _capacity;
    public:
        /**
         * TODO
         * a type for actions of the elements of a vector, and you should write
         *   a class named const_iterator with same interfaces.
         */
        /**
         * you can see RandomAccessIterator at CppReference for help.
         */
        class const_iterator;
        class iterator {
        private:
            /**
             * TODO add data members
             *   just add whatever you want.
             */
             T *_p;
        public:

            iterator():_p(nullptr){}
            iterator(T *des):_p(des){}
            iterator operator+(const int &n) const {
                //TODO
                iterator tmp=*this;
                tmp._p+=n;
                return tmp;
            }
            iterator operator-(const int &n) const {
                //TODO
                iterator tmp=*this;
                tmp._p-=n;
                return tmp;
            }
            // return the distance between two iterators,
            // if these two iterators point to different vectors, throw invaild_iterator.
            int operator-(const iterator &rhs) const {
                //TODO
                return _p-rhs._p;
            }
            iterator& operator+=(const int &n) {
                //TODO
                _p+=n;
                return *this;
            }
            iterator& operator-=(const int &n) {
                //TODO
                _p-=n;
                return *this;
            }
            /**
             * TODO iter++
             */
            iterator operator++(int) {
                iterator tmp = *this;
                _p++;
                return tmp;
            }
            /**
             * TODO ++iter
             */
            iterator& operator++() {
                _p++;
                return *this;
            }
            /**
             * TODO iter--
             */
            iterator operator--(int) {
                iterator tmp = *this;
                _p--;
                return tmp;
            }
            /**
             * TODO --iter
             */
            iterator& operator--() {
                _p--;
                return *this;
            }
            /**
             * TODO *it
             */
            T& operator*() const{
                return *_p;
            }
            /**
             * a operator to check whether two iterators are same (pointing to the same memory address).
             */
            bool operator==(const iterator &rhs) const {
                return _p==rhs._p;
            }
            bool operator==(const const_iterator &rhs) const {
                return _p==rhs._p;
            }
            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {
                return _p!=rhs._p;
            }
            bool operator!=(const const_iterator &rhs) const {
                return _p!=rhs._p;
            }
        };
        /**
         * TODO
         * has same function as iterator, just for a const object.
         */
        class const_iterator {
        private:
            const T *_p;
        public:
            const_iterator():_p(nullptr){}
            const_iterator(const T *des):_p(des){}
            const_iterator operator+(const int &n) const {
                const_iterator tmp=*this;
                tmp._p+=n;
                return tmp;
            }
            const_iterator operator-(const int &n) const {
                const_iterator tmp=*this;
                tmp._p-=n;
                return tmp;
            }
            int operator-(const const_iterator &rhs) const {
                return _p-rhs._p;
            }
            const_iterator& operator+=(const int &n) {
                _p+=n;
                return *this;
            }
            const_iterator& operator-=(const int &n) {
                _p-=n;
                return *this;
            }

            const_iterator operator++(int) {
                const_iterator tmp = *this;
                _p++;
                return tmp;
            }

            const_iterator& operator++() {
                _p++;
                return *this;
            }

            const_iterator operator--(int) {
                const_iterator tmp = *this;
                _p--;
                return tmp;
            }

            const_iterator& operator--() {
                _p--;
                return *this;
            }

            const T& operator*() const{
                return *_p;
            }

            bool operator==(const iterator &rhs) const {
                return _p==rhs._p;
            }
            bool operator==(const const_iterator &rhs) const {
                return _p==rhs._p;
            }

            bool operator!=(const iterator &rhs) const {
                return _p!=rhs._p;
            }
            bool operator!=(const const_iterator &rhs) const {
                return _p!=rhs._p;
            }
        };

        vector():_data(nullptr),_size(0),_capacity(0){}
        vector(const vector &other) {
            if(other._size==0) {
                _size = 0;
                _capacity = 0;
                _data = nullptr;
            }
            else{
                _size=other._size;
                _capacity=other._capacity;
                _data=new T [_capacity];
                for(int i=0;i<_size;i++) _data[i]=other._data[i];
            }
        }

        ~vector() {
            if (_data!= nullptr){
                delete[] _data;
                _data= nullptr;
            }
            _size=0;
            _capacity=0;
        }
        /**
         * TODO Assignment operator
         */
        vector &operator=(const vector &other) {
            if(_capacity<other._size){
                if(_data!= nullptr) delete[] _data;
                _capacity=other._capacity;
                _data=new T [_capacity];
                _size=other._size;
            }else if(_size>other._size){
                for(int i=_size;i<other._size;i++) _data[i]=NULL;
                _size=other._size;
            }
            for(int i=0;i<_size;i++) _data[i]=other._data[i];
        }

        T & at(const size_t &pos) {
            if(pos<0||pos>=_size) throw sjtu::index_out_of_bound();
            return _data[pos];
        }
        const T & at(const size_t &pos) const {
            if(pos<0||pos>=_size) throw sjtu::index_out_of_bound();
            return _data[pos];
        }

        T & operator[](const size_t &pos) {
            if(pos<0||pos>=_size) throw sjtu::index_out_of_bound();
            return _data[pos];
        }
        const T & operator[](const size_t &pos) const {
            if(pos<0||pos>=_size) throw sjtu::index_out_of_bound();
            return _data[pos];
        }

        const T & front() const {
            if(_size==0) throw sjtu::container_is_empty();
            return _data[0];
        }

        const T & back() const {
            if(_size==0) throw sjtu::container_is_empty();
            return _data[_size-1];
        }

        iterator begin() {
            iterator beg(_data);
            return beg;
        }
        const_iterator cbegin() const {
            const_iterator cbeg(_data);
            return cbeg;
        }

        iterator end() {
            iterator en(_data+_size);
            return en;
        }
        const_iterator cend() const {
            const_iterator cen(_data+_size);
            return cen;
        }

        bool empty() const {
            if(_size>0) return false;
            return true;
        }

        size_t size() const {
            return _size;
        }

        void clear() {
            for(int i=0;i<_size;i++) _data[i]=NULL;
            _size=0;
        }
        /**
         * inserts value before pos
         * returns an iterator pointing to the inserted value.
         */
        iterator insert(iterator pos, const T &value) {
            int index=pos-begin();
            if(_size==_capacity){
                if (_capacity == 0) {
                    _capacity = 2;
                } else {
                    _capacity *= 2;
                }
                T *new_data = new T[_capacity];
                for (int i = 0,j = 0; i < _size; i++,j++) {
                    if(i==index) {
                        new_data[i]=value;
                        _size++;
                        j--;
                    }
                    else{
                        new_data[i] = _data[j];
                    }
                }
                if(index==_size) new_data[_size]=value;
                if (_data != nullptr) delete[] _data;
                _data = new_data;
                return begin()+index;
            }
            for(int i=_size;i>index;i--) _data[i]=_data[i-1];
            _data[index]=value;
            _size++;
            return begin()+index;
        }
        /**
         * inserts value at index ind.
         * after inserting, this->at(ind) == value
         * returns an iterator pointing to the inserted value.
         * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
         */
        iterator insert(const size_t &ind, const T &value) {
            if(ind>_size) throw sjtu::index_out_of_bound();
            if(_size==_capacity){
                if (_capacity == 0) {
                    _capacity = 2;
                } else {
                    _capacity *= 2;
                }
                T *new_data = new T[_capacity];
                for (size_t i = 0,j = 0; i < _size; i++,j++) {
                    if(i==ind) {
                        new_data[i]=value;
                        _size++;
                        j--;
                    }
                    else{
                        new_data[i] = _data[j];
                    }
                }
                if(ind==_size) new_data[_size]=value;
                if (_data != nullptr) delete[] _data;
                _data = new_data;
                return begin()+index;
            }
            for(size_t i=_size;i>ind;i--) _data[i]=_data[i-1];
            _data[index]=value;
            _size++;
            return begin()+index;
        }
        /**
         * removes the element at pos.
         * return an iterator pointing to the following element.
         * If the iterator pos refers the last element, the end() iterator is returned.
         */
        iterator erase(iterator pos) {
            int index=pos-begin();
            _size--;
            for(int i=index;i<_size;i++) _data[i]=_data[i+1];
            _data[_size]=NULL;
            return begin()+index;
        }
        /**
         * removes the element with index ind.
         * return an iterator pointing to the following element.
         * throw index_out_of_bound if ind >= size
         */
        iterator erase(const size_t &ind) {
            if(ind>=_size) throw sjtu::index_out_of_bound();
            _size--;
            for(size_t i=ind;i<_size;i++) _data[i]=_data[i+1];
            _data[_size]=NULL;
            return begin()+index;
        }

        void push_back(const T &value) {
            if(_size==_capacity) {
                if (_capacity == 0) {
                    _capacity = 5;
                } else {
                    _capacity *= 2;
                }
                T *new_data = new T[_capacity];
                for (int i = 0; i < _size; i++) {
                    new_data[i] = _data[i];
                }
                if (_data != nullptr) delete[] _data;
                _data = new_data;
            }
            _data[_size]=value;
            _size++;
        }
        void pop_back() {
            if(_size==0){
                throw sjtu::container_is_empty ();
            }
            else{
                _size--;
            }
        }
    };


}

#endif
