#ifndef STREAM_SRC_TYPE_HELPERS_HPP
#define STREAM_SRC_TYPE_HELPERS_HPP

namespace stream {

// Duck-typing (has iterator -> then is container)
template<typename T>
class is_container {
    typedef float s;
    typedef double d;

    template<typename C> static s test(typename C::const_iterator*);
    template<typename C> static d test(...);

public:
    enum { value = sizeof(test<T>(0)) == sizeof(s) };
};

} // namespace stream

#endif // STREAM_SRC_TYPE_HELPERS_HPP
