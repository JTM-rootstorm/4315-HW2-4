#ifndef MYPYTHON_PYOBJECT_HPP
#define MYPYTHON_PYOBJECT_HPP

#include <boost/any.hpp>
#include <boost/multiprecision/gmp.hpp>

#include <utility>

#include "pyconstants.hpp"

typedef boost::multiprecision::mpz_int mpz_int;

/* template and boost::any usage idea from https://stackoverflow.com/questions/7968023/c-virtual-template-method */

class PyObject {
public:
    explicit PyObject (PyConstants::VarTypes _type = PyConstants::VarTypes::NONE) : type(_type) {}
    virtual ~PyObject() = default;

    template <typename T>
    void setData(T _data) {
        setDataImpl(boost::any(_data));
    }

    template <typename T>
    T getData() {
        boost::any res = getDataImpl();
        return boost::any_cast<T>(res);
    }

    PyConstants::VarTypes type = PyConstants::VarTypes::NONE;

protected:
    virtual void setDataImpl(boost::any const &value) = 0;
    virtual boost::any getDataImpl() const = 0;
};

class PyInt : public PyObject {
public:
    explicit PyInt (mpz_int _data) : PyObject(PyConstants::VarTypes::NUMBER), data(std::move(_data)) {}

protected:
    void setDataImpl(boost::any const &value) override {
        data = boost::any_cast<mpz_int>(value);
    }

    boost::any getDataImpl() const override {
        return boost::any(data);
    }

private:
    mpz_int data;
};

class PyFloat : public PyObject {
public:
    explicit PyFloat (double _data) :PyObject(PyConstants::VarTypes::NUMBER), data(_data) {}

protected:
    void setDataImpl(boost::any const &value) override {
        data = boost::any_cast<double>(value);
    }

    boost::any getDataImpl() const override {
        return boost::any(data);
    }

private:
    double data;
};

class PyBool : public PyObject {
public:
    explicit PyBool (bool _data) : PyObject(PyConstants::VarTypes::BOOL), data(_data) {}

protected:
    void setDataImpl(boost::any const &value) override {
        data = boost::any_cast<bool>(value);
    }

    boost::any getDataImpl() const override {
        return boost::any(data);
    }

private:
    bool data;
};

class PyString : public PyObject {
public:
    explicit PyString(std::string _data) : PyObject(PyConstants::VarTypes::STRING), data(std::move(_data)) {}

protected:
    void setDataImpl(boost::any const &value) override {
        data = boost::any_cast<std::string>(value);
    }

    boost::any getDataImpl() const override {
        return boost::any(data);
    }
private:
    std::string data;
};

#endif //MYPYTHON_PYOBJECT_HPP
