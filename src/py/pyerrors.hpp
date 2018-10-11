//
// Created by mgjus on 10/9/2018.
//

#ifndef MYPYTHON_PYERRORS_HPP
#define MYPYTHON_PYERRORS_HPP

#include <unordered_map>

class PyErrors {
public:
    static PyErrors& Instance() {
        static PyErrors myInstance;

        return myInstance;
    }

    PyErrors(PyErrors const&) = delete;
    PyErrors(PyErrors&&) = delete;
    PyErrors& operator=(PyErrors const&) = delete;
    PyErrors& operator=(PyErrors &&) = delete;

    enum ERR_TYPE {
        NameError,
        TypeError
    };

    std::string getErrorMessage(ERR_TYPE err_type) {
        return logMessages.at(err_type);
    }

protected:
    PyErrors() = default;
    ~PyErrors() = default;

private:
    std::unordered_map<int, std::string> logMessages = {
            {ERR_TYPE::NameError, "NameError: name '%s' is not defined\n"},
            {ERR_TYPE::TypeError, "TypeError: variable '%s' of type '%s' cannot be used\n"}
    };

    /** define some pretty colors **/
    const std::string red   = "\033[1;31m";
    const std::string blue  = "\033[1;36m";
    const std::string norm  = "\033[0m";
};

#endif //MYPYTHON_PYERRORS_HPP
