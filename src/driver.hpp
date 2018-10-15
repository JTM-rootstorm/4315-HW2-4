/* modified from http://www.jonathanbeard.io/tutorials/FlexBisonC++ with a bit of
 * https://github.com/bingmann/flex-bison-cpp-example/blob/master/src/driver.h sprinkled in */

#ifndef MYPYTHON_DRIVER_HPP
#define MYPYTHON_DRIVER_HPP

#include <string>
#include <vector>

#include "scanner.hpp"
#include "rcdgen/parser.tab.hh"

namespace Py {
    class Driver {
    public:
        Driver() = default;

        virtual ~Driver() = default;

        std::string streamName;

        bool parse_stream(std::istream& in, const std::string& sname = "stream input");

        bool parse_string(const std::string& input, const std::string& sname = "string stream");

        bool parse_file(const std::string& filename);

        void error(const class location& l, const std::string& m);

        void error(const std::string& m);

    private:
        Py::Parser *parser = nullptr;
        Py::Scanner *scanner = nullptr;
    };
}

#endif //MYPYTHON_DRIVER_HPP
