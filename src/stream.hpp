#ifndef STREAM_SRC_STREAM_HPP_
#define STREAM_SRC_STREAM_HPP_

#include <iostream>
#include <vector>

namespace stream {

template<typename T>
class Stream {
public:
    // Constructor ??
    // Move, copy ??
    // Destructor ??

    template<typename Iterator>
    static Stream<T> MakeStream(Iterator begin, Iterator end);

    template<typename Container>
    static Stream<T> MakeStream(const Container &cont);

    template<typename Container>
    static Stream<T> MakeStream(Container &&cont);

    static Stream<T> MakeStream(std::initializer_list<T> init);

#if 0
    template<typename Generator>
    Stream<T> MakeStream(Generator &&generator);

    Stream<T> MakeStream(value1, value2, ...);
#endif

    template<typename Transform>
    Stream<T> map(Transform &&transform);

    template<typename Accumulator>
    Stream<T> reduce(Accumulator &&accum);

    template<typename IdentityFn, typename Accumulator>
    Stream<T> reduce(IdentityFn &&identityFn, Accumulator &&accum);

    template<typename Predicate>
    Stream<T> filter(Predicate &&predicate);

    Stream<T> operator|(const Stream<T> &another);

    Stream<T> skip(const size_t amount);

    Stream<T> group(const size_t N);

    Stream<T> sum();

    std::ostream &print_to(std::ostream &os,
            const char *delimiter = ' ');

    std::vector<T> to_vector();

    T &nth(const size_t index);
};

} // namespace stream

#endif // STREAM_SRC_STREAM_HPP_
