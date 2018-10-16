#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

#include "stdprint.hpp"
#include "../pyenvironment.hpp"

void StdPrint::evaluate(std::vector<std::string> args) {
    std::stringstream ss;

    boost::regex strRegex{R"(\".*\")"};
    boost::regex varRegex{R"([A-Za-z_]+[A-Za-z0-9_]*)"};
    boost::smatch match;

    /* nix the function name */
    args.erase(args.begin(), args.begin() + 1);

    for (auto &string : args) {
        if (boost::regex_search(string, match, strRegex)) {
            boost::replace_all(string, "\"", "");
        }
        else if (boost::regex_search(string, match, varRegex)) {
            std::shared_ptr<PyObject> pyObject = PyEnvironment::Instance().getVar(string);

            if (pyObject == nullptr) {
                PyEnvironment::Instance().pyConsole.logError(PyErrors::ERR_TYPE::NameError, string.c_str());
                return;
            }

            switch ( pyObject->type ) {
                case PyConstants::VarTypes::NUMBER: {
                    int val = pyObject->getData<int>();
                    string = std::to_string(val);
                    break;
                }
                case PyConstants::VarTypes::BOOL: {
                    bool val = pyObject->getData<bool>();
                    if (val) {
                        string = "True";
                    }
                    else {
                        string = "False";
                    }
                    break;
                }
                case PyConstants::VarTypes::STRING: {
                    string = pyObject->getData<std::string>();
                    break;
                }
                case PyConstants::VarTypes::NONE: {
                    PyEnvironment::Instance().pyConsole.logError(PyErrors::ERR_TYPE::NameError, string.c_str());
                    break;
                }
                default: {
                    break;
                }
            }
        }

        ss << string;
    }

    std::string finalstr = ss.str();
    PyEnvironment::Instance().pyConsole.printToConsole(finalstr);
}
