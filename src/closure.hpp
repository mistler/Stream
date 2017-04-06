#ifndef STREAM_SRC_CLOSURE_HPP_
#define STREAM_SRC_CLOSURE_HPP_

#include <type_traits>

namespace stream {

template<typename T>
struct closure_wrapper {
    constexpr explicit closure_wrapper(T&& v) noexcept(
        std::is_nothrow_move_constructible<std::remove_const_t<T>>{})
        : val_(std::move(v)) {}
    constexpr T&& get() const noexcept { return std::move(val_); }

private:
    static_assert(not std::is_rvalue_reference<T>{}, "");
    std::remove_const_t<T> mutable val_;
};

template<typename T>
struct closure_wrapper<T&> {
    constexpr explicit closure_wrapper(T& r) noexcept : ref_(r) {}
    constexpr T& get() const noexcept { return ref_; }
private:
    T& ref_;
};

template<typename T>
constexpr typename std::enable_if<
    std::is_lvalue_reference<T>{}, closure_wrapper<T>>::type
closure(std::remove_reference_t<T>& t) noexcept {
    return closure_wrapper<T>(t);
}

template <typename T>
constexpr typename std::enable_if<std::is_rvalue_reference<T&&>{},
            closure_wrapper<std::remove_reference_t<T>>>::type
    closure(std::remove_reference_t<T>&& t) noexcept(
        std::is_nothrow_constructible<
            closure_wrapper<std::remove_reference_t<T>>, T&&>{}) {
    return closure_wrapper<std::remove_reference_t<T>>(std::move(t));
}

template <typename T>
constexpr typename std::enable_if<std::is_rvalue_reference<T&&>{},
            closure_wrapper<std::remove_reference_t<T>>>::type
    closure(std::remove_reference_t<T>& t) noexcept(
        std::is_nothrow_constructible<
            closure_wrapper<std::remove_reference_t<T>>, T&&>{}) {
    return closure_wrapper<std::remove_reference_t<T>>(std::move(t));
}

}  // namespace stream

#endif  // STREAM_SRC_CLOSURE_HPP_
