template <typename T>
SmartPointer<T>::SmartPointer(T* ptr) : ptr_(ptr) {}

template <typename T>
SmartPointer<T>::~SmartPointer() {
    reset();
}

template <typename T>
SmartPointer<T>::SmartPointer(SmartPointer&& other) noexcept : ptr_(other.ptr_) {
    other.ptr_ = nullptr;
}

template <typename T>
SmartPointer<T>& SmartPointer<T>::operator=(SmartPointer&& other) noexcept {
    if (this != &other) {
        reset();
        ptr_ = other.ptr_;
        other.ptr_ = nullptr;
    }
    return *this;
}

template <typename T>
T& SmartPointer<T>::operator*() const {
    return *ptr_;
}

template <typename T>
T* SmartPointer<T>::operator->() const {
    return ptr_;
}

template <typename T>
SmartPointer<T>::operator bool() const {
    return ptr_ != nullptr;
}

template <typename T>
T* SmartPointer<T>::get() const {
    return ptr_;
}

template <typename T>
T* SmartPointer<T>::release() {
    T* ptr = ptr_;
    ptr_ = nullptr;
    return ptr;
}

template <typename T>
void SmartPointer<T>::reset(T* ptr) {
    if (ptr_ != ptr) {
        delete ptr_;
        ptr_ = ptr;
    }
}
