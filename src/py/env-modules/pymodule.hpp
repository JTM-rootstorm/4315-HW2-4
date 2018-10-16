#ifndef MYPYTHON_PYENVIRONMENTMODULE_HPP
#define MYPYTHON_PYENVIRONMENTMODULE_HPP

#include <string>
#include <vector>
#include <boost/any.hpp>

class PyObject;

class PyModule {
public:
    PyModule() = default;

    virtual void evaluate(const std::string &func, std::vector<boost::any> args) = 0;
    virtual void evaluate(const std::string &func, const std::string &varName, PyObject &object) = 0;
};

#endif //MYPYTHON_PYENVIRONMENTMODULE_HPP
