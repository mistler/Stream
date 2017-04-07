#ifndef STREAM_SRC_STREAM_HPP_
#define STREAM_SRC_STREAM_HPP_

#include <cstddef>

#include <functional>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <ostream>
#include <queue>
#include <stdexcept>
#include <type_traits>
#include <vector>

#include "stream_operator.hpp"
#include "logger.hpp"
#include "type_helpers.hpp"
#include "closure.hpp"

namespace stream {

template<typename T>
class Stream;

struct DefaultStreamType {};

template<typename Iterator,
    typename T = typename std::iterator_traits<Iterator>::value_type>
auto MakeStream(Iterator begin, Iterator end) {
    LOG("MakeStream: Iterators");
    std::vector<T> *vec = new std::vector<T>;
    for (auto it = begin; it != end; ++it) {
        vec->push_back(*it);
    }
    return Stream<T>(vec);
}

template<typename T>
auto MakeStream(std::initializer_list<T> init) {
    LOG("MakeStream: Initializer list");
    std::vector<T> *vec = new std::vector<T>(std::move(init));
    return Stream<T>(vec);
}

template<typename Container,
    typename T = typename std::remove_reference<Container>::type::value_type,
    typename U = typename std::enable_if<
        is_container<Container>::value, Container>::type>
auto MakeStream(const Container &cont) {
    LOG("MakeStream: const Containter &cont");
    std::vector<T> *vec = new std::vector<T>;
    for (auto it = cont.begin(); it != cont.end(); ++it) {
        vec->push_back(*it);
    }
    return Stream<T>(vec);
}

template<typename Container,
    typename T = typename std::remove_reference<Container>::type::value_type,
    typename U = typename std::enable_if<
        is_container<Container>::value, Container>::type>
auto MakeStream(Container &&cont) {
    LOG("MakeStream: Container &&cont");
    std::vector<T> *vec = new std::vector<T>;
    for (auto it = cont.begin(); it != cont.end(); ++it) {
        vec->push_back(*it);
    }
    return Stream<T>(vec);
}

template<typename Generator,
    typename T = decltype(std::declval<Generator>()())>
auto MakeStream(Generator &&generator) {
    LOG("MakeStream: Generator &&genereator");
    std::vector<T> *vec = new std::vector<T>{ std::move(generator()) };
    return Stream<T>(vec);
}

namespace {

template<typename S>
auto MakeStreamVariadic(S &&s) {
    return std::forward<S>(s);
}

template<typename S, typename Arg, typename ...Args,
    typename TypeCheck = typename std::enable_if<
        std::is_same<typename S::value_type, Arg>::value, Arg>::type>
auto MakeStreamVariadic(S &&s, Arg &&arg, Args &&...args) {
    auto vp = reinterpret_cast<std::vector<Arg>*>(s.getVector());
    vp->push_back(std::forward<Arg>(arg));
    return MakeStreamVariadic(std::forward<S>(s),
                std::forward<Args>(args)...);
}

} // namespace

template<typename Arg, typename ...Args,
    typename U = typename std::enable_if<is_container<
        typename std::remove_reference<Arg>::type>::value == 0, Arg>::type>
auto MakeStream(Arg &&arg, Args &&...args) {
    LOG("MakeStream: variadic create");
    std::vector<Arg> *vec = new std::vector<Arg>;
    return MakeStreamVariadic<Stream<Arg>, Args...>(std::move(Stream<Arg>(vec)),
            std::forward<Arg>(arg), std::forward<Args>(args)...);
}

// Special case for variadic MakeStream without arguments
Stream<DefaultStreamType> MakeStream();

template<typename T>
class Stream {
public:
    typedef typename std::remove_reference<T>::type value_type;

    Stream(std::vector<T> *v): values(v) {}
    ~Stream() { LOG("Stream: destructor"); }

    Stream(const Stream<T> &s) {
        LOG("Stream: copy constructor");
        values = s.values;
        functions = s.functions;
    }

    Stream(Stream<T> &&s) {
        LOG("Stream: move constructor");
        values = s.values;
        functions = std::move(s.functions);
    }

    Stream<T> &operator=(const Stream<T> &s) {
        LOG("Stream: assignment");
        values = s.values;
        functions = s.functions;
        return *this;
    }

    Stream<T> &operator=(Stream<T> &&s) {
        LOG("Stream: move assignment");
        values = s.values;
        functions = std::move(s.functions);
        return *this;
    }

    void *getVector() {
        return values.get();
    }

    template<typename Transform, typename U =
        decltype(std::declval<Transform>()(std::declval<T>()))>
    Stream<U> map(Transform &&transform) {
        LOG("Stream: map");
        auto deferred = [transform = closure<Transform>(transform)] (
                void *current_stream) {
            LOG("Stream: map execution");
            Stream<T> *cs = reinterpret_cast<Stream<T>*>(current_stream);
            auto vp = reinterpret_cast<std::vector<T>*>(cs->values.get());
            auto svp = new std::vector<U>;
            svp->reserve(vp->size());
            for (auto it = vp->begin(); it != vp->end(); ++it) {
                svp->push_back(transform.get()(std::forward<T>(*it)));
            }
            cs->values.reset(svp);
        };
        auto s = Stream<U>(*this);
        s.functions.push(deferred);
        return s;
    }

    template<typename IdentityFn, typename Accumulator, typename U =
        decltype(std::declval<IdentityFn>()(std::declval<T>()))>
    U reduce(IdentityFn &&identityFn, Accumulator &&accum) {
        LOG("Stream: reduce with identity");
        execute();
        LOG("Stream: reduce with identity execution");
        auto &v = *reinterpret_cast<std::vector<T>*>(values.get());
        auto begin = v.begin();
        if (begin == v.end()) throw std::runtime_error(
                "Empty stream in reduce function with identity");
        U total = identityFn(*(begin++));
        for (auto it = begin; it != v.end(); ++it) {
            total = accum(total, std::forward<T>(*it));
        }
        return total;
    }

    // Hope that we have copy constructor for type T
    template<typename Accumulator>
    T reduce(Accumulator &&accum) {
        LOG("Stream: reduce");
        execute();
        LOG("Stream: reduce execution");
        auto &v = *reinterpret_cast<std::vector<T>*>(values.get());
        auto begin = v.begin();
        if (begin == v.end()) throw std::runtime_error(
                "Empty stream in reduce function");
        T total = *(begin++);
        for (auto it = begin; it != v.end(); ++it) {
            total = accum(total, std::forward<T>(*it));
        }
        return total;
    }

    template<typename Predicate>
    Stream<T> filter(Predicate &&predicate) {
        LOG("Stream: filter");
        auto deferred = [predicate = closure<Predicate>(predicate)] (
                void *current_stream) {
            LOG("Stream: filter execution");
            Stream<T> *cs = reinterpret_cast<Stream<T>*>(current_stream);
            auto vp = reinterpret_cast<std::vector<T>*>(cs->values.get());
            auto svp = new std::vector<T>;
            for (auto it = vp->begin(); it != vp->end(); ++it) {
                auto &v = *it;
                if (predicate.get()(v)) svp->push_back(std::forward<T>(v));
            }
            cs->values.reset(svp);
        };
        auto s = Stream<T>(*this);
        s.functions.push(deferred);
        return s;
    }

    // For StreamOperator
    template<typename Func,
        typename S = typename std::enable_if<
            std::is_base_of<StreamOperator, Func>::value, Func>::type>
    decltype(auto) operator|(Func &&f) {
        return f.template apply_to(std::forward<Stream<T>>(*this));
    }

    // For transforms that can be applied on T
    template<typename Func,
        typename U = decltype(std::declval<Func>()(std::declval<T>())),
        typename S =
            typename std::enable_if<std::is_same<U, T>::value, U>::type>
    Stream<S> operator|(Func &&f) {
        return map(std::forward<Func>(f));
    }

    // TODO: some checks for amount value
    Stream<T> skip(const size_t amount) {
        LOGV("Stream: skip", amount);
        auto deferred = [=] (void *current_stream) {
            LOGV("Stream: skip execution", amount);
            Stream<T> *cs = reinterpret_cast<Stream<T>*>(current_stream);
            auto vp = reinterpret_cast<std::vector<T>*>(cs->values.get());
            vp->erase(vp->begin(), vp->begin() + amount);
        };
        auto s = Stream<T>(*this);
        s.functions.push(deferred);
        return s;
    }

    Stream<Stream<T>> group(const size_t N) {
        LOGV("Stream: group", N);
        if (N == 0) throw std::runtime_error("N is 0 in group function");
        auto deferred = [=] (void *current_stream) {
            LOGV("Stream: group execution", N);
            Stream<T> *cs = reinterpret_cast<Stream<T>*>(current_stream);
            auto vp = reinterpret_cast<std::vector<T>*>(cs->values.get());
            auto svp = new std::vector<Stream<T>>;
            // TODO: we dont want to reserve a lot of memory
            // It works not ok in case of input stream size = 1 and N = 100000
            // svp->reserve(div_up(vp->size(), N));
            for (auto it = vp->begin();;) {
                auto ivp = new std::vector<T>;
                ivp->insert(ivp->end(), it, it+N);
                svp->push_back(Stream<T>(ivp));
                it += N;
                if (it >= vp->end()) break;
            }
            cs->values.reset(svp);
        };
        auto s = Stream<Stream<T>>(*this);
        s.functions.push(deferred);
        return s;
    }

    // Hope that we have operator+= for type T
    // Hope that we have copy constructor for T
    T sum() {
        LOG("Stream: sum");
        execute();
        LOG("Stream: sum execution");
        auto vp = reinterpret_cast<std::vector<T>*>(values.get());
        auto begin = vp->begin();
        if (begin == vp->end())
            throw std::runtime_error("Stream is empty in sum function");
        T total = *(begin++);
        for (auto it = begin; it != vp->end(); ++it) {
            total += std::forward<T>(*it);
        }
        return total;
    }

    std::ostream &print_to(std::ostream &os, const char *delimiter = " ") {
        LOG("Stream: print_to");
        execute();
        LOG("Stream: print_to execution");
        auto vp = reinterpret_cast<std::vector<T>*>(values.get());
        auto it = vp->begin();
        if (it == vp->end()) return os;
        os << std::forward<T>(*(it++));
        for (; it != vp->end(); ++it) {
            os << delimiter << *it;
        }
        return os;
    }

    std::vector<T> &to_vector() {
        LOG("Stream: to_vector");
        execute();
        LOG("Stream: to_vector execution");
        return *reinterpret_cast<std::vector<T>*>(values.get());
    }

    T &nth(const size_t index) {
        LOGV("Stream: nth", index);
        execute();
        LOGV("Stream: nth execution", index);
        return (*reinterpret_cast<std::vector<T>*>(values.get()))[index];
    }

private:
    std::queue<std::function<void(void*)>> functions;
    std::shared_ptr<void> values;

private:
    void execute() {
        while (!functions.empty()) {
            functions.front()(this);
            functions.pop();
        }
    }

    Stream(const std::shared_ptr<void> &p): values(p) {}

    explicit operator Stream<Stream<T>>() {
        LOG("Stream: explicit cast to Stream<Stream<T>>");
        Stream<Stream<T>> s(this->values);
        s.functions = std::move(this->functions);
        return s;
    }

    template<typename U>
    explicit operator Stream<U>() {
        LOG("Stream: explicit cast from Stream<T> to Stream<U>");
        Stream<U> s(this->values);
        s.functions = std::move(this->functions);
        return s;
    }

    template<typename U>
    static U div_up(const U &a, const U &b) {
        return (a + b - 1) / b;
    }

private:
    template<typename U>
    friend class Stream;
};

} // namespace stream

#endif // STREAM_SRC_STREAM_HPP_
