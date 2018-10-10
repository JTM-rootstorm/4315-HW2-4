#include "pyenvironment.hpp"
#include "pyfunction.hpp"

#include <regex>

/* Regex stuff from https://stackoverflow.com/a/41187191
 * finally can stop beating my head against the wall with that */

void PyFunction::parseFunctionHeader() {
    std::vector<std::string> args;

    std::regex reggie(R"(("[\w ,\-]+")|([\w\-]+))");

    std::smatch results;

    for (std::sregex_iterator i = std::sregex_iterator(funcSignature.begin(), funcSignature.end(), reggie);
            i != std::sregex_iterator(); ++i) {
        std::smatch m = *i;

        args.push_back(m.str());
    }

    if (funcName == "print") {
        PyFunction::PyPrint(args);
    }
}

void PyFunction::PyPrint(std::vector<std::string> args) {
    std::stringstream ss;

    for (auto &string : args) {
        ss << string;
    }
    std::string finalString = ss.str();

    PyEnvironment::Instance().pyConsole.printToConsole(finalString);
}
