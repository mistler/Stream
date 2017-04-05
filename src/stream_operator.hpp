#ifndef STREAM_SRC_STREAM_OPERATOR_HPP_
#define STREAM_SRC_STREAM_OPERATOR_HPP_

#include <cstdlib>
#include <ostream>
#include <utility>

#include "logger.hpp"

namespace stream {

// TODO: think of closure forwarding
struct StreamOperator {};

struct OperatorToVector: public StreamOperator {
    template<typename S>
    auto apply_to(S &&s) {
        return s.to_vector();
    }
};

template<typename Transform>
struct OperatorMap: public StreamOperator {
    OperatorMap(Transform &&transform): _transform(transform) {}
    template<typename S>
    auto apply_to(S &&s) {
        return s.map(_transform);
    }
private:
    Transform _transform;
};

template<typename ...Args>
struct OperatorReduce;

template<typename Accumulator>
struct OperatorReduce<Accumulator>: public StreamOperator {
    OperatorReduce(Accumulator &&accum): _accum(accum) {}
    template<typename S>
    auto apply_to(S &&s) {
        return s.reduce(_accum);
    }
private:
    Accumulator _accum;
};

template<typename IdentityFn, typename Accumulator>
struct OperatorReduce<IdentityFn, Accumulator>: public StreamOperator {
    OperatorReduce(IdentityFn &&identityFn, Accumulator &&accum)
        : _identityFn(identityFn)
        , _accum(accum) {}
    template<typename S>
    auto apply_to(S &&s) {
        return s.reduce(_identityFn, _accum);
    }
private:
    IdentityFn _identityFn;
    Accumulator _accum;
};

template<typename Predicate>
struct OperatorFilter: public StreamOperator {
    OperatorFilter(Predicate &&predicate): _predicate(predicate) {}
    template<typename S>
    auto apply_to(S &&s) {
        return s.filter(_predicate);
    }
private:
    Predicate _predicate;
};

struct OperatorSkip: public StreamOperator {
    OperatorSkip(const size_t amount): _amount(amount) {}
    template<typename S>
    auto apply_to(S &&s) {
        return s.skip(_amount);
    }
private:
    size_t _amount;
};

struct OperatorGroup: public StreamOperator {
    OperatorGroup(const size_t N): _N(N) {}
    template<typename S>
    auto apply_to(S &&s) {
        return s.group(_N);
    }
private:
    size_t _N;
};

struct OperatorSum: public StreamOperator {
    template<typename S>
    auto apply_to(S &&s) {
        return s.sum();
    }
};

struct OperatorPrintTo: public StreamOperator {
    OperatorPrintTo(std::ostream &os, const char *delimiter = " ")
        : _os(os)
        , _delimiter(delimiter) {}
    template<typename S>
    auto &apply_to(S &&s) {
        return s.print_to(_os, _delimiter);
    }
private:
    std::ostream &_os;
    const char *_delimiter;
};

struct OperatorNth: public StreamOperator {
    OperatorNth(const size_t index): _index(index) {}
    template<typename S>
    auto apply_to(S &&s) {
        return s.nth(_index);
    }
private:
    size_t _index;
};

OperatorSkip skip(const size_t amount);
OperatorGroup group(const size_t N);
OperatorSum sum();
OperatorPrintTo print_to(std::ostream &os, const char *delimiter = " ");
OperatorToVector to_vector();
OperatorNth nth(const size_t index);

template<typename Transform>
auto map(Transform &&transform) {
    LOG("Global: map");
    return OperatorMap<Transform>(std::forward<Transform>(transform));
}

template<typename ...Args>
auto reduce(Args &&...args) {
    LOG("Global: reduce");
    return OperatorReduce<Args...>(std::forward<Args>(args)...);
}

template<typename Predicate>
auto filter(Predicate &&predicate) {
    LOG("Global: filter");
    return OperatorFilter<Predicate>(std::forward<Predicate>(predicate));
}

} // namespace stream

#endif // STREAM_SRC_STREAM_OPERATOR_HPP_
