#pragma once
#ifndef UNIQUE_PTR_H_INCLUDED
#define UNIQUE_PTR_H_INCLUDED

#include <stdexcept>

using namespace std;

template<typename T>
class My_Weak_Ptr;

class Weak_Ptr_is_Expired_Exception: public std::exception{};

template<typename T>
class Enable_Shared_From_This;

template <typename T>
class My_Shared_Ptr;

class Standart_Deleter{
public:

    template <typename T>
    void operator()(T* ptr){
        delete ptr;
    }
};

template <typename T, typename Deleter = Standart_Deleter>
class My_Unique_Ptr{
private:
    T* m_ptr;
    Deleter deleter;
public:
    friend class My_Shared_Ptr<T>;

    My_Unique_Ptr& operator=(const My_Unique_Ptr &new_ptr) = delete;
    My_Unique_Ptr(const My_Unique_Ptr &ptr) = delete;

    My_Unique_Ptr(): m_ptr(nullptr){
    }

    explicit My_Unique_Ptr(T *ptr): m_ptr(ptr){
    };

    My_Unique_Ptr(T *ptr, const Deleter &del): m_ptr(ptr), deleter(del){
    }

    My_Unique_Ptr(My_Unique_Ptr &&other) noexcept: My_Unique_Ptr(){
        swap(other);
        other.reset(nullptr);
    }

    My_Unique_Ptr& operator=(My_Unique_Ptr &&new_ptr) noexcept{
        swap(new_ptr);
        new_ptr.reset(nullptr);
        return *this;
    }

    T* get() const noexcept {
        return m_ptr;
    };

    T* release() {
        T* ptr = get();
        m_ptr = nullptr;
        return ptr;
    };

    void reset(T* new_ptr){
        if (m_ptr) {
            deleter (m_ptr);
        }
        m_ptr = new_ptr;
    };

    void swap(My_Unique_Ptr &other) noexcept{
        std::swap(m_ptr, other.m_ptr);
        std::swap(deleter, other.deleter);
    };

    template<typename ... R>
    static My_Unique_Ptr make_unique(R&& ... arg){
        T* new_obj = new T(std::forward<R>(arg)...);
        return My_Unique_Ptr(new_obj);
    }

    const Deleter& get_deleter() const noexcept{
        return deleter;
    }

    T* operator -> () const noexcept{
        return m_ptr;
    }

    T& operator * () const noexcept{
        return *m_ptr;
    }

    bool operator < (const My_Unique_Ptr &other) const noexcept{
        return (m_ptr < other.m_ptr);
    }

    bool operator > (const My_Unique_Ptr &other) const noexcept{
        return (m_ptr > other.m_ptr);
    }

    bool operator == (const My_Unique_Ptr &other) const noexcept{
        return (m_ptr == other.m_ptr);
    }

    bool operator != (const My_Unique_Ptr &other) const noexcept{
        return (m_ptr != other.m_ptr);
    }

    bool operator >= (const My_Unique_Ptr &other) const noexcept{
        return (m_ptr >= other.m_ptr);
    }

    bool operator <= (const My_Unique_Ptr &other) const noexcept{
        return (m_ptr <= other.m_ptr);
    }

    explicit operator bool() const noexcept{
        return (m_ptr != nullptr);
    }

    ~My_Unique_Ptr(){
        deleter (m_ptr);
    };
};

template<typename T>
class Base_Proxy{
public:
    T* ptr;
    size_t shared_cnt;
    size_t weak_cnt;
    explicit Base_Proxy(T* p):ptr(p), shared_cnt(0), weak_cnt(0){}
    virtual void Delete(T* p) = 0;
    virtual ~Base_Proxy() = default;
};

template<typename T, typename D>
class Proxy : public Base_Proxy<T>{
public:
    D deleter;
    virtual void Delete(T* p) override{
        deleter(p);
        p = nullptr;
    }
    Proxy(T* p, const D &del): Base_Proxy<T>(p), deleter(del){}
    explicit Proxy(T* p): Base_Proxy<T>(p){}
};

template<typename T>
class Make_Shared_Proxy : public Base_Proxy<T>{
public:
    char data[sizeof(T)];
    virtual void Delete(T* ptr) override{
        ptr->~T();
    }
    template <typename ... R>
    Make_Shared_Proxy(R&& ... arg): Base_Proxy<T>(reinterpret_cast<T*>(data)){
        new (Base_Proxy<T>::ptr) T(forward<R>(arg)...);
    }
};

template <typename T>
class My_Shared_Ptr{

private:
    Base_Proxy<T>* proxy;

    void decrease_shared_cnt(){
        --(proxy->shared_cnt);
        bool f = proxy->weak_cnt == 0;
        if (proxy->shared_cnt == 0){
            proxy->Delete(proxy->ptr);
            proxy->ptr = nullptr;
            if (f){
                delete proxy;
                proxy = nullptr;
            }
        }
    }

public:

    friend class My_Weak_Ptr<T>;

    My_Shared_Ptr() noexcept: proxy(nullptr){}

    void fill_shared_from_this(Enable_Shared_From_This<T> *p){
        p->w_ptr.proxy = proxy;
        ++(proxy->weak_cnt);
    }

    void fill_shared_from_this(...){};

    explicit My_Shared_Ptr(T* p){
        proxy = new Proxy<T, Standart_Deleter>(p);
        ++(proxy->shared_cnt);
        fill_shared_from_this(p);
    }

    template <typename D>
    My_Shared_Ptr(T* p, const D &del){
        proxy = new Proxy<T, D>(p, del);
        ++(proxy->shared_cnt);
        fill_shared_from_this(p);
    }

    My_Shared_Ptr(My_Shared_Ptr &&other) noexcept{
        swap(other);
        other.reset(nullptr);
    }

    My_Shared_Ptr(const My_Shared_Ptr &other) noexcept{
        proxy = other.proxy;
        if (proxy){
            ++(proxy->shared_cnt);
        }
    }

    template <typename D>
    My_Shared_Ptr(My_Unique_Ptr<T, D> &&other_unique): My_Shared_Ptr(other_unique.m_ptr, other_unique.deleter){
        other_unique.m_ptr = nullptr;
    }


    My_Shared_Ptr(const My_Weak_Ptr<T> &w_ptr){
        if(w_ptr.expired()){
            throw Weak_Ptr_is_Expired_Exception();
        }
        proxy = w_ptr.proxy;
        ++(proxy->shared_cnt);
    }

    template <typename ... R>
    static My_Shared_Ptr<T> Make_Shared(R&& ... arg){
        My_Shared_Ptr<T> to_make;
        to_make.proxy = new Make_Shared_Proxy<T>(forward<R>(arg)...);
        ++(to_make.proxy->shared_cnt);
        return to_make;
    }

    void reset(T* new_ptr){
        if (proxy){
            proxy->Delete(proxy->ptr);
        }
        proxy->ptr = new_ptr;
    };

    void swap(My_Shared_Ptr &other) noexcept{
        std::swap(proxy, other.proxy);
    };

    T* get() const noexcept{
        return proxy->ptr;
    }

    My_Shared_Ptr& operator=(const My_Shared_Ptr &other) noexcept{
        if (proxy == nullptr){
            proxy = other.proxy;
            if (proxy){
                ++(proxy->shared_cnt);
            }
            return *this;
        }
        if (proxy == other.proxy){
            return *this;
        }

        decrease_shared_cnt();
        proxy = other.proxy;
        if (proxy){
            ++(proxy->shared_cnt);
        }
        return *this;
    }

    T* operator -> () const noexcept{
        return proxy->ptr;
    }

    T& operator * () const noexcept{
        return *(proxy->ptr);
    }

    bool operator < (const My_Shared_Ptr &other) const noexcept{
        return (proxy->ptr < other.proxy->ptr);
    }

    bool operator > (const My_Shared_Ptr &other) const noexcept{
        return (proxy->ptr > other.proxy->ptr);
    }

    bool operator == (const My_Shared_Ptr &other) const noexcept{
        return (proxy->ptr == other.proxy->ptr);
    }

    bool operator != (const My_Shared_Ptr &other) const noexcept{
        return (proxy->ptr != other.proxy->ptr);
    }

    bool operator >= (const My_Shared_Ptr &other) const noexcept{
        return (proxy->ptr >= other.proxy->ptr);
    }

    bool operator <= (const My_Shared_Ptr &other) const noexcept{
        return (proxy->ptr <= other.proxy->ptr);
    }

    explicit operator bool() const noexcept{
        if (proxy == nullptr){
            return false;
        }
        return (proxy->ptr != nullptr);
    }

    ~My_Shared_Ptr(){
        if (proxy == nullptr){
            return;
        }
        decrease_shared_cnt();
    }
};

template <typename T>
class My_Weak_Ptr{

private:
    Base_Proxy<T>* proxy;

public:
    friend class My_Shared_Ptr<T>;

    My_Weak_Ptr(): proxy(nullptr){}

    My_Weak_Ptr(const My_Shared_Ptr<T> &sh_ptr): proxy(sh_ptr.proxy){
        if (proxy){
            ++(proxy->weak_cnt);
        }
    }

    My_Weak_Ptr(const My_Weak_Ptr &other) noexcept: proxy(other.proxy){
        if (proxy){
            ++(proxy->weak_cnt);
        }
    }

    bool expired() const{
        return ((proxy == nullptr) || (proxy->ptr == nullptr));
    }

    My_Shared_Ptr<T> lock() const{
        if (expired()){
            return My_Shared_Ptr<T>();
        }
        return My_Shared_Ptr<T> (*this);
    }

    ~My_Weak_Ptr(){
        if (proxy == nullptr){
            return;
        }
        --(proxy->weak_cnt);
        if ((proxy->weak_cnt == 0) && (proxy->shared_cnt == 0)){
            delete proxy;
            proxy = nullptr;
        }
    }

    My_Weak_Ptr& operator =(const My_Weak_Ptr<T> &other) noexcept{
        if (proxy == nullptr){
            proxy = other.proxy;
            if (proxy){
                ++(proxy->weak_cnt);
            }
            return *this;
        }
        if (proxy == other.proxy){
            return *this;
        }
        --(proxy->weak_cnt);
        if((proxy->weak_cnt == 0) && (proxy->shared_cnt == 0)){
            delete proxy;
        }
        proxy = other.proxy;
        if (proxy){
            ++(proxy->weak_cnt);
        }
        return *this;
    }
};

template<typename T>
class Enable_Shared_From_This{

private:

    My_Weak_Ptr<T> w_ptr;

public:

    friend class My_Shared_Ptr<T>;

    Enable_Shared_From_This(): w_ptr(){}

    My_Shared_Ptr<T> Shared_From_This(){
        return My_Shared_Ptr<T>(w_ptr);
    }
};
#endif // UNIQUE_PTR_H_INCLUDED
