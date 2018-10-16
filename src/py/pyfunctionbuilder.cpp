#include "pyfunctionbuilder.hpp"

#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/regex.hpp>

#include "pyfunction.hpp"
#include "statements/pyifblock.hpp"
#include "statements/pyreturn.hpp"

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

        function->funcSigVars.push_back(m.str());
    }

    return *this;
}

PyFunctionBuilder & PyFunctionBuilder::parseStatements(std::string funcBody) {
    if (funcBody.empty()) return *this;

    boost::regex regex{R"(((   .+)(\n)?((   )(   )+.+(\n)?)*))"};
    boost::regex ifregex{R"(^if)"};
    boost::regex returnregex{R"(^return)"};

    for (boost::sregex_iterator i = boost::sregex_iterator(funcBody.begin(), funcBody.end(), regex);
         i != boost::sregex_iterator(); ++i) {
        boost::smatch m = *i;
        boost::smatch s;

        std::string str = m.str();
        boost::trim(str);

        if(boost::regex_search(str, s, ifregex)) {
            std::unique_ptr<PyIfBlock> statement = std::unique_ptr<PyIfBlock>(new PyIfBlock(str));
            function->funcStatements.push_back(std::move(statement));
        }
        else if (boost::regex_search(str, s, returnregex)) {
            std::unique_ptr<PyReturn> statement = std::unique_ptr<PyReturn>(new PyReturn(str));
            function->funcStatements.push_back(std::move(statement));
        }
        else {
            std::stringstream ss(str);
            std::string target;

            while(std::getline(ss, target, '\n')) {
                boost::trim(target);
                std::unique_ptr<PyStatement> statement = std::unique_ptr<PyStatement>(new PyStatement(target));
                function->funcStatements.push_back(std::move(statement));
            }
        }
    }

    return *this;
}

std::unique_ptr<PyFunction> PyFunctionBuilder::build() {
    return std::move(function);
}
