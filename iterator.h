#pragma once

#include "./typeHandler.h"

/**
 * @brief
 *     링크 리스트 기반의 자료구조에 대한 반복자 클래스다.
 *     노드 객체에는 다음 노드를 가리키는 next 변수가 필요하다.
 *
 * @tparam T 노드 객체
 */
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
        // decltype(auto) operator*() const;    const 추가하는 방법?

        Iterator<T>& operator++();      // prefix
        Iterator<T>  operator++(int);   // postfix

    private:
        T* mTarget{ };
};

template <typename T> Iterator<T>::Iterator() { }
template <typename T> Iterator<T>::Iterator(T* target)
    : mTarget{target} { }
template <typename T> Iterator<T>::~Iterator() noexcept { }

/**
 * @brief
 *     호출 객체와 다른 객체의 노드가 동일한지 확인한다.
 *
 * @tparam T 노드 객체
 *
 * @param other 호출자와 다른 객체
 *
 * @return true  : 동일한 노드를 가리키는 경우
 * @return false : 서로 다른 노드를 가리키는 경우
 */
template <typename T> bool Iterator<T>::operator==(const Iterator<T>& other) const {
    if (mTarget == other.mTarget)
        return true;

    return false;
}
template <typename T> bool Iterator<T>::operator!=(const Iterator<T>& other) const { return !(*this == other); }
/**
 * @brief
 *     호출 객체의 노드가 비어있는지 확인한다.
 *
 * @return true  : 노드가 비어있지 않은 경우
 * @return false : 비어있는 노드인 경우
 */
template <typename T> Iterator<T>::operator bool() const { return (mTarget != nullptr); }

/**
 * @brief
 *     호출 객체의 노드의 값을 반환한다.
 *
 * @tparam T 노드 객체
 *
 * @return decltype(auto) : 노드 객체가 비어있으면 nullptr 반환. 비어있지 않으면 데이터 값 반환
 */
template <typename T> decltype(auto) Iterator<T>::operator*() {
    if (*this)
        return mTarget->data;

    return nullptr;
}

/**
 * @brief
 *     다음 노드 객체로 이동한 후의 Iterator 참조를 반환한다.
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
 *
 * @tparam T 노드 객체
 *
 * @return Iterator<T> : 다음 노드 객체로 이동하기 전의 Iterator 반환
 */
template <typename T> Iterator<T> Iterator<T>::operator++(int) {
    Iterator<T> it(mTarget);

    if (*this)
        mTarget = mTarget->next;

    return it;
}