/* Modified from https://github.com/bingmann/flex-bison-cpp-example/blob/master/src/expression.h
 * but really only to handle arbitrary precision */

#pragma once

#include <cmath>
#include <memory>
#include <vector>
#include <utility>

#include "py/pyobject.hpp"

class BaseExpressionNode {
public:
    virtual ~BaseExpressionNode() = default;

    virtual std::shared_ptr<PyObject> evaluate() const = 0;
};

class ExprConstant : public BaseExpressionNode {
public:
    explicit ExprConstant(int _value) : BaseExpressionNode() {
        object = std::make_shared<PyInt>(_value);
    }

    std::shared_ptr<PyObject> evaluate() const override {
        return object;
    }

private:
    std::shared_ptr<PyObject> object;
};


class ExprNegate : public BaseExpressionNode {
public:
    explicit ExprNegate(BaseExpressionNode* _node) : BaseExpressionNode(), node(_node) {}

    ~ExprNegate() override { delete node; }

    std::shared_ptr<PyObject> evaluate() const override {
        auto val = node->evaluate()->getData<int>();
        val = -(val);

        return std::make_shared<PyInt>(val);
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
        auto leftVal = left->evaluate()->getData<int>();
        auto rightVal = right->evaluate()->getData<int>();
        int total;

        if (doSub) {
            total = leftVal - rightVal;
        } else {
            total = leftVal + rightVal;
        }

        return std::make_shared<PyInt>(total);
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
        auto leftVal = left->evaluate()->getData<int>();
        auto rightVal = right->evaluate()->getData<int>();
        int total;

        if (doDiv) {
            total = leftVal / rightVal;
        } else {
            total = leftVal * rightVal;
        }

        return std::make_shared<PyInt>(total);
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
        auto leftVal = left->evaluate()->getData<int>();
        auto rightVal = right->evaluate()->getData<int>();
        int total = leftVal % rightVal;

        return std::make_shared<PyInt>(total);
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
        auto leftVal = left->evaluate()->getData<int>();
        auto rightVal = right->evaluate()->getData<int>();
        int total = static_cast<int>(std::pow(leftVal, rightVal));

        return std::make_shared<PyInt>(total);
    }
private:
    BaseExpressionNode* left;
    BaseExpressionNode* right;
};