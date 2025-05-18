#ifndef SMARTPOINTER_H
#define SMARTPOINTER_H

template <typename T>
class SmartPointer {
private:
    T* ptr_;

public:
    explicit SmartPointer(T* ptr = nullptr);

    ~SmartPointer();

    SmartPointer(const SmartPointer&) = delete;

    SmartPointer& operator=(const SmartPointer&) = delete;

    SmartPointer(SmartPointer&& other) noexcept;

    SmartPointer& operator=(SmartPointer&& other) noexcept;

    T& operator*() const;

    T* operator->() const;

    explicit operator bool() const;

    T* get() const;

    T* release();

    void reset(T* ptr = nullptr);
};

#include "smartpointer.inl"

#endif // SMARTPOINTER_H
