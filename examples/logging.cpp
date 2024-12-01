#include "logging.hpp"


namespace {

const PutsWriter WRITER = PutsWriter{};

} // namespace


my::log::Logger<Level::DEBUG, PutsWriter> d() {
    return Logger<Level::DEBUG, PutsWriter>(WRITER);
}
