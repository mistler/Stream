#ifndef STREAM_SRC_STREAM_HPP_
#define STREAM_SRC_STREAM_HPP_

#include <cstddef>

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
    // Constructor ??
    // Move, copy ??
    // Destructor ??

#if 0

    template<typename Transform>
    Stream<T> map(Transform &&transform);

    template<typename Accumulator>
    Stream<T> reduce(Accumulator &&accum);

    template<typename IdentityFn, typename Accumulator>
    Stream<T> reduce(IdentityFn &&identityFn, Accumulator &&accum);

    template<typename Predicate>
    Stream<T> filter(Predicate &&predicate);

    template<typename Func>
    Stream<T> operator|(const Func &f);

    Stream<T> skip(const size_t amount);

    Stream<T> group(const size_t N);

    Stream<T> sum();
#endif

    std::ostream &print_to(std::ostream &os, const char *delimiter = " ") {
        // TODO: here should be work with queue.
        auto it = values.begin();
        if (it == values.end()) return os;
        os << *(it++);
        for (; it != values.end(); ++it) {
            os << delimiter << *it;
        }
        return os;
    }

    std::vector<T> to_vector() {
        // TODO: here should be work with queue.
        return values;
    }

    T &nth(const size_t index) {
        // TODO: here should be work with queue.
        return values[index];
    }

private:
    std::vector<T> values;
    //std::queue<function<???>> actions_queue;
    //
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

private:
    Stream() {}

    template<typename Iterator>
    Stream(Iterator begin, Iterator end) {
        values.reserve(std::distance(begin, end));
        values.insert(values.end(), begin, end);
    }
};

template<typename Iterator>
auto MakeStream(Iterator begin, Iterator end) {
    using t = typename std::iterator_traits<Iterator>::value_type;
    return Stream<t>(begin, end);
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
