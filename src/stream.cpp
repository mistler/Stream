#include "stream.hpp"

#include "logger.hpp"

namespace stream {

Stream<DefaultStreamType> MakeStream() {
    LOG("MakeStream: variadic create");
    auto v = new std::vector<DefaultStreamType>;
    return Stream<DefaultStreamType>(v);
}

} // namespace stream
