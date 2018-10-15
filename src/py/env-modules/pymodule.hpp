#ifndef MYPYTHON_PYENVIRONMENTMODULE_HPP
#define MYPYTHON_PYENVIRONMENTMODULE_HPP

#include <string>
#include <vector>
#include <boost/any.hpp>

class PyObject;

class PyModule {
public:
    PyModule() = default;

    virtual void evaluate(std::string func, std::vector<boost::any> args) = 0;
    virtual void evaluate(std::string func, PyObject &object) = 0;
};

#endif //MYPYTHON_PYENVIRONMENTMODULE_HPP
