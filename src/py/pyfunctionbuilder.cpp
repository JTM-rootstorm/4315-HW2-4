#include "pyfunctionbuilder.hpp"

#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/regex.hpp>

#include "pyfunction.hpp"
#include "statements/pyifblock.hpp"
#include "statements/pyreturn.hpp"

#include <iostream>

PyFunctionBuilder & PyFunctionBuilder::init() {
    function = std::unique_ptr<PyFunction>(new PyFunction());

    return *this;
}

PyFunctionBuilder & PyFunctionBuilder::parseFunctionSignature(std::string sig) {
    boost::regex regex;
    boost::smatch match;
    std::vector<std::string> results;

    /* get the function name and wipe it from the sig */
    regex = R"(^[A-Za-z_]+([\w]+)?)";

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

    boost::regex regex{R"((   .+)(\n)?(((   )(   )+.+(\n)?)+)?)"};
    boost::regex ifregex{R"((   )+(if)[ ]+(.+\:))"};
    boost::regex returnregex{R"((   )+return[ ]+.+(\n)?)"};

    boost::smatch match;

    std::stringstream ss(funcBody);
    std::string target;
    while(std::getline(ss, target, '\n')) {

        if(boost::regex_search(target, match, ifregex)) {
            std::string ifbod;
            boost::regex bodreg{R"((   )else([ ]+)?|(   )(   .+))"};

            std::vector<std::string> ifstatements;

            ifstatements.push_back(match.str());

            while (std::getline(ss, ifbod, '\n')) {
                if(boost::regex_search(ifbod, match, bodreg)) {

                    ifstatements.push_back(match.str());
                }
                else {
                    target = ifbod;

                    std::stringstream fullstate;

                    for (const auto &string : ifstatements) {
                        fullstate << string << '\n';
                    }

                    std::unique_ptr<PyStatement> statement = std::unique_ptr<PyIfBlock>(new PyIfBlock(fullstate.str()));
                    function->funcStatements.push_back(std::move(statement));
                    break;
                }
            }
        }
        else if (boost::regex_search(target, match, returnregex)) {
            std::string ret = match.str();
            boost::trim(ret);
            std::unique_ptr<PyStatement> statement = std::unique_ptr<PyReturn>(new PyReturn(ret));
            function->funcStatements.push_back(std::move(statement));
            continue;
        }


        boost::trim(target);
        std::unique_ptr<PyStatement> statement = std::unique_ptr<PyStatement>(new PyStatement(target));
        function->funcStatements.push_back(std::move(statement));
    }

    return *this;
}

std::unique_ptr<PyFunction> PyFunctionBuilder::build() {
    return std::move(function);
}
