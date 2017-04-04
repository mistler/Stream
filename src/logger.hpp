#ifndef STREAM_SRC_LOGGER_HPP_
#define STREAM_SRC_LOGGER_HPP_

#ifdef STREAM_VERBOSE
#include <iostream>
#define LOG(str) do { \
    std::cout << (str) << std::endl; \
} while (false)

#define LOGV(str, val) do { \
    std::cout << (str) << ": " << (val) << std::endl; \
} while (false)
#else
#define LOG(str)
#define LOGV(str, val)
#endif

#endif // STREAM_SRC_LOGGER_HPP_
