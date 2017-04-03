#ifndef STREAM_SRC_STREAM_HPP_
#define STREAM_SRC_STREAM_HPP_

#include <cstddef>

#include <functional>
#include <initializer_list>
#include <iterator>
#include <ostream>
#include <queue>
#include <type_traits>
#include <vector>

// TODO: remove me!!!
#include <iostream>

namespace stream {

template<typename T>
class Stream;

template<typename T>
auto MakeStream(std::initializer_list<T> init);

template<typename Iterator>
auto MakeStream(Iterator begin, Iterator end);

template<typename Container>
auto MakeStream(const Container &cont);

template<typename Container>
auto MakeStream(Container &&cont);

template<typename Generator>
auto MakeStream(Generator &&generator);

template<typename T>
class Stream {
public:
    // TODO: // Constructor ??  // Move, copy ??  // Destructor ??

    // Returns new Stream anyway (even in case of empty)
    template<typename Transform, typename U =
        decltype(std::declval<Transform>()(std::declval<T>()))>
    Stream<U> map(Transform &&transform) {
        Stream<U> s;
        auto &v = *reinterpret_cast<std::vector<T>*>(values);
        auto &sv = *reinterpret_cast<std::vector<U>*>(s.values);
        sv.reserve(v.size());
        for (auto it = v.begin(); it != v.end(); ++it) {
            sv.push_back(transform(*it));
        }
        return s;
    }

    // TODO: figure out what to do in case of empty
    template<typename IdentityFn, typename Accumulator, typename U =
        decltype(std::declval<IdentityFn>()(std::declval<T>()))>
    U reduce(IdentityFn &&identityFn, Accumulator &&accum) {
        auto &v = *reinterpret_cast<std::vector<T>*>(values);
        auto begin = v.begin();
        if (begin == v.end()) throw 1;
        U total = identityFn(*(begin++));
        for (auto it = begin; it != v.end(); ++it) {
            total = accum(total, *it);
        }
        return total;
    }

    // TODO: extend it to support different than T return type
    // TODO: figure out what to do in case of empty
    template<typename Accumulator>
    T reduce(Accumulator &&accum) {
        auto &v = *reinterpret_cast<std::vector<T>*>(values);
        auto begin = v.begin();
        if (begin == v.end()) throw 1;
        // Copy to keep first element of Stream clean
        T total = *(begin++);
        for (auto it = begin; it != v.end(); ++it) {
            total = accum(total, *it);
        }
        return total;
    }
#if 0
    // TODO: figure out how to deduce Uundeduced
    // Returns new Stream anyway (even in case of empty)
    template<typename Accumulator, typename Uundeduced, typename U =
        decltype(std::declval<Accumulator>()(
                    std::declval<Uundeduced>(), std::declval<T>()))>
    Stream<U> reduce(Accumulator &&accum) {
        U total;
        auto begin = values.begin();
        if (begin == values.end()) return total;
        total = accum(total, *(begin++));
        for (auto it = begin; it != values.end(); ++it) {
            total = accum(total, *it);
        }
        return total;
    }
#endif

    template<typename Predicate>
    Stream<T> filter(Predicate &&predicate) {
        Stream<T> s;
        auto &v = *reinterpret_cast<std::vector<T>*>(values);
        auto &sv = *reinterpret_cast<std::vector<T>*>(s.values);
        for (auto it = v.begin(); it != v.end(); ++it) {
            auto &v = *it;
            if (predicate(v)) sv.push_back(v);
        }
        return s;
    }

#if 0
    template<typename Func,
        typename RetType =
        decltype(std::declval<Func>()(std::declval<Stream<T>>())),
        typename U = typename std::enable_if<RetType::is_stream, RetType>::type>
    U operator|(const Func &f) {
        return this->f();
    }
#endif

    Stream<T> skip(const size_t amount) {
        // TODO: some checks for amount value
        auto &v = *reinterpret_cast<std::vector<T>*>(values);
        v.erase(v.begin(), v.begin() + amount);
        return *this;
    }

    // TODO: figure out what to do in case of N=0
    Stream<Stream<T>> group(const size_t N) {
        if (N == 0) throw 1;
        Stream<Stream<T>> s;
        auto &v = *reinterpret_cast<std::vector<T>*>(values);
        auto &sv = *reinterpret_cast<std::vector<Stream<T>>*>(s.values);
        sv.reserve(div_up(v.size(), N));
        for (auto it = v.begin();;) {
            Stream<T> inside_stream = MakeStream(it, it+N);
            sv.push_back(inside_stream);
            it += N;
            if (it >= v.end()) break;
        }
        return s;
    }

    // Hope that we have operator+= for type T
    // Hope that we have copy constructor for T
    // TODO: figure out what to do with empty stream
    T sum() {
        auto &v = *reinterpret_cast<std::vector<T>*>(values);
        auto begin = v.begin();
        if (begin == v.end()) throw 1;
        T total = *(begin++);
        for (auto it = begin; it != v.end(); ++it) {
            total += *it;
        }
        return total;
    }

    std::ostream &print_to(std::ostream &os, const char *delimiter = " ") {
        // TODO: here should be work with queue.
        auto &v = *reinterpret_cast<std::vector<T>*>(values);
        auto it = v.begin();
        if (it == v.end()) return os;
        os << *(it++);
        for (; it != v.end(); ++it) {
            os << delimiter << *it;
        }
        return os;
    }

    std::vector<T> to_vector() {
        // TODO: here should be work with queue.
        return *reinterpret_cast<std::vector<T>*>(values);
    }

    T nth(const size_t index) {
        // TODO: here should be work with queue.
        return (*reinterpret_cast<std::vector<T>*>(values))[index];
    }

private:
    enum { is_stream = 1 };
    void *values;
    std::queue<std::function<void()>> functions;

private:
    friend auto MakeStream<T>(std::initializer_list<T> init);

    template<typename Iterator>
    friend auto MakeStream(Iterator begin, Iterator end);

    template<typename Container>
    friend auto MakeStream(const Container &cont);

    template<typename Container>
    friend auto MakeStream(Container &&cont);

    template<typename Generator>
    friend auto MakeStream(Generator &&generator);

    template<typename U>
    friend class Stream;

private:
    Stream(): values(reinterpret_cast<std::vector<T>*>(new std::vector<T>)) {}

    template<typename Iterator, typename U =
        typename std::iterator_traits<Iterator>::value_type>
    Stream(Iterator begin, Iterator end) {
        auto v = new std::vector<U>();
        v->reserve(std::distance(begin, end));
        v->insert(v->end(), begin, end);
        values = reinterpret_cast<void*>(v);
    }

    template<typename U>
    static U div_up(const U &a, const U &b) {
        return (a + b - 1) / b;
    }
};

template<typename Iterator>
auto MakeStream(Iterator begin, Iterator end) {
    using T = typename std::iterator_traits<Iterator>::value_type;
    return Stream<T>(begin, end);
}

template<typename T>
auto MakeStream(std::initializer_list<T> init) {
    return Stream<T>(init.begin(), init.end());
}

template<typename Container>
auto MakeStream(const Container &cont) {
    return Stream<typename Container::value_type>(cont.begin(), cont.end());
}

template<typename Container>
auto MakeStream(Container &&cont) {
    return Stream<typename std::remove_reference<Container>::type::value_type>(
            // TODO: really move iterators???
            std::move(cont.begin()), std::move(cont.end()));
}

#if 0
// Actually checks if class contains const interator
template<typename T>
class is_container {
    typedef float s;
    typedef double d;

    template<typename C> static s test(typename C::const_iterator*);
    template<typename C> static d test(...);

public:
    enum { value = sizeof(test<T>(0)) == sizeof(s) };
};

template<typename Container, typename T =
std::enable_if<is_container<Container>::value, Container>>
auto MakeStream(T &&cont) {
    return Stream<typename std::remove_reference<T>::type::value_type>(
            // TODO: really move iterators???
            std::move(cont.begin()), std::move(cont.end()));
}

template<typename T>
class is_generator {
    typedef float s;
    typedef double d;

    template<typename C> static s test(decltype(&C::operator()));
    template<typename C> static d test(...);

public:
    enum { value = sizeof(test<T>(0)) == sizeof(s) };
};

template<typename Generator>
auto MakeStream(Generator &&generator) {
    std::cout << "GENERATOR" << std::endl;
    auto &&generated = generator();
    Stream<typename std::remove_reference<decltype(generated)>::type> s;
    s.values.push_back(std::move(s));
}
#endif

/*
auto MakeStream(value1, value2, ...);
*/

} // namespace stream

#endif // STREAM_SRC_STREAM_HPP_
