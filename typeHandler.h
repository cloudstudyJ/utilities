#pragma once

namespace typeHandlingBase {
    template <typename T> struct removeReference      { using type = T; };
    template <typename T> struct removeReference<T&>  { using type = T; };
    template <typename T> struct removeReference<T&&> { using type = T; };
}

template <typename T> using removeReference = typename typeHandlingBase::removeReference<T>::type;

template <typename T>
[[nodiscard]] constexpr decltype(auto) move(T&& _val) noexcept { return static_cast<removeReference<T>&&>(_val); }