#ifndef _PUTS_LOGGER_HPP_
#define _PUTS_LOGGER_HPP_

#include "mylog.hpp"


using my::log::Level;
using my::log::Logger;


#define D(value) (d() <<  __FUNCTION__ << "[" << __FILE_NAME__ << ":" << __LINE__ << "] " << value);


struct PutsWriter : public my::log::Writer<PutsWriter> {

    template<Level Lvl>
    void write(std::string_view msg) const {
        puts(msg.data());
    }
};


my::log::Logger<Level::DEBUG, PutsWriter> d();

#endif // _PUTS_LOGGER_HPP_
