#include <utility>

/* Modified from https://github.com/bingmann/flex-bison-cpp-example/blob/master/src/expression.h
 * but really only to handle arbitrary precision */

#pragma once

#include <cmath>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <memory>
#include <vector>

#include "py/pyobject.hpp"

class BaseExpressionNode {
public:
    virtual ~BaseExpressionNode() = default;

    virtual std::shared_ptr<PyObject> evaluate() const = 0;
};


class ExprConstant : public BaseExpressionNode {
public:
    explicit ExprConstant(mpz_int _value) : BaseExpressionNode(), val(std::move(_value)) {}

    std::shared_ptr<PyObject> evaluate() const override {
        std::shared_ptr<PyInt> pyInt = std::make_shared<PyInt>(val);

        return std::dynamic_pointer_cast<PyObject>(pyInt);
    }

private:
    mpz_int val;
};


class ExprNegate : public BaseExpressionNode {
public:
    explicit ExprNegate(BaseExpressionNode* _node) : BaseExpressionNode(), node(_node) {}

    ~ExprNegate() override { delete node; }

    std::shared_ptr<PyObject> evaluate() const override {
        auto temp = node->evaluate()->getData<mpz_int>();
        temp = -(temp);
        std::shared_ptr<PyInt> pyInt = std::make_shared<PyInt>(temp);

        return std::dynamic_pointer_cast<PyObject>(pyInt);
    }

private:
    BaseExpressionNode* node;
};


class ExprAddSub : public BaseExpressionNode {
public:
    ExprAddSub(BaseExpressionNode* _left, BaseExpressionNode* _right, bool _doSub = false) :
        BaseExpressionNode(), left(_left), right(_right), doSub(_doSub) {}

    ~ExprAddSub() override {
        delete left;
        delete right;
    }

    std::shared_ptr<PyObject> evaluate() const override {
        auto leftVal = left->evaluate()->getData<mpz_int>();
        auto rightVal = right->evaluate()->getData<mpz_int>();
        mpz_int total;

        if (doSub) {
            total = leftVal - rightVal;
        } else {
            total = leftVal + rightVal;
        }

        std::shared_ptr<PyInt> pyInt = std::make_shared<PyInt>(total);
        return std::dynamic_pointer_cast<PyObject>(pyInt);
    }
private:
    bool doSub;

    BaseExpressionNode* left;
    BaseExpressionNode* right;
};


class ExprMultDiv : public BaseExpressionNode {
public:
    ExprMultDiv(BaseExpressionNode* _left, BaseExpressionNode* _right, bool _doDiv = false) :
        BaseExpressionNode(), left(_left), right(_right), doDiv(_doDiv) {}

    ~ExprMultDiv() override {
        delete left;
        delete right;
    }

    std::shared_ptr<PyObject> evaluate() const override {
        auto leftVal = left->evaluate()->getData<mpz_int>();
        auto rightVal = right->evaluate()->getData<mpz_int>();
        mpz_int total;

        if (doDiv) {
            total = leftVal / rightVal;
        } else {
            total = leftVal * rightVal;
        }

        std::shared_ptr<PyInt> pyInt = std::make_shared<PyInt>(total);
        return std::dynamic_pointer_cast<PyObject>(pyInt);
    }
private:
    bool doDiv = false;

    BaseExpressionNode* left;
    BaseExpressionNode* right;
};

class ExprModulo : public BaseExpressionNode {
public:
    ExprModulo(BaseExpressionNode* _left, BaseExpressionNode* _right) :
        BaseExpressionNode(), left(_left), right(_right) {}

    ~ExprModulo() override {
        delete left;
        delete right;
    }

    std::shared_ptr<PyObject> evaluate() const override {
        auto leftVal = left->evaluate()->getData<mpz_int>();
        auto rightVal = right->evaluate()->getData<mpz_int>();
        mpz_int total = leftVal % rightVal;

        std::shared_ptr<PyInt> pyInt = std::make_shared<PyInt>(total);
        return std::dynamic_pointer_cast<PyObject>(pyInt);
    }
private:
    BaseExpressionNode* left;
    BaseExpressionNode* right;
};

class ExprPow : public BaseExpressionNode {
public:
    ExprPow(BaseExpressionNode* _left, BaseExpressionNode* _right) :
        BaseExpressionNode(), left(_left), right(_right) {}

    ~ExprPow() override {
        delete left;
        delete right;
    }

    std::shared_ptr<PyObject> evaluate() const override {
        /*auto leftVal = left->evaluate()->getData<mpz_int>();
        auto rightVal = right->evaluate()->getData<mpz_int>();
        cpp_dec_float_100 total = boost::multiprecision::pow(leftVal.convert_to<cpp_dec_float_100>(), rightVal.convert_to<cpp_dec_float_100>());
        mpz_int goodVal = total.convert_to<mpz_int>();

        std::shared_ptr<PyInt> pyInt = std::make_shared<PyInt>(goodVal);
        return std::dynamic_pointer_cast<PyObject>(pyInt);*/
        return std::make_shared<PyInt>(1);
    }
private:
    BaseExpressionNode* left;
    BaseExpressionNode* right;

    typedef boost::multiprecision::cpp_dec_float_100 cpp_dec_float_100;
};