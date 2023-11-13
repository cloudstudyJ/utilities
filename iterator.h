#pragma once

template <typename T>
class Iterator {
    public:
        Iterator();
        Iterator(T* target);
        ~Iterator() noexcept;

        bool operator==(const Iterator<T>& other) const;
        bool operator!=(const Iterator<T>& other) const;
        explicit operator bool() const;

        decltype(auto) operator*();
        decltype(auto) operator*() const;

        Iterator<T>& operator++();      // prefix
        Iterator<T>  operator++(int);   // postfix

    private:
        T* mTarget{ };
};

template <typename T> Iterator<T>::Iterator() { }
template <typename T> Iterator<T>::Iterator(T* target)
    : mTarget{target} { }
template <typename T> Iterator<T>::~Iterator() noexcept { }

template <typename T> bool Iterator<T>::operator==(const Iterator<T>& other) const {
    if (mTarget == other.mTarget)
        return true;

    return false;
}
template <typename T> bool Iterator<T>::operator!=(const Iterator<T>& other) const { return !(*this == other); }
template <typename T> Iterator<T>::operator bool() const { return (mTarget != nullptr); }

template <typename T> decltype(auto) Iterator<T>::operator*() {
    if (*this)
        return mTarget->data;

    return nullptr;
}
template <typename T> decltype(auto) Iterator<T>::operator*() const {
    if (*this)
        return mTarget->data;

    return nullptr;
}

/**
 * @brief
 *     다음 노드 객체로 이동한 후의 Iterator 참조를 반환한다.
 *     노드 객체에는 다음 노드 객체를 가리키는 next 변수가 필요하다.
 *
 * @tparam T 노드 객체
 *
 * @return Iterator<T>& : 다음 노드 객체로 이동한 뒤의 Iterator 참조 반환
 */
template <typename T> Iterator<T>& Iterator<T>::operator++() {
    if (*this)
        mTarget = mTarget->next;

    return *this;
}
/**
 * @brief
 *     현재 노드 객체의 Iterator를 반환한 후 다음 노드 객체로 이동한다.
 *     노드 객체에는 다음 노드 객체를 가리키는 next 변수가 필요하다.
 *
 * @tparam T 노드 객체
 *
 * @return Iterator<T> : 다음 노드 객체로 이동하기 전의 Iterator
 */
template <typename T> Iterator<T> Iterator<T>::operator++(int) {
    Iterator<T> it(mTarget);

    if (*this)
        mTarget = mTarget->next;

    return it;
}