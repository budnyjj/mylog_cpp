#ifndef _LOGGER_HPP_
#define _LOGGER_HPP_

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


// Just an additional namespace to separate itself from the rest of the project.
namespace my {

namespace log {

enum class Level {
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
};

template<class Impl>
struct Writer {

    /**
     * Writes message with to the output with specified log level.
     *
     * @param |lvl| log level
     * @param |msg| zero-terminated UTF8-encoded message to write
     *
     * TODO: add return value
     */
    template<Level Lvl>
    void write(std::string_view msg) const {
        static_cast<const Impl&>(*this).template write<Lvl>(msg);
    };
};

struct Buffer {
    typedef std::size_t size_t;

    Buffer() : static_(), length_() { }

    std::string_view view() {
        static_[length_] = '\0';
        return std::string_view(&static_[0], length_);
    }

    size_t put(int8_t value);
    size_t put(int16_t value);
    size_t put(int32_t value);
    size_t put(int64_t value);
    size_t put(uint8_t value);
    size_t put(uint16_t value);
    size_t put(uint32_t value);
    size_t put(uint64_t value);
    size_t put(float value);
    size_t put(double value);
    size_t put(long double value);
    size_t put(std::string_view value);
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

template<Level Lvl, class Writer>
struct Logger {

    // TODO: pass tag
    explicit Logger(const Writer& writer)
     : buffer_{},
       lazy_stream_provider_{},
       writer_{writer}
    {}

    Logger(Logger&&) = default;

    ~Logger() { writer_.template write<Lvl>(buffer_.view()); }

    template<typename V>
    Logger& operator<<(const V& value) {
        if constexpr (std::is_integral<V>::value) {
            buffer_.put(value);
        } else if constexpr (std::is_floating_point<V>::value) {
            buffer_.put(value);
        } else if constexpr (std::is_constructible<std::string_view, V>::value) {
            buffer_.put(value);
        } else if constexpr (std::is_base_of<std::exception, V>::value) {
            // TODO: implement
        } else {
            stream() << value;
        }
        return *this;
    }

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
    const Writer& writer_;
};


// Buffer implementation
// TODO: add more overloads like in
// https://en.cppreference.com/w/cpp/io/basic_ostream/operator_ltlt
// TODO: consider adding char* overload
inline Buffer::size_t
Buffer::put(int8_t value) {
    const auto init_end = &static_[length_];
    const auto [p, _] = std::to_chars(init_end, &static_[STATIC_SIZE], value);
    const auto chars_copied = p - init_end;
    length_ += chars_copied;
    return chars_copied;
}

inline Buffer::size_t
Buffer::put(int16_t value) {
    const auto init_end = &static_[length_];
    const auto [p, _] = std::to_chars(init_end, &static_[STATIC_SIZE], value);
    const auto chars_copied = p - init_end;
    length_ += chars_copied;
    return chars_copied;
}

inline Buffer::size_t
Buffer::put(int32_t value) {
    const auto init_end = &static_[length_];
    const auto [p, _] = std::to_chars(init_end, &static_[STATIC_SIZE], value);
    const auto chars_copied = p - init_end;
    length_ += chars_copied;
    return chars_copied;
}

inline Buffer::size_t
Buffer::put(int64_t value) {
    const auto init_end = &static_[length_];
    const auto [p, _] = std::to_chars(init_end, &static_[STATIC_SIZE], value);
    const auto chars_copied = p - init_end;
    length_ += chars_copied;
    return chars_copied;
}

inline Buffer::size_t
Buffer::put(uint8_t value) {
    const auto init_end = &static_[length_];
    const auto [p, _] = std::to_chars(init_end, &static_[STATIC_SIZE], value);
    const auto chars_copied = p - init_end;
    length_ += chars_copied;
    return chars_copied;
}

inline Buffer::size_t
Buffer::put(uint16_t value) {
    const auto init_end = &static_[length_];
    const auto [p, _] = std::to_chars(init_end, &static_[STATIC_SIZE], value);
    const auto chars_copied = p - init_end;
    length_ += chars_copied;
    return chars_copied;
}

inline Buffer::size_t
Buffer::put(uint32_t value) {
    const auto init_end = &static_[length_];
    const auto [p, _] = std::to_chars(init_end, &static_[STATIC_SIZE], value);
    const auto chars_copied = p - init_end;
    length_ += chars_copied;
    return chars_copied;
}

inline Buffer::size_t
Buffer::put(uint64_t value) {
    const auto init_end = &static_[length_];
    const auto [p, _] = std::to_chars(init_end, &static_[STATIC_SIZE], value);
    const auto chars_copied = p - init_end;
    length_ += chars_copied;
    return chars_copied;
}

inline Buffer::size_t
Buffer::put(float value) {
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
Buffer::put(double value) {
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
Buffer::put(long double value) {
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
Buffer::put(std::string_view value) {
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
        n
    );
    std::char_traits<char>::copy(&static_[length_], s, num_chars_to_copy);
    length_ += num_chars_to_copy;
    return num_chars_to_copy;
}

} // engine

} // namespace log

#endif // _MY_LOG_HPP_
