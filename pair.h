#pragma once

#include "./typeHandler.h"

/**
 * @brief
 *     비교 연산 대상 지정을 위한 구조체 타입
 */
namespace Compare {
    struct   Key{ };
    struct Value{ };

    using First  = Key;
    using Second = Value;

    template <typename T>
    constexpr inline bool isCompareTypes = isSame<T, First> || isSame<T, Second>;
}

/** TODO LISTS
 *      1. operator==, <, >의 실수형에 대한 처리
 */

/**
 * @brief
 *     두 개의 데이터를 쌍으로 저장하여 관리할 수 있게 해주는 클래스
 *
 * @tparam T1 첫 번째 데이터의 타입
 * @tparam T2 두 번째 데이터의 타입
 * @tparam COMPARE 비교연산 시 기준이 될 데이터 지정
 */
template <typename T1, typename T2, typename COMPARE = Compare::First, typename = enableIF<Compare::isCompareTypes<COMPARE>>>
class Pair {
    template <typename U>
    using otherObj = Pair<T1, T2, U>;

    public:
        Pair() noexcept;
        Pair(const T1& key, const T2& value) noexcept;
        ~Pair() noexcept;
        template <typename U> Pair(const otherObj<U>& other) noexcept;
        template <typename U> Pair(otherObj<U>&& other) noexcept;

        template <typename U> Pair& operator=(const otherObj<U>& other) noexcept;
        template <typename U> Pair& operator=(otherObj<U>&& other) noexcept;

        Pair& operator()(const T1& first, const T2& second) noexcept;

        explicit inline operator bool() const noexcept;

        template <typename U> bool operator==(const otherObj<U>& other) const noexcept;
        template <typename U> bool operator!=(const otherObj<U>& other) const noexcept;

        template <typename U> inline bool operator<(const otherObj<U>& other) const noexcept;
        template <typename U> inline bool operator>(const otherObj<U>& other) const noexcept;
        template <typename U> inline bool operator<=(const otherObj<U>& other) const noexcept;
        template <typename U> inline bool operator>=(const otherObj<U>& other) const noexcept;

        void swap() noexcept;

        inline void assign(const T1& first, const T2& second) noexcept;

        inline void key(const T1& key) noexcept;
        inline void value(const T2& value) noexcept;
        inline void first(const T1& first) noexcept;
        inline void second(const T2& second) noexcept;

        inline T1&   key() noexcept;
        inline T2& value() noexcept;
        inline const T1&   key() const noexcept;
        inline const T2& value() const noexcept;

        inline T1&  first() noexcept;
        inline T2& second() noexcept;
        inline const T1&  first() const noexcept;
        inline const T2& second() const noexcept;

    private:
        union {
            T1 mKey{ };
            T1 mFirst;
        };
        union {
            T2 mValue{ };
            T2 mSecond;
        };
};

template <typename T1, typename T2, typename COMPARE, typename E>
Pair<T1, T2, COMPARE, E>::Pair() noexcept { }
template <typename T1, typename T2, typename COMPARE, typename E> template <typename U>
Pair<T1, T2, COMPARE, E>::Pair(const otherObj<U>& other) noexcept { *this = other; }
template <typename T1, typename T2, typename COMPARE, typename E> template <typename U>
Pair<T1, T2, COMPARE, E>::Pair(otherObj<U>&& other) noexcept { *this = move(other); }
template <typename T1, typename T2, typename COMPARE, typename E>
Pair<T1, T2, COMPARE, E>::~Pair() noexcept { }
template <typename T1, typename T2, typename COMPARE, typename E>
Pair<T1, T2, COMPARE, E>::Pair(const T1& key, const T2& value) noexcept
    : mKey{key}
    , mValue{value} { }

template <typename T1, typename T2, typename COMPARE, typename E> template <typename U>
Pair<T1, T2, COMPARE, E>& Pair<T1, T2, COMPARE, E>::operator=(const otherObj<U>& other) noexcept {
    assign(other.key(), other.value());

    return *this;
}
template <typename T1, typename T2, typename COMPARE, typename E> template <typename U>
Pair<T1, T2, COMPARE, E>& Pair<T1, T2, COMPARE, E>::operator=(otherObj<U>&& other) noexcept {
    mKey   = move(other.key());
    mValue = move(other.value());

    other.key({ });
    other.value({ });

    return *this;
}

template <typename T1, typename T2, typename COMPARE, typename E>
Pair<T1, T2, COMPARE, E>& Pair<T1, T2, COMPARE, E>::operator()(const T1& first, const T2& second) noexcept {
    assign(first, second);

    return *this;
}

template <typename T1, typename T2, typename COMPARE, typename E>
inline Pair<T1, T2, COMPARE, E>::operator bool() const noexcept { return (mKey || mValue) ? true : false; }

/**
 * @brief
 *     서로다른 두 객체의 두 값이 같은지 검사한다.
 *
 * @return true  두 값이 모두 일치한 경우
 * @return false 두 값 중 하나 이상이 일치하지 않은 경우
 */
template <typename T1, typename T2, typename COMPARE, typename E> template <typename U>
bool Pair<T1, T2, COMPARE, E>::operator==(const otherObj<U>& other) const noexcept { return (mKey != other.key() || mValue != other.value()) ? false : true; }
template <typename T1, typename T2, typename COMPARE, typename E> template <typename U>
bool Pair<T1, T2, COMPARE, E>::operator!=(const otherObj<U>& other) const noexcept { return !(*this == other); }

template <typename T1, typename T2, typename COMPARE, typename E> template <typename U>
inline bool Pair<T1, T2, COMPARE, E>::operator<(const otherObj<U>& other) const noexcept {
    if constexpr (isSame<COMPARE, Compare::First>)
        return (mKey < other.key()) ? true : false;
    else
        return (mValue < other.value()) ? true : false;
}
template <typename T1, typename T2, typename COMPARE, typename E> template <typename U>
inline bool Pair<T1, T2, COMPARE, E>::operator>(const otherObj<U>& other) const noexcept {
    if constexpr (isSame<COMPARE, Compare::First>)
        return (mKey > other.key()) ? true : false;
    else
        return (mValue > other.value()) ? true : false;
}
template <typename T1, typename T2, typename COMPARE, typename E> template <typename U>
inline bool Pair<T1, T2, COMPARE, E>::operator<=(const otherObj<U>& other) const noexcept { return !(*this > other); }
template <typename T1, typename T2, typename COMPARE, typename E> template <typename U>
inline bool Pair<T1, T2, COMPARE, E>::operator>=(const otherObj<U>& other) const noexcept { return !(*this < other); }

template <typename T1, typename T2, typename COMPARE, typename E>
void Pair<T1, T2, COMPARE, E>::swap() noexcept {
    if constexpr (isSame<T1, T2>) {
        T1 tmp = move(mKey);

        mKey   = move(mValue);
        mValue = move(tmp);
    }
}
template <typename T1, typename T2, typename COMPARE, typename E>
inline void Pair<T1, T2, COMPARE, E>::assign(const T1& first, const T2& second) noexcept {
    mKey   = first;
    mValue = second;
}

template <typename T1, typename T2, typename COMPARE, typename E>
inline void Pair<T1, T2, COMPARE, E>::key(const T1& key) noexcept { mKey = key; }
template <typename T1, typename T2, typename COMPARE, typename E>
inline void Pair<T1, T2, COMPARE, E>::value(const T2& value) noexcept { mValue = value; }
template <typename T1, typename T2, typename COMPARE, typename E>
inline void Pair<T1, T2, COMPARE, E>::first(const T1& first)  noexcept { key(first); }
template <typename T1, typename T2, typename COMPARE, typename E>
inline void Pair<T1, T2, COMPARE, E>::second(const T2& second) noexcept { value(second); }

template <typename T1, typename T2, typename COMPARE, typename E>
inline T1& Pair<T1, T2, COMPARE, E>::key() noexcept { return mKey; }
template <typename T1, typename T2, typename COMPARE, typename E>
inline T2& Pair<T1, T2, COMPARE, E>::value() noexcept { return mValue; }
template <typename T1, typename T2, typename COMPARE, typename E>
inline const T1& Pair<T1, T2, COMPARE, E>::key() const noexcept { return mKey; }
template <typename T1, typename T2, typename COMPARE, typename E>
inline const T2& Pair<T1, T2, COMPARE, E>::value() const noexcept { return mValue; }

template <typename T1, typename T2, typename COMPARE, typename E>
inline T1& Pair<T1, T2, COMPARE, E>::first() noexcept { return key(); }
template <typename T1, typename T2, typename COMPARE, typename E>
inline T2& Pair<T1, T2, COMPARE, E>::second() noexcept { return value(); }
template <typename T1, typename T2, typename COMPARE, typename E>
inline const T1& Pair<T1, T2, COMPARE, E>::first() const noexcept { return key(); }
template <typename T1, typename T2, typename COMPARE, typename E>
inline const T2& Pair<T1, T2, COMPARE, E>::second() const noexcept { return value(); }