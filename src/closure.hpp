#ifndef STREAM_SRC_CLOSURE_HPP_
#define STREAM_SRC_CLOSURE_HPP_

#include <type_traits>

namespace stream {

template<typename T>
struct closure_wrapper {
    constexpr explicit closure_wrapper(T&& v): _val(std::move(v)) {}
    constexpr T&& get() const{ return std::move(_val); }
private:
    static_assert(not std::is_rvalue_reference<T>::value, "Closure error");
    std::remove_const_t<T> mutable _val;
};

template<typename T>
struct closure_wrapper<T&> {
    constexpr explicit closure_wrapper(T &r): _ref(r) {}
    constexpr T& get() const{ return _ref; }
private:
    T &_ref;
};

template<typename T,
    typename S = typename std::enable_if<
        std::is_lvalue_reference<T>::value, closure_wrapper<T>>::type,
    typename NonRefS = typename std::remove_reference_t<T>::type>
constexpr S closure(NonRefS &t) {
    return closure_wrapper<T>(t);
}

template<typename T,
    typename S = typename std::enable_if<std::is_rvalue_reference<T&&>::value,
            closure_wrapper<std::remove_reference_t<T>>>::type>
constexpr S closure(std::remove_reference_t<T>&& t) {
    return closure_wrapper<std::remove_reference_t<T>>(std::move(t));
}

template<typename T,
    typename S = typename std::enable_if<std::is_rvalue_reference<T&&>::value,
            closure_wrapper<std::remove_reference_t<T>>>::type>
constexpr S closure(std::remove_reference_t<T>& t) {
    return closure_wrapper<std::remove_reference_t<T>>(std::move(t));
}

}  // namespace stream

#endif  // STREAM_SRC_CLOSURE_HPP_
