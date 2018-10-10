//
// Created by mgjus on 10/4/2018.
//
#include <fstream>
#include <sstream>

#include "driver.hpp"
#include "scanner.hpp"
#include "py/pyenvironment.hpp"

namespace Py {
    bool Driver::parse_stream(std::istream &in, const std::string &sname) {
        streamName = sname;

        delete (scanner);
        try {
            scanner = new Py::Scanner(&in);
        } catch (std::bad_alloc &ba) {
            std::cerr << "Failed to allocate scanner: (" << ba.what() << "), exiting..\n";
            exit(EXIT_FAILURE);
        }

        delete (parser);
        try {
            parser = new Py::Parser(*scanner, *this);
        } catch (std::bad_alloc &ba) {
            std::cerr << "Failed to allocate parser: (" << ba.what() << "(, exiting..\n";
            exit(EXIT_FAILURE);
        }

        return (parser->parse() == 0);
    }

    bool Driver::parse_string(const std::string &input, const std::string &sname) {
        std::istringstream iss(input);
        return parse_stream(iss, sname);
    }

    bool Driver::parse_file(const std::string &filename) {
        std::ifstream in(filename.c_str());
        if (!in.good()) return false;
        return parse_stream(in, filename);
    }

    void Driver::error(const Py::Parser::location_type &l, const std::string &m) {
        std::cerr << l << ": " << m << std::endl;
    }

    void Driver::error(const std::string &m) {
        std::cerr << m << std::endl;
    }
}

