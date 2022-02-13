#ifndef _LOG_HPP_
#define _LOG_HPP_

#include <cstdio>
#include <cstdint>

#include <algorithm>
#include <array>
#include <charconv>
#include <memory>
#include <ostream>
#include <string>
#include <string_view>
#include <utility>


namespace log {

enum class Level {
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
};

struct Writer {

    /**
     * Writes message with to the output with specified log level.
     *
     * @param |lvl| log level
     * @param |msg| zero-terminated UTF8-encoded message to write
     *
     * TODO: add return value
     */
    virtual void write(const Level lvl, const char* msg) const = 0;

    virtual ~Writer() {}
};

struct Buffer {
    typedef std::size_t size_t;

    Buffer() : static_(), length_() { }

    const char* c_str() {
        static_[length_] = '\0';
        return &static_[0];
    }

    size_t put(const int value);
    size_t put(const float value);
    size_t put(const double value);
    size_t put(const long double value);
    size_t put(const std::string_view& value);
    size_t put(const char* s, std::streamsize n);

    /// The actual size of static storage.
    static const size_t STATIC_SIZE = 120;

 private:
    /// The maximum number of characters (without termination symbol)
    /// that is allowed to store in the static storage.
    static const size_t STATIC_MAX_CHARS = STATIC_SIZE - 1;

    std::array<char, STATIC_SIZE> static_;
    size_t length_;
};

struct StdBufferAdapter final : public std::streambuf {

    explicit StdBufferAdapter(Buffer& buffer) : buffer_(buffer) {}

    std::streamsize xsputn(
        const char* s,
        std::streamsize n
    ) override {
        return buffer_.put(s, n);
    }

    // TODO: implement properly
    int overflow(int c) override {
        return 0;
    };

 private:
    Buffer& buffer_;
};

struct StdOStreamProvider {

    explicit StdOStreamProvider(Buffer& buffer)
     : adapter_(buffer), stream(&adapter_) {}

    std::ostream stream;

 private:
    StdBufferAdapter adapter_;
};

struct Logger {

    // TODO: pass tag
    explicit Logger(const Level level, const Writer& writer)
     : buffer_{},
       lazy_stream_provider_{},
       level_{level},
       writer_{writer}
    {}

    Logger(Logger&&) = default;

    ~Logger() { writer_.write(level_, buffer_.c_str()); }

    template<typename T>
    Logger& operator<<(const T& value);

 private:
    std::ostream& stream() {
        if (!lazy_stream_provider_) {
            lazy_stream_provider_ =
                std::make_unique<StdOStreamProvider>(buffer_);
        }
        return lazy_stream_provider_->stream;
    }

    Buffer buffer_;
    std::unique_ptr<StdOStreamProvider> lazy_stream_provider_;
    const Level level_;
    const Writer& writer_;
};


// Buffer implementation
// TODO: add more overloads like in
// https://en.cppreference.com/w/cpp/io/basic_ostream/operator_ltlt
// TODO: consider adding char* overload
inline Buffer::size_t
Buffer::put(const int value) {
    const auto init_end = &static_[length_];
    const auto [p, _] = std::to_chars(init_end, &static_[STATIC_SIZE], value);
    const auto chars_copied = p - init_end;
    length_ += chars_copied;
    return chars_copied;
}

inline Buffer::size_t
Buffer::put(const float value) {
    const auto max_chars_to_copy = STATIC_SIZE - length_;
    const auto chars_to_copy = snprintf(
        &static_[length_], max_chars_to_copy, "%f", value);
    // TODO: handle scenario when chars_to_copy > max_chars_to_copy
    if (chars_to_copy >= 0 && chars_to_copy <= max_chars_to_copy) {
        length_ += chars_to_copy;
    }
    return chars_to_copy;
}

inline Buffer::size_t
Buffer::put(const double value) {
    const auto max_chars_to_copy = STATIC_SIZE - length_;
    const auto chars_to_copy = snprintf(
        &static_[length_], max_chars_to_copy, "%lf", value);
    // TODO: handle scenario when chars_to_copy > max_chars_to_copy
    if (chars_to_copy >= 0 && chars_to_copy <= max_chars_to_copy) {
        length_ += chars_to_copy;
    }
    return chars_to_copy;
}

inline Buffer::size_t
Buffer::put(const long double value) {
    const auto max_chars_to_copy = STATIC_SIZE - length_;
    const auto chars_to_copy = snprintf(
        &static_[length_], max_chars_to_copy, "%Lf", value);
    // TODO: handle scenario when chars_to_copy > max_chars_to_copy
    if (chars_to_copy >= 0 && chars_to_copy <= max_chars_to_copy) {
        length_ += chars_to_copy;
    }
    return chars_to_copy;
}

inline Buffer::size_t
Buffer::put(const std::string_view& value) {
    // TODO: handle scenario when value length >= STATIC_MAX_LENGTH - length_
    const auto chars_copied =
        value.copy(&static_[length_], STATIC_MAX_CHARS - length_);
    length_ += chars_copied;
    return chars_copied;
}

inline Buffer::size_t
Buffer::put(const char* s, std::streamsize n) {
    // TODO: handle scenario when value n >= STATIC_MAX_LENGTH - length_
    const auto num_chars_to_copy = std::min(
        static_cast<std::streamsize>(STATIC_MAX_CHARS - length_),
        n);
    std::char_traits<char>::copy(&static_[length_], s, num_chars_to_copy);
    length_ += num_chars_to_copy;
    return num_chars_to_copy;
}


// Logger implementation
// TODO: add more cases
template<typename T>
inline Logger&
Logger::operator<<(const T& value) {
    if constexpr (std::is_integral<T>::value) {
        buffer_.put(value);
    } else if constexpr (std::is_floating_point<T>::value) {
        buffer_.put(value);
    } else if constexpr (std::is_constructible<std::string_view, T>::value) {
        buffer_.put(value);
    } else if constexpr (std::is_base_of<std::exception, T>::value) {
        // TODO: implement
    } else {
        stream() << value;
    }
    return *this;
}

} // namespace log

#endif // _LOG_HPP_
