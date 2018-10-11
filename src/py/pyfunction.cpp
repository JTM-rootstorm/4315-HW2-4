#include "pyenvironment.hpp"

#include <boost/regex.hpp>

/* Regex stuff from https://stackoverflow.com/a/41187191
 * finally can stop beating my head against the wall with that */

void PyFunction::parseFunctionHeader() {
    std::vector<std::string> args;

    boost::regex reggie{R"(("[\w ,\.\-]+")|([A-Za-z_]+[A-Za-z0-9_]*)|([\d]+(\.[\d]+)?))"};

    for (boost::sregex_iterator i = boost::sregex_iterator(funcSignature.begin(), funcSignature.end(), reggie);
            i != boost::sregex_iterator(); ++i) {
        boost::smatch m = *i;

        args.push_back(m.str());
    }

    /*if (funcName == "print") {
        PyFunction::PyPrint(args);
    }*/
}

void PyFunction::PyPrint(std::vector<std::string> args) {
    std::stringstream ss;

    for (auto &string : args) {
        ss << string;
    }
    ss << std::endl;
    std::string finalString = ss.str();

    PyEnvironment::Instance().pyConsole.printToConsole(finalString);
}
