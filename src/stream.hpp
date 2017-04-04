#ifndef STREAM_SRC_STREAM_HPP_
#define STREAM_SRC_STREAM_HPP_

#include <cstddef>

#include <functional>
#include <initializer_list>
#include <iterator>
#include <ostream>
#include <queue>
#include <stdexcept>
#include <type_traits>
#include <vector>

// Logging
#ifdef STREAM_VERBOSE
#include <iostream>
#define LOG(str) do { \
    std::cout << (str) << std::endl; \
} while (false)
#else
#define LOG(str)
#endif

namespace stream {

template<typename T>
class Stream;

template<typename Iterator>
auto MakeStream(Iterator begin, Iterator end) {
    LOG("MakeStream: Iterators");
    using T = typename std::iterator_traits<Iterator>::value_type;
    std::vector<T> *vec = new std::vector<T>;
    for (auto it = begin; it != end; ++it) {
        vec->push_back(*it);
    }
    return Stream<T>(vec);
}

template<typename T>
auto MakeStream(std::initializer_list<T> init) {
    LOG("MakeStream: Initializer list");
    std::vector<T> *vec = new std::vector<T>;
    for (auto it = init.begin(); it != init.end(); ++it) {
        vec->push_back(*it);
    }
    return Stream<T>(vec);
}

template<typename Container>
auto MakeStream(const Container &cont) {
    LOG("MakeStream: const Containter &cont");
    typedef typename Container::value_type T;
    std::vector<T> *vec = new std::vector<T>;
    for (auto it = cont.begin(); it != cont.end(); ++it) {
        vec->push_back(*it);
    }
    return Stream<T>(vec);
}

template<typename Container>
auto MakeStream(Container &&cont) {
    LOG("MakeStream: Container &&cont");
    typedef typename std::remove_reference<Container>::type::value_type T;
    std::vector<T> *vec = new std::vector<T>;
    for (auto it = cont.begin(); it != cont.end(); ++it) {
        vec->push_back(*it);
    }
    return Stream<T>(vec);
    /*
    return Stream<typename std::remove_reference<Container>::type::value_type>(
            std::move(cont.begin()), std::move(cont.end()));
            */
}

template<typename T>
class Stream {
public:
    Stream(void *v): values(v) {
        LOG("Stream: constructor using pointer to values");
    }

#if 0
    template<typename Iterator, typename U =
        typename std::iterator_traits<Iterator>::value_type>
    Stream(Iterator begin, Iterator end) {
        auto v = new std::vector<U>();
        v->reserve(std::distance(begin, end));
        v->insert(v->end(), begin, end);
        values = reinterpret_cast<void*>(v);
    }
#endif

    ~Stream() {
        LOG("Stream: destructor");
        if (values) delete reinterpret_cast<std::vector<T>*>(values);
    }

    template<typename U>
    Stream(const Stream<U> &s) {
        LOG("Stream: copy constructor");
        auto svp = new std::vector<U>;
        auto vp = reinterpret_cast<std::vector<U>*>(s.values);
        svp->insert(svp->end(), vp->begin(), vp->end());
        values = svp;
        functions = s.functions;
    }

    template<typename U>
    Stream(Stream<U> &&s) {
        LOG("Stream: move constructor");
        values = s.values;
        s.values = nullptr;
        functions = std::move(s.functions);
    }

    Stream<T> &operator=(const Stream<T> &s) {
        LOG("Stream: assignment");
        auto svp = new std::vector<T>;
        auto vp = reinterpret_cast<std::vector<T>*>(s.values);
        svp->insert(svp->end(), vp->begin(), vp->end());
        values = svp;
        functions = s.functions;
        return *this;
    }

    template<typename U>
    Stream<T> &operator=(Stream<U> &&s) {
        LOG("Stream: move assignment");
        values = s.values;
        s.values = nullptr;
        functions = std::move(s.functions);
        return *this;
    }
    // TODO: // Constructor ??  // Move, copy ??  // Destructor ??
    // TODO: thread safe?
    // TODO: const methods?
    // TODO: return value or reference?
    // TODO: perfect forwarding?

    template<typename Transform, typename U =
        decltype(std::declval<Transform>()(std::declval<T>()))>
    Stream<U> &map(Transform &&transform) {
        LOG("Stream: map");
        auto deferred = [&] (void *current_stream) {
            LOG("Stream: deferred map execution");
            Stream<T> *cs = reinterpret_cast<Stream<T>*>(current_stream);
            auto vp = reinterpret_cast<std::vector<T>*>(cs->values);
            auto svp = new std::vector<U>;
            svp->reserve(vp->size());
            for (auto it = vp->begin(); it != vp->end(); ++it) {
                svp->push_back(transform(*it));
            }
            delete vp;
            cs->values = svp;
        };
        functions.push(deferred);
        return *reinterpret_cast<Stream<U>*>(this);
    }

    // TODO: figure out what to do in case of empty
    template<typename IdentityFn, typename Accumulator, typename U =
        decltype(std::declval<IdentityFn>()(std::declval<T>()))>
    U reduce(IdentityFn &&identityFn, Accumulator &&accum) {
        LOG("Stream: reduce with identity execution");
        execute();
        auto &v = *reinterpret_cast<std::vector<T>*>(values);
        auto begin = v.begin();
        if (begin == v.end()) throw std::runtime_error(
                "Empty stream in reduce function with identity");
        U total = identityFn(*(begin++));
        for (auto it = begin; it != v.end(); ++it) {
            total = accum(total, *it);
        }
        return total;
    }

    // TODO: extend it to support different than T return type
    // TODO: figure out what to do in case of empty
    // Hope that we have copy constructor for type T
    template<typename Accumulator>
    T reduce(Accumulator &&accum) {
        LOG("Stream: reduce execution");
        execute();
        auto &v = *reinterpret_cast<std::vector<T>*>(values);
        auto begin = v.begin();
        if (begin == v.end()) throw std::runtime_error(
                "Empty stream in reduce function");
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
    Stream<T> &filter(Predicate &&predicate) {
        LOG("Stream: filter");
        auto deferred = [&] (void *current_stream) {
            LOG("Stream: deferred filter execution");
            Stream<T> *cs = reinterpret_cast<Stream<T>*>(current_stream);
            auto vp = reinterpret_cast<std::vector<T>*>(cs->values);
            auto svp = new std::vector<T>;
            for (auto it = vp->begin(); it != vp->end(); ++it) {
                auto &v = *it;
                if (predicate(v)) svp->push_back(v);
            }
            delete vp;
            cs->values = svp;
        };
        functions.push(deferred);
        return *this;
    }

// TODO: implement me
#if 0
    template<typename Func,
        typename RetType =
        decltype(std::declval<Func>()(std::declval<Stream<T>>())),
        typename U = typename std::enable_if<RetType::is_stream, RetType>::type>
    U operator|(const Func &f) {
        return this->f();
    }
#endif

    // TODO: some checks for amount value
    Stream<T> &skip(const size_t amount) {
        LOG("Stream: skip");
        auto deferred = [=] (void *current_stream) {
            LOG("Stream: deferred skip execution");
            Stream<T> *cs = reinterpret_cast<Stream<T>*>(current_stream);
            auto vp = reinterpret_cast<std::vector<T>*>(cs->values);
            vp->erase(vp->begin(), vp->begin() + amount);
        };
        functions.push(deferred);
        return *this;
    }

    // TODO: figure out what to do in case of N=0
    Stream<Stream<T>> &group(const size_t N) {
        LOG("Stream: group");
        if (N == 0) throw std::runtime_error("N is 0 in group function");
        auto deferred = [=] (void *current_stream) {
            LOG("Stream: deferred group execution");
            Stream<T> *cs = reinterpret_cast<Stream<T>*>(current_stream);
            auto vp = reinterpret_cast<std::vector<T>*>(cs->values);
            auto svp = new std::vector<Stream<T>>;
            // TODO: we dont want to reserve a lot of memory
            // It works not ok in case of input stream size = 1 and N = 100000
            svp->reserve(div_up(vp->size(), N));
            for (auto it = vp->begin();;) {
                auto ivp = new std::vector<T>;
                ivp->insert(ivp->end(), it, it+N);
                svp->push_back(std::move(Stream<T>(ivp)));
                it += N;
                if (it >= vp->end()) break;
            }
            delete vp;
            cs->values = svp;
        };
        functions.push(deferred);
        return *reinterpret_cast<Stream<Stream<T>>*>(this);
    }

    // Hope that we have operator+= for type T
    // Hope that we have copy constructor for T
    // TODO: figure out what to do with empty stream
    T sum() {
        LOG("Stream: sum");
        execute();
        LOG("Stream: actual sum execution");
        auto vp = reinterpret_cast<std::vector<T>*>(values);
        auto begin = vp->begin();
        if (begin == vp->end())
            throw std::runtime_error("Stream is empty in sum function");
        T total = *(begin++);
        for (auto it = begin; it != vp->end(); ++it) {
            total += *it;
        }
        return total;
    }

    std::ostream &print_to(std::ostream &os, const char *delimiter = " ") {
        LOG("Stream: print_to");
        execute();
        LOG("Stream: actual print_to execution");
        auto vp = reinterpret_cast<std::vector<T>*>(values);
        auto it = vp->begin();
        if (it == vp->end()) return os;
        os << *(it++);
        for (; it != vp->end(); ++it) {
            os << delimiter << *it;
        }
        return os;
    }

    std::vector<T> to_vector() {
        LOG("Stream: to_vector");
        execute();
        LOG("Stream: actual to_vector execution");
        return *reinterpret_cast<std::vector<T>*>(values);
    }

    T nth(const size_t index) {
        LOG("Stream: nth");
        execute();
        LOG("Stream: actual nth execution");
        return (*reinterpret_cast<std::vector<T>*>(values))[index];
    }

private:
    void *values = nullptr;
    std::queue<std::function<void(void*)>> functions;

private:
    void execute() {
        while (!functions.empty()) {
            functions.front()(this);
            functions.pop();
        }
    }

    template<typename U>
    static U div_up(const U &a, const U &b) {
        return (a + b - 1) / b;
    }

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
};

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
