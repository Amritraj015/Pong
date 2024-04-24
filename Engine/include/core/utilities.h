#include <stdexcept>

template <typename E> constexpr typename std::underlying_type<E>::type to_underlying(E e) noexcept {
    return static_cast<typename std::underlying_type<E>::type>(e);
}

// WARNING: DO NOT PASS FUNCTIONS into this macro or else it "could" get executed multiple times.
#define ENSURE_SUCCESS(statusCode, message, ...)                                                                       \
    if (statusCode != StatusCode::Successful) {                                                                        \
        VERROR(message, ##__VA_ARGS__)                                                                                 \
        return statusCode;                                                                                             \
    }

// WARNING: DO NOT PASS FUNCTIONS into this macro or else it "could" get executed multiple times.
#define RETURN_ON_FAIL(statusCode)                                                                                     \
    if (statusCode != StatusCode::Successful) return statusCode;
