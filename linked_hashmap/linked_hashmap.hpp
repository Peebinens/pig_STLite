/**
 * implement a container like std::linked_hashmap
 */
#ifndef SJTU_LINKEDHASHMAP_HPP
#define SJTU_LINKEDHASHMAP_HPP

// only for std::equal_to<T> and std::hash<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {
    /**
     * In linked_hashmap, iteration ordering is differ from map,
     * which is the order in which keys were inserted into the map.
     * You should maintain a doubly-linked list running through all
     * of its entries to keep the correct iteration order.
     *
     * Note that insertion order is not affected if a key is re-inserted
     * into the map.
     */

    template<
            class Key,
            class T,
            class Hash = std::hash<Key>,
            class Equal = std::equal_to<Key>
    >
    class linked_hashmap {
    private:
        struct HashNode {
            pair<const Key, T> *data;
            HashNode *hash_next, *pre, *next;

            HashNode(const pair<const Key, T> &d, HashNode *hn = nullptr, HashNode *p = nullptr, HashNode *n = nullptr)
                    : hash_next(hn), pre(p), next(n) {
                data = new pair<const Key, T>(d);
            }
            HashNode() : hash_next(nullptr), pre(nullptr), next(nullptr), data(nullptr) {}
            ~HashNode() {
                if (data != nullptr) {
                    delete data;
                }
            }
        };

        size_t Capacity, CurrentSize;
        double LoadFactor;
        Hash MyHash;
        Equal MyEqual;
        HashNode *MyMap, *Head, *Tail;

    public:
        /**
         * the internal type of data.
         * it should have a default constructor, a copy constructor.
         * You can use sjtu::linked_hashmap as value_type by typedef.
         */
        typedef pair<const Key, T> value_type;

        /**
         * see BidirectionalIterator at CppReference for help.
         *
         * if there is anything wrong throw invalid_iterator.
         *     like it = linked_hashmap.begin(); --it;
         *       or it = linked_hashmap.end(); ++end();
         */
        class const_iterator;

        class iterator {
        private:
            /**
             * TODO add data members
             *   just add whatever you want.
             */
            linked_hashmap::HashNode *ptr;
        public:
            // The following code is written for the C++ type_traits library.
            // Type traits is a C++ feature for describing certain properties of a type.
            // For instance, for an iterator, iterator::value_type is the type that the
            // iterator points to.
            // STL algorithms and containers may use these type_traits (e.g. the following
            // typedef) to work properly.
            // See these websites for more information:
            // https://en.cppreference.com/w/cpp/header/type_traits
            // About value_type: https://blog.csdn.net/u014299153/article/details/72419713
            // About iterator_category: https://en.cppreference.com/w/cpp/iterator
            using difference_type = std::ptrdiff_t;
            using value_type = typename linked_hashmap::value_type;
            using pointer = value_type *;
            using reference = value_type &;
            using iterator_category = std::output_iterator_tag;

            friend class linked_hashmap<Key, T, Hash, Equal>;
            friend class const_iterator;

            iterator() {
                // TODO
                ptr = nullptr;
            }
            iterator(const iterator &other) {
                // TODO
                ptr = other.ptr;
            }
            iterator(linked_hashmap::HashNode *p) {
                ptr = p;
            }

            /**
             * TODO iter++
             */
            iterator operator++(int) {
                if (ptr->next == nullptr) {
                    throw sjtu::index_out_of_bound();
                }
                iterator temp = *this;
                ptr = ptr->next;
                return temp;
            }
            /**
             * TODO ++iter
             */
            iterator &operator++() {
                if (ptr->next == nullptr) {
                    throw sjtu::index_out_of_bound();
                }
                ptr = ptr->next;
                return *this;
            }
            /**
             * TODO iter--
             */
            iterator operator--(int) {
                if (ptr->pre->pre == nullptr) {
                    throw sjtu::index_out_of_bound();
                }
                iterator temp = *this;
                ptr = ptr->pre;
                return temp;
            }
            /**
             * TODO --iter
             */
            iterator &operator--() {
                if (ptr->pre->pre == nullptr) {
                    throw sjtu::index_out_of_bound();
                }
                ptr = ptr->pre;
                return *this;
            }

            /**
             * a operator to check whether two iterators are same (pointing to the same memory).
             */
            value_type &operator*() const { return *ptr->data; }

            bool operator==(const iterator &rhs) const { return ptr == rhs.ptr; }
            bool operator==(const const_iterator &rhs) const { return ptr == rhs.ptr; }
            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const { return ptr != rhs.ptr; }
            bool operator!=(const const_iterator &rhs) const { return ptr != rhs.ptr; }

            /**
             * for the support of it->first.
             * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
             */
            value_type *operator->() const noexcept { return ptr->data; }
        };

        class const_iterator {
            // it should has similar member method as iterator.
            //  and it should be able to construct from an iterator.
        private:
            // data members.
            linked_hashmap::HashNode *ptr;
        public:
            friend class linked_hashmap<Key, T, Hash, Equal>;
            friend class iterator;

            const_iterator() { ptr = nullptr; }
            const_iterator(const const_iterator &other) { ptr = other.ptr; }
            const_iterator(const iterator &other) { ptr = other.ptr; }

            const_iterator operator++(int) {
                if (ptr->next == nullptr) {
                    throw sjtu::index_out_of_bound();
                }
                const_iterator temp = *this;
                ptr = ptr->next;
                return temp;
            }
            const_iterator &operator++() {
                if (ptr->next == nullptr) {
                    throw sjtu::index_out_of_bound();
                }
                ptr = ptr->next;
                return *this;
            }
            const_iterator operator--(int) {
                if (ptr->pre->pre == nullptr) {
                    throw sjtu::index_out_of_bound();
                }
                const_iterator temp = *this;
                ptr = ptr->pre;
                return temp;
            }
            const_iterator &operator--() {
                if (ptr->pre->pre == nullptr) {
                    throw sjtu::index_out_of_bound();
                }
                ptr = ptr->pre;
                return *this;
            }

            const value_type &operator*() const { return *ptr->data; }

            bool operator==(const iterator &rhs) const { return ptr == rhs.ptr; }
            bool operator==(const const_iterator &rhs) const { return ptr == rhs.ptr; }
            bool operator!=(const iterator &rhs) const { return ptr != rhs.ptr; }
            bool operator!=(const const_iterator &rhs) const { return ptr != rhs.ptr; }

            value_type *operator->() const noexcept { return ptr->data; }
        };

        /**
         * TODO two constructors
         */
        linked_hashmap() : Capacity(20), CurrentSize(0), LoadFactor(0.618), MyMap(new HashNode[20]), Head(new HashNode),
                           Tail(new HashNode) {
            Head->next = Tail;
            Tail->pre = Head;
        }
        linked_hashmap(const linked_hashmap &other) : Capacity(20), CurrentSize(other.CurrentSize),
                                                      LoadFactor(0.618), Head(new HashNode), Tail(new HashNode) {
            while (Capacity * LoadFactor <= CurrentSize) Capacity = Capacity << 1;
            MyMap = new HashNode[Capacity];
            Head->next = Tail;
            Tail->pre = Head;
            for (HashNode *p = other.Head->next; p != other.Tail; p = p->next) {
                int pos = MyHash(p->data->first) % Capacity;
                MyMap[pos].hash_next = new HashNode(*p->data, MyMap[pos].hash_next, Tail->pre, Tail);
                Tail->pre->next = MyMap[pos].hash_next;
                Tail->pre = MyMap[pos].hash_next;
            }
        }

        /**
         * TODO assignment operator
         */
        linked_hashmap &operator=(const linked_hashmap &other) {
            if (&other == this) return *this;
            clear();
            if (other.CurrentSize == 0) { return *this; }
            delete[] MyMap;
            Capacity = 20;
            CurrentSize = other.CurrentSize;
            while (Capacity * LoadFactor <= CurrentSize) Capacity = Capacity << 1;
            MyMap = new HashNode[Capacity];
            for (HashNode *p = other.Head->next; p != other.Tail; p = p->next) {
                int pos = MyHash(p->data->first) % Capacity;
                MyMap[pos].hash_next = new HashNode(*p->data, MyMap[pos].hash_next, Tail->pre, Tail);
                Tail->pre->next = MyMap[pos].hash_next;
                Tail->pre = MyMap[pos].hash_next;
            }
            return *this;
        }

        /**
         * TODO Destructors
         */
        ~linked_hashmap() {
            clear();
            delete[] MyMap;
            delete Head;
            delete Tail;
        }

        /**
         * TODO
         * access specified element with bounds checking
         * Returns a reference to the mapped value of the element with key equivalent to key.
         * If no such element exists, an exception of type `index_out_of_bound'
         */
        T &at(const Key &key) {
            size_t pos = MyHash(key) % Capacity;
            for (HashNode *p = MyMap[pos].hash_next; p != nullptr; p = p->hash_next) {
                if (MyEqual(p->data->first, key)) return p->data->second;
            }
            throw sjtu::index_out_of_bound();
        }
        const T &at(const Key &key) const {
            size_t pos = MyHash(key) % Capacity;
            for (HashNode *p = MyMap[pos].hash_next; p != nullptr; p = p->hash_next) {
                if (MyEqual(p->data->first, key)) return p->data->second;
            }
            throw sjtu::index_out_of_bound();
        }

        /**
         * TODO
         * access specified element
         * Returns a reference to the value that is mapped to a key equivalent to key,
         *   performing an insertion if such key does not already exist.
         */
        T &operator[](const Key &key) {
            size_t pos = MyHash(key) % Capacity;
            for (HashNode *p = MyMap[pos].hash_next; p != nullptr; p = p->hash_next) {
                if (MyEqual((*p->data).first, key)) return p->data->second;
            }
            pair<Key, T> ins(key, T());
            if (CurrentSize >= Capacity * LoadFactor) {
                Capacity = Capacity << 1;
                pos = MyHash(key) % Capacity;
                HashNode *NewMap = new HashNode[Capacity];
                for (HashNode *p = Head->next; p != Tail; p = p->next) {
                    int ind = MyHash(p->data->first) % Capacity;
                    p->hash_next = NewMap[ind].hash_next;
                    NewMap[ind].hash_next = p;
                }
                delete[] MyMap;
                MyMap = NewMap;
            }
            MyMap[pos].hash_next = new HashNode(ins, MyMap[pos].hash_next, Tail->pre, Tail);
            Tail->pre->next = MyMap[pos].hash_next;
            Tail->pre = MyMap[pos].hash_next;
            CurrentSize++;
            return MyMap[pos].hash_next->data->second;
        }
        /**
         * behave like at() throw index_out_of_bound if such key does not exist.
         */
        const T &operator[](const Key &key) const {
            size_t pos = MyHash(key) % Capacity;
            for (HashNode *p = MyMap[pos].hash_next; p != nullptr; p = p->hash_next) {
                if (MyEqual(p->data->first, key)) return p->data->second;
            }
            throw sjtu::index_out_of_bound();
        }

        /**
         * return a iterator to the beginning
         */
        iterator begin() { return iterator(Head->next); }
        const_iterator cbegin() const { return const_iterator(Head->next); }

        /**
         * return a iterator to the end
         * in fact, it returns past-the-end.
         */
        iterator end() { return iterator(Tail); }
        const_iterator cend() const { return const_iterator(Tail); }

        /**
         * checks whether the container is empty
         * return true if empty, otherwise false.
         */
        bool empty() const { return CurrentSize == 0; }

        /**
         * returns the number of elements.
         */
        size_t size() const { return CurrentSize; }

        /**
         * clears the contents
         */
        void clear() {
            CurrentSize = 0;
            for (HashNode *p = Head->next; p != Tail;) {
                HashNode *q = p->next;
                delete p;
                p = q;
            }
            Head->next = Tail;
            Tail->pre = Head;
            for (int i = 0; i < Capacity; i++) { MyMap[i].hash_next = nullptr; }
        }

        /**
         * insert an element.
         * return a pair, the first of the pair is
         *   the iterator to the new element (or the element that prevented the insertion),
         *   the second one is true if insert successfully, or false.
         */
        pair<iterator, bool> insert(const value_type &value) {
            int pos = MyHash(value.first) % Capacity;
            for (HashNode *p = MyMap[pos].hash_next; p != nullptr; p = p->hash_next) {
                if (MyEqual(p->data->first, value.first)) {
                    return pair<iterator, bool>(iterator(p), false);
                }
            }
            if (CurrentSize >= Capacity * LoadFactor) {
                Capacity = Capacity << 1;
                pos = MyHash(value.first) % Capacity;
                HashNode *NewMap = new HashNode[Capacity];
                for (HashNode *p = Head->next; p != Tail; p = p->next) {
                    int ind = MyHash((*p->data).first) % Capacity;
                    p->hash_next = NewMap[ind].hash_next;
                    NewMap[ind].hash_next = p;
                }
                delete[] MyMap;
                MyMap = NewMap;
            }
            MyMap[pos].hash_next = new HashNode(value, MyMap[pos].hash_next, Tail->pre, Tail);
            Tail->pre->next = MyMap[pos].hash_next;
            Tail->pre = MyMap[pos].hash_next;
            CurrentSize++;
            return pair<iterator, bool>(iterator(MyMap[pos].hash_next), true);
        }

        /**
         * erase the element at pos.
         *
         * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
         */
        void erase(iterator pos) {
            if (pos.ptr->data == nullptr) throw sjtu::index_out_of_bound();
            size_t position = MyHash(pos.ptr->data->first) % Capacity;
            HashNode *q = MyMap + position;
            for (HashNode *p = MyMap[position].hash_next; p != nullptr; q = p, p = p->hash_next) {
                if (p == pos.ptr) {
                    p->next->pre = p->pre;
                    p->pre->next = p->next;
                    q->hash_next = p->hash_next;
                    delete p;
                    CurrentSize--;
                    return;
                }
            }
            throw sjtu::index_out_of_bound();
        }

        /**
         * Returns the number of elements with key
         *   that compares equivalent to the specified argument,
         *   which is either 1 or 0
         *     since this container does not allow duplicates.
         */
        size_t count(const Key &key) const {
            size_t pos = MyHash(key) % Capacity;
            for (HashNode *p = MyMap[pos].hash_next; p != nullptr; p = p->hash_next) {
                if (MyEqual(p->data->first, key)) return 1;
            }
            return 0;
        }

        /**
         * Finds an element with key equivalent to key.
         * key value of the element to search for.
         * Iterator to an element with key equivalent to key.
         *   If no such element is found, past-the-end (see end()) iterator is returned.
         */
        iterator find(const Key &key) {
            size_t pos = MyHash(key) % Capacity;
            for (HashNode *p = MyMap[pos].hash_next; p != nullptr; p = p->hash_next) {
                if (MyEqual(p->data->first, key)) return iterator(p);
            }
            return end();
        }

        const_iterator find(const Key &key) const {
            size_t pos = MyHash(key) % Capacity;
            for (HashNode *p = MyMap[pos].hash_next; p != nullptr; p = p->hash_next) {
                if (MyEqual(p->data->first, key)) return const_iterator(p);
            }
            return cend();
        }
    };

}

#endif