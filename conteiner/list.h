#ifndef LIST_H
#define LIST_H

#include <iterator>
#include <memory>
#include "../square.h"

namespace containers {


    template<class T, class Allocator = std::allocator<T>>
    class list {
    private:
        struct element;
        unsigned int size = 0;
    public:
        list() = default;

        class forward_iterator {
        public:
            using value_type = T;
            using reference = T&;
            using pointer = T*;
            using difference_type = std::ptrdiff_t; //для арифметики указателей и индексации массива
            using iterator_category = std::forward_iterator_tag;//пустой класс для идентификации прямого итератора
            explicit forward_iterator(element* ptr);
            T& operator*();
            forward_iterator& operator++();
            forward_iterator operator++(int);
            bool operator== (const forward_iterator& other) const;
            bool operator!= (const forward_iterator& other) const;

        private:
            element* it_ptr;
            friend list;

        };
        forward_iterator begin();
        forward_iterator end();
        void pop_back();
        void pop_front();
        void push_back(const T& value);
        size_t length();
        void delete_by_it(forward_iterator d_it);
        void delete_by_number(size_t N);
        void insert_by_it(forward_iterator ins_it, T& value);
        void insert_by_number(size_t N, T& value);
        T& operator[](size_t index) ;
        list& operator=(list&& other);
    private:
        using allocator_type = typename Allocator::template rebind<element>::other;

        struct deleter {
        private:
            allocator_type* allocator_;
        public:
            deleter(allocator_type* allocator) : allocator_(allocator) {}

            void operator() (element* ptr) {
                if (ptr != nullptr) {
                    std::allocator_traits<allocator_type>::destroy(*allocator_, ptr);
                    allocator_->deallocate(ptr, 1);
                }
            }

        };
        struct element {
            T value;
            std::shared_ptr<element> next_element = nullptr;
            std::shared_ptr<element> prev_element = nullptr;
            forward_iterator next();
        };
        allocator_type allocator_{};
        static std::shared_ptr<element> push_impl(std::shared_ptr<element> cur);
        static std::shared_ptr<element> pop_impl(std::shared_ptr<element> cur);
        std::shared_ptr<element> first = nullptr;
    };//===============================end-of-class-list======================================//

    template<class T, class Allocator>
    typename list<T, Allocator>::forward_iterator list<T, Allocator>::begin() {
        return forward_iterator(first.get());
    }

    template<class T, class Allocator>
    typename list<T, Allocator>::forward_iterator list<T, Allocator>::end() {
        return forward_iterator(nullptr);
    }

//=========================base-methods-of-list==========================================//
    template<class T, class Allocator>
    size_t list<T, Allocator>::length() {
        return size;
    }

    template<class T, class Allocator>
    std::shared_ptr<typename list<T, Allocator>::element> list<T, Allocator>::push_impl(std::shared_ptr<element> cur) {
        if (cur -> next_element != nullptr) {
            return push_impl(cur->next_element);
        }
        return cur;
    }

    template<class T, class Allocator>
    void list<T, Allocator>::pop_front() {
        if (size == 0) {
            throw std::logic_error ("stack is empty");
        }

        first = first->next_element;
        first->prev_element = nullptr;
        size--;
    }

    template<class T, class Allocator>
    void list<T, Allocator>::pop_back() {
        if (size == 0) {
            throw std::logic_error("can`t pop from empty list");
        }
        first = pop_impl(first);
        size--;
    }


    template<class T, class Allocator >
    std::shared_ptr<typename list<T, Allocator>::element> list<T, Allocator>::pop_impl(std::shared_ptr<element> cur) {
        if (cur->next_element != nullptr) {
            cur->next_element = pop_impl(cur->next_element);
            return cur;
        }
        return nullptr;
    }

    template<class T, class Allocator>
    void list<T, Allocator>::push_back(const T& value) {
        element* tmp = this->allocator_.allocate(1);
        std::allocator_traits<allocator_type>::construct(this->allocator_, tmp, value);
        if (first == nullptr){
            first = std::unique_ptr<element, deleter> (tmp, deleter{&this->allocator_});
        }else{
            std::swap(tmp->next_element, first);
            first = std::move(std::unique_ptr<element, deleter> (tmp, deleter{&this->allocator_}));
        }
        size++;
    }


//=================================advanced-methods========================================//

    template<class T, class Allocator >
    void list<T, Allocator>::delete_by_it(containers::list<T, Allocator>::forward_iterator d_it) { //удаление по итератору
        if (d_it.it_ptr == nullptr) {
            throw std::logic_error("попытка доступа к несуществующему элементу");
        }
        if (d_it == this->begin()) {
            this->pop_front();
            size --;
            return;
        }
        if (d_it == this->end()) {
            this->pop_back();
            size --;
            return;
        }
        d_it.it_ptr->prev_element->next_element = d_it.it_ptr->next_element;
        d_it.it_ptr->next_element->prev_element = d_it.it_ptr->prev_element;

        size--;
    }
//удаление по номеру
    template<class T, class Allocator>
    void list<T, Allocator>::delete_by_number(size_t N) {
        forward_iterator it = this->begin();
        for (size_t i = 1; i <= N; ++i) {
            ++it;
        }
        this->delete_by_it(it);
    }

    template<class T, class Allocator >
    void list<T, Allocator>::insert_by_it(containers::list<T, Allocator>::forward_iterator ins_it, T& value) {
        if (first != nullptr) {
            if (ins_it == this->begin()) {
                std::shared_ptr<element> tmp = std::shared_ptr<element>(new element{ value });
                tmp->next_element = first;
                first->prev_element = tmp;
                first = tmp;
                if (tmp->value.Area() > tmp->next_element->value.Area()) {
                    throw std::logic_error("Area is too big");
                }
                size++;
                return;
            }else {
                if (ins_it.it_ptr == nullptr) {
                    std::shared_ptr<element> tmp = std::shared_ptr<element>(new element{value});

                    tmp->prev_element = push_impl(first);
                    push_impl(first)->next_element = std::shared_ptr<element>(tmp);
                    if (tmp->value.Area() < tmp->prev_element->value.Area()) {
                        throw std::logic_error("Area is too low");
                    }
                    size++;
                    return;
                } else {
                    std::shared_ptr<element> tmp = std::shared_ptr<element>(new element{value});
                    tmp->prev_element = ins_it.it_ptr->prev_element;
                    tmp->next_element = ins_it.it_ptr->prev_element->next_element;
                    ins_it.it_ptr->prev_element = tmp;
                    tmp->prev_element->next_element = tmp;

                    if (tmp->value.Area() > tmp->next_element->value.Area()) {
                        throw std::logic_error("Area is too big");
                    }
                    if (tmp->value.Area() < tmp->prev_element->value.Area()) {
                        throw std::logic_error("Area is too low");
                    }
                }
            }
        } else first=std::shared_ptr<element>(new element{value});

        size++;
    }

    template<class T, class Allocator>
    void list<T, Allocator>::insert_by_number(size_t N, T& value) {
        forward_iterator it = this->begin();
        for (size_t i = 0; i < N; ++i) {
            ++it;
        }
        this->insert_by_it(it, value);
    }
//==============================iterator`s-stuff=======================================//
    template<class T, class Allocator >
    typename list<T, Allocator>::forward_iterator list<T, Allocator>::element::next() {
        return forward_iterator(this->next_element.get());
    }

    template<class T, class Allocator >
    list<T, Allocator>::forward_iterator::forward_iterator(containers::list<T, Allocator>::element *ptr) {
        it_ptr = ptr;
    }

    template<class T, class Allocator>
    T& list<T, Allocator>::forward_iterator::operator*() {
        return this->it_ptr->value;
    }

    template<class T, class Allocator>
    typename list<T, Allocator>::forward_iterator& list<T, Allocator>::forward_iterator::operator++() {
        if (it_ptr == nullptr) throw std::logic_error ("out of list borders");
        *this = it_ptr->next();
        return *this;
    }

    template<class T, class Allocator>
    typename list<T, Allocator>::forward_iterator list<T, Allocator>::forward_iterator::operator++(int) {
        forward_iterator old = *this;
        ++*this;
        return old;
    }

    template<class T, class Allocator >
    bool list<T, Allocator>::forward_iterator::operator==(const forward_iterator& other) const {
        return it_ptr == other.it_ptr;
    }
    template<class T, class Allocator >
    list<T, Allocator>& list<T, Allocator>::operator=(list<T, Allocator>&& other){
        size = other.size;
        first = std::move(other.first);
    }

    template<class T, class Allocator>
    bool list<T, Allocator>::forward_iterator::operator!=(const forward_iterator& other) const {
        return it_ptr != other.it_ptr;
    }

    template<class T, class Allocator>
    T& list<T, Allocator>::operator[](size_t index)  {
        if (index < 0 || index >= size) {
            throw std::out_of_range("out of list's borders");
        }
        forward_iterator it = this->begin();
        for (size_t i = 0; i < index; i++) {
            it++;
        }
        return *it;
    }

}
#endif //LIST_H