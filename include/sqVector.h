
#ifndef sqVector_h
#define sqVector_h

#include <iostream>

template<typename T> class sqVector
{
protected:
    std::size_t size;
    std::size_t capacity;
    T* data;
public:
    // constructors/destructor
    sqVector(std::size_t sz=0) : size(sz), capacity(sz*2+2)
    {
        data = new T[capacity];
        if (data == nullptr) throw std::bad_alloc();
    }
    
    sqVector(T* arr, std::size_t sz) : size(sz), capacity(sz*2+2)
    {
        data = new T[capacity];
        if (data==nullptr) throw std::bad_alloc(); else std::copy(arr, arr+sz, data);
    }
    
    sqVector(const sqVector& t) : size(t.size), capacity(t.capacity)
    {
        data = new T[capacity];
        if (data==nullptr) throw std::bad_alloc(); else std::copy(t.data, t.data+t.size, data);
    }
    
    sqVector(const sqVector&& t) noexcept
    {
        data=nullptr;
        swap(*this, t);
    }
    
    ~sqVector()
    {
        delete[] data;
        data=nullptr;
    }
    // operator =
    sqVector& operator=(const sqVector& v)
    {
        if (&v==this) return *this;
        if (size != v.size)
        {
            T* p = new T[v.capacity];
            if (p == nullptr) throw std::bad_alloc();
            else {
                delete[] data; data = nullptr;
                data = p;
                size = v.size;
                capacity = v.capacity;
                p = nullptr;
            }
        }
        std::copy(v.data,v.data+v.size,data);
        return *this;
    }
    
    sqVector& operator=(const sqVector&& v) noexcept
    {
        delete[] data;
        data=nullptr;
        swap(*this, v);
        return *this;
    }
    
    friend void swap(sqVector &lhs, sqVector &rhs)
    {
        std::swap(lhs.size,rhs.size);
        std::swap(lhs.capacity,rhs.capacity);
        std::swap(lhs.data,rhs.data);
    }
    // sizes
    size_t size_() const noexcept { return size; };
    size_t capacity_() const noexcept { return capacity; };
    bool empty() const noexcept { return (size==0); };
    bool full() const noexcept { return (size==capacity); };
    // comparations
    bool operator==(const sqVector &t) const noexcept
    {
        if (size!=t.size) return 0;
        else for (std::size_t i=0;i<size;i++)
            if (data[i]!=t.data[i]) return 0;
        return 1;
    }
    bool operator!=(const sqVector &t) const noexcept
    {
        return (!(*this==t));
    }
    // indexation
    T operator[](std::size_t ind)
    {
        return data[ind];
    }
    const T operator[](std::size_t ind) const
    {
        return data[ind];
    }
    // methods
    T back() { if (!(this->empty())) return data[size-1]; else throw std::logic_error("Vector is empty"); };
    void resize(int new_size)
    {
        T* mem = new T[new_size * 2 + 2];
        if (mem != nullptr) {
            std::copy(data, data + capacity, mem);
            delete[] data;
            capacity = new_size * 2 + 2;
            data = mem;
            mem = nullptr;
        }
    }
    void push_back(T elem)
    {
        insert(elem,size);
    }
    void push_front(T elem)
    {
        insert(elem,0);
    }
    void pop_back()
    {
        erase(size);
    }
    void pop_front()
    {
        erase(0);
    }
    void insert(T elem, std::size_t ind)
    {
        if((this->full())) this->resize(size_t(size_t((int)size + 1)));
        for (std::size_t i = size; i > ind; i--) data[i] = data[i - 1];
        data[ind] = elem; size++;
    }
    void erase(std::size_t ind)
    {
        if (!(this->empty()))
        {
            for (std::size_t i=ind; i<size;i++)
                data[i]=data[i+1];
            size--;
        }
        else throw std::logic_error("Vector is empty!");
    }
};

#endif
