#pragma once

class String {
    using uint32 = unsigned int;

    static constexpr uint32 STR_NOT_FOUND = 0x8FFFFFFF;

    public:
        String();
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

        char& operator[](const uint32& idx) const;

        bool operator==(const String& other) const;
        bool operator!=(const String& other) const;
        explicit operator bool() const;

        String reverse() const;
        String toLower() const;
        String toUpper() const;
        String slice(const uint32& from, const uint32& to) const;

        // Array<String> split(const char& delimiter) const;

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

        inline bool isEmpty() const;

    private:
        char*  alloc(const uint32& size) const;
        void reAlloc(const uint32& size) const;

    private:
        uint32 mLength{ };
        uint32 mCapacity{ };

        char* mStr{ };
};

String::String() { }
String::~String() noexcept { clear(); }

explicit String::operator bool() const { return !isEmpty(); }

void String::clear() {
    mLength   = { };
    mCapacity = { };

    delete[] mStr;
    mStr = { };
}

inline typename String::uint32 String::length()   const { return mLength;   }
inline typename String::uint32 String::capacity() const { return mCapacity; }

inline bool String::isEmpty() const { return (mLength == 0 ? true : false); }