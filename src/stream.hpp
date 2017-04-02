#ifndef STREAM_SRC_STREAM_HPP_
#define STREAM_SRC_STREAM_HPP_

#include <cstddef>

#include <initializer_list>
#include <iterator>
#include <ostream>
#include <queue>
#include <vector>

namespace stream {

template<typename T>
class Stream;

template<typename T>
auto MakeStream(std::initializer_list<T> init);

template<typename Iterator>
auto MakeStream(Iterator begin, Iterator end);

template<typename T>
class Stream {
public:
    // Constructor ??
    // Move, copy ??
    // Destructor ??

#if 0
    template<typename Generator>
    Stream<T> MakeStream(Generator &&generator);

    Stream<T> MakeStream(value1, value2, ...);

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

    std::ostream &print_to(std::ostream &os,
            const char *delimiter = ' ');

    std::vector<T> to_vector();

    T &nth(const size_t index);
#endif

private:
    std::vector<T> values;
    //std::queue<function<???>> actions_queue;
    //
private:
    friend auto MakeStream<T>(std::initializer_list<T> init);

    template<typename Iterator>
    friend auto MakeStream(Iterator begin, Iterator end);
};

template<typename Iterator>
auto MakeStream(Iterator begin, Iterator end) {
    Stream<typename std::iterator_traits<Iterator>::value_type> s;
    s.values.reserve(std::distance(begin, end));
    s.values.insert(s.values.end(), begin, end);
    return s;
}

template<typename T>
auto MakeStream(std::initializer_list<T> init) {
    Stream<T> s;
    s.values.reserve(init.size());
    s.values.insert(s.values.end(), init.begin(), init.end());
    return s;
}

/*
template<typename Container>
auto MakeStream(const Container &cont) {
    return nullptr;
}

template<typename Container>
auto MakeStream(Container &&cont) {
    return nullptr;
}
*/

} // namespace stream

#endif // STREAM_SRC_STREAM_HPP_
