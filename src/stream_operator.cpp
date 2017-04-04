#include "stream_operator.hpp"

#include "logger.hpp"

namespace stream {

OperatorToVector to_vector() {
    LOG("Global: to_vector");
    return OperatorToVector();
}

OperatorSkip skip(const size_t amount) {
    LOG("Global: skip");
    return OperatorSkip(amount);
}

OperatorGroup group(const size_t N) {
    LOG("Global: group");
    return OperatorGroup(N);
}

OperatorSum sum() {
    LOG("Global: sum");
    return OperatorSum();
}

OperatorPrintTo print_to(std::ostream &os, const char *delimiter) {
    LOG("Global: print_to");
    return OperatorPrintTo(os, delimiter);
}

OperatorNth nth(const size_t index) {
    LOG("Global: nth");
    return OperatorNth(index);
}

} // namespace stream
