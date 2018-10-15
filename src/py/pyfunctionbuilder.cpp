#include "pyfunctionbuilder.hpp"

#include <boost/algorithm/string/replace.hpp>
#include <boost/regex.hpp>

#include "pyfunction.hpp"

PyFunctionBuilder & PyFunctionBuilder::init() {
    function = std::unique_ptr<PyFunction>(new PyFunction());

    return *this;
}

PyFunctionBuilder & PyFunctionBuilder::parseFunctionSignature(std::string sig) {
    boost::regex regex;
    boost::smatch match;
    std::vector<std::string> results;

    /* get the function name and wipe it from the sig */
    regex = R"(^[A-Za-z_]+[\w]+)";

    if (boost::regex_search(sig, match, regex)) {
        function->funcName = match.str();

        std::string::size_type i = sig.find(function->funcName);
        if (i != std::string::npos) {
            sig.erase(i, function->funcName.size());
        }
    }

    /* grab function arguments */
    regex = R"(("[\w ,\.\-]+")|([A-Za-z_]+[A-Za-z0-9_]*)|([\d]+(\.[\d]+)?))";

    for (boost::sregex_iterator i = boost::sregex_iterator(sig.begin(), sig.end(), regex);
         i != boost::sregex_iterator(); ++i) {
        boost::smatch m = *i;

        function->funcArgs.emplace(m.str(), nullptr);
    }

    return *this;
}

PyFunctionBuilder & PyFunctionBuilder::parseStatements(std::string funcBody) {
    boost::regex regex{R"((   .+)\n((   )(   )+.+\n)*)"};

    std::vector<std::string> results;

    for (boost::sregex_iterator i = boost::sregex_iterator(funcBody.begin(), funcBody.end(), regex);
         i != boost::sregex_iterator(); ++i) {
        boost::smatch m = *i;

        std::string val = m.str();

        results.push_back(val);
    }

    return *this;
}

std::unique_ptr<PyFunction> PyFunctionBuilder::build() {
    return std::move(function);
}
