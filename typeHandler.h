#pragma once

namespace typeHandlingBase {
    template <typename T> struct removeReference      { using type = T; };
    template <typename T> struct removeReference<T&>  { using type = T; };
    template <typename T> struct removeReference<T&&> { using type = T; };

    template <bool, typename = void> struct enableIF          { };
	template <typename T>            struct enableIF<true, T> { using type = T; };
}

template <typename T> using removeReference = typename typeHandlingBase::removeReference<T>::type;

template <bool Condition, typename T = void>
using enableIF = typename typeHandlingBase::enableIF<Condition, T>::type;

template <typename, typename> inline constexpr bool isSame       = false;
template <typename T>         inline constexpr bool isSame<T, T> = true;

template <typename T>
[[nodiscard]] constexpr decltype(auto) move(T&& _val) noexcept { return static_cast<removeReference<T>&&>(_val); }