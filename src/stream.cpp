#include "stream.hpp"

#include "logger.hpp"

namespace stream {

pipable<to_vector_pipable, void> to_vector() {
    LOG("Global: to_vector");
    return pipable<to_vector_pipable, void>();
}

} // namespace stream
