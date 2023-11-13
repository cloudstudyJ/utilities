#pragma once

#include "./typeHandler.h"

/*      ! TODO LISTS
    1. 구현 못한것들 구현하기
    2. toLower, toUpper 리팩토링
*/

class String {
    using uint32 = unsigned int;

    static constexpr uint32 STR_NOT_FOUND = 0x8FFFFFFF;

    public:
        String();
        String(const uint32& size);
        String(const String& other);
        String(String&& other) noexcept;
        String(const char* str);
        ~String() noexcept;

        String& operator=(const String& other);
        String& operator=(String&& other) noexcept;

        String& operator+=(const String& str);
        String& operator+=(String&& str) noexcept;
        String& operator+=(const char& ch);
        String& operator*=(const uint32& multiplier);

        char& operator[](const uint32& idx);
        const char& operator[](const uint32& idx) const;

        bool operator==(const String& other) const;
        bool operator!=(const String& other) const;
        explicit operator bool() const;

        String reverse() const;
        String toLower() const;
        String toUpper() const;
        String slice(const uint32& from, const uint32& to) const;

        // Array<String> split(const char& delimiter = ' ') const;

        void insert(const uint32& idx, const String& str);
        void replace(const uint32& from, const uint32& to, const String& str);
        void resize(const uint32& size);
        void remove(const String& str);
        void remove(const uint32& from, const uint32& to);
        void clear();

        uint32  find(const String& str) const;
        uint32 rfind(const String& str) const;

        inline uint32   length() const;
        inline uint32 capacity() const;
        inline const char* str() const;

        inline bool isEmpty() const;

        static inline uint32 length(const char* str);

    private:
        [[nodiscard]] char*  alloc(const uint32& size) const;
        void reAlloc(const uint32& size) const;

        inline bool isEnoughSpace(const uint32& needSpace) const;
        inline bool isValidIdx(const uint32& idx) const;
        inline bool isValidInterval(const uint32& from, const uint32& to) const;

    private:
        uint32 mLength  { };
        uint32 mCapacity{ };

        char* mStr{ };
};

String::String() { }
/**
 * @brief
 *     미리 size 크기만큼 할당하여 객체를 생성한다.
 *
 * @param size 할당할 문자 배열 크기
 */
String::String(const uint32& size)
    : mCapacity{size}
    , mStr{alloc(size)} { }
String::String(const String& other) { *this = other; }
String::String(String&& other) noexcept { *this = move(other); }
/**
 * @brief
 *     문자열을 복사하여 딱 맞는 크기로 생성한다.
 *
 * @param str 문자열
 */
String::String(const char* str)
    : mLength{String::length(str)}
    , mCapacity{mLength}
    , mStr{alloc(mLength + 1)} {
    // alloc(mLength + 1) 인 이유 -> null terminator 추가를 위해 크기 + 1
    for (uint32 i = 0; i <= mLength; ++i)
        mStr[i] = str[i];
}
String::~String() noexcept { clear(); }

/**
 * @brief
 *     문자열의 끝에 ch 문자를 추가한다.
 *
 * @param ch 추가할 문자
 *
 * @return String& : 호출 객체의 참조
 */
String& String::operator+=(const char& ch) {
    if (!isEnoughSpace(1)) {
        // re-allocation

    }

    mStr[mLength++] = ch;
    mStr[mLength]   = 0;

    return *this;
}

/**
 * @brief
 *     문자열에서 idx 값에 해당하는 인덱스의 문자를 반환한다.
 *     이 때, 경계 검사는 하지 않는다.
 *
 * @param idx 인덱스 값
 *
 * @return char& : idx 값에 해당하는 문자의 참조
 */
      char& String::operator[](const uint32& idx)       { return mStr[idx]; }
const char& String::operator[](const uint32& idx) const { return mStr[idx]; }

/**
 * @brief
 *     다른 객체의 문자열을 복사한다.
 *     복사 생성자, 복사 대입 연산자에서 사용
 *
 * @param other 호출자와 다른 객체
 *
 * @return String& : 호출 객체의 참조
 */
String& String::operator=(const String& other) {
    if (this != &other) {
        clear();

        mLength   = other.mLength;
        mCapacity = other.mCapacity;
        mStr      = alloc(mCapacity);

        for (uint32 i = 0; i <= mLength; ++i)
            mStr[i] = other.mStr[i];
    }

    return *this;
}

/**
 * @brief
 *     다른 객체의 문자열을 호출자로 이동한다.
 *     이동 생성자, 이동 대입 연산자에서 사용 (복사 X)
 *
 * @param other 호출자와 다른 객체
 *
 * @return String& : 호출 객체의 참조
 */
String& String::operator=(String&& other) noexcept {
    if (this != &other) {
        delete[] mStr;
        mStr      = other.mStr;
        mLength   = other.mLength;
        mCapacity = other.mCapacity;

        other.mStr      = { };
        other.mLength   = { };
        other.mCapacity = { };
    }

    return *this;
}

/**
 * @brief
 *     호출 객체와 다른 객체의 문자열이 동일한지 확인한다.
 *
 * @param other 호출자와 다른 객체
 *
 * @return true  : 두 객체의 문자열이 동일한 경우
 * @return false : 두 객체의 문자열이 다른 경우
 */
bool String::operator==(const String& other) const {
    if (mLength != other.mLength)
        return false;

    else {
        if (this != &other) {
            for (uint32 i = 0; i < mLength; ++i)
                if (mStr[i] != other.mStr[i]) return false;
        }

        return true;
    }
}
bool String::operator!=(const String& other) const { return !(*this == other); }
/**
 * @brief
 *     호출 객체의 문자열이 있는지 확인한다.
 *
 * @return true  : 문자열이 있는 경우
 * @return false : 문자열이 없는 경우
 */
String::operator bool() const { return !isEmpty(); }

/**
 * @brief
 *     현재 문자열 중 알파벳들을 소문자로 변경한다.
 *
 * @return String : 변경된 문자열
 */
String String::toLower() const {
    String result = *this;

    for (uint32 i = 0; i < result.mLength; ++i) {
        if ('A' <= result.mStr[i] && result.mStr[i] <= 'Z')
            result.mStr[i] += 0x20;
    }

    return result;
}
/**
 * @brief
 *     현재 문자열 중 알파벳들을 대문자로 변경한다.
 *
 * @return String : 변경된 문자열
 */
String String::toUpper() const {
    String result = *this;

    for (uint32 i = 0; i < mLength; ++i) {
        if ('a' <= result.mStr[i] && result.mStr[i] <= 'z')
            result.mStr[i] -= 0x20;
    }

    return result;
}
/**
 * @brief
 *     [from, to] 구간의 문자열을 자른다.
 *     이 때, from <= to 이어야 한다.
 *
 * @param from 구간 시작 지점 인덱스
 * @param to   구간 끝 지점 인덱스
 *
 * @return String : 잘라낸 문자열
 */
String String::slice(const uint32& from, const uint32& to) const {
    if (isValidInterval(from, to)) {
        String result(from - to + 2);

        for (uint32 i = from; i <= to; ++i)
            result += mStr[i];

        return result;
    }

    return String();
}

void String::clear() {
    delete[] mStr;
    mStr      = { };
    mLength   = { };
    mCapacity = { };
}

inline typename String::uint32 String::length()   const { return mLength;   }
inline typename String::uint32 String::capacity() const { return mCapacity; }
inline const char* String::str() const { return mStr; }

inline bool String::isEmpty() const { return (mLength == 0); }

/**
 * @brief
 *     C-style 문자열의 길이를 구한다. (null terminator 제외)
 *
 * @param str 문자열
 *
 * @return String::uint32 : 문자열의 길이
 */
inline String::uint32 String::length(const char* str) {
    uint32 len{ };

    while (str[len] != 0)
        ++len;

    return len;
}

/**
 * @brief
 *     size 크기만큼 문자 배열 할당
 *
 * @param size 할당할 문자 배열의 크기
 *
 * @return char* : 할당된 문자 배열
 */
[[nodiscard]] char* String::alloc(const uint32& size) const { return new char[size]; }

/**
 * @brief
 *     할당 없이 needSpace 만큼의 공간을 추가로 사용 가능한지를 확인한다.
 *
 * @param needSpace 앞으로 필요한 공간 [Byte]
 *
 * @return true  : 공간 할당이 필요 없는 경우
 * @return false : 공간 할당이 필요한 경우
 */
inline bool String::isEnoughSpace(const uint32& needSpace) const { return (mLength + needSpace < mCapacity); }
/**
 * @brief
 *     어떤 인덱스 값이 문자열 내에 있는지 확인한다.
 *
 * @param idx 인덱스 값
 *
 * @return true  : 값이 0이상이고 문자열 길이 미만인 경우
 * @return false : 값이 문자열 길이 이상인 경우
 */
inline bool String::isValidIdx(const uint32& idx) const { return (idx < mLength); }
/**
 * @brief
 *     어떤 두 인덱스 값으로 이루어진 구간이 문자열 내에 있는지 확인한다.
 *
 * @param from 구간 시작 인덱스 값
 * @param to   구간 끝 인덱스 값
 *
 * @return true  : 두 인덱스 값이 유효하고, from <= to 인 경우
 * @return false : 그 외의 경우
 */
inline bool String::isValidInterval(const uint32& from, const uint32& to) const { return ((isValidIdx(from) && isValidIdx(to)) && (from <= to)); }