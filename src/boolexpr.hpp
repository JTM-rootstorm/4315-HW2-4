//
// Created by mgjus on 10/9/2018.
//

#ifndef MYPYTHON_BOOLEXPR_HPP
#define MYPYTHON_BOOLEXPR_HPP

#include <vector>

#include "expr.hpp"

class BoolExprConstant : public BaseExpressionNode {
public:
    explicit BoolExprConstant(int _value) : BaseExpressionNode() {
        object = std::make_shared<PyBool>(_value);
    }

    std::shared_ptr<PyObject> evaluate() const override {
        return object;
    }

private:
    std::shared_ptr<PyObject> object;
};

class BoolExprAnd : public BaseExpressionNode {
public:
    explicit BoolExprAnd(BaseExpressionNode* _left, BaseExpressionNode* _right) :
        BaseExpressionNode(), left(_left), right(_right) {}

    ~BoolExprAnd() override {
        delete left;
        delete right;
    }

    std::shared_ptr<PyObject> evaluate() const override {
        auto leftVal = left->evaluate()->getData<int>();
        auto rightVal = right->evaluate()->getData<int>();
        int result = leftVal && rightVal;

        return std::make_shared<PyInt>(result);
    }

private:
    BaseExpressionNode* left;
    BaseExpressionNode* right;
};

class BoolExprOr : public BaseExpressionNode {
public:
    explicit BoolExprOr(BaseExpressionNode* _left, BaseExpressionNode* _right) : BaseExpressionNode(), left(_left), right(_right) {}

    ~BoolExprOr() override {
        delete left;
        delete right;
    }

    std::shared_ptr<PyObject> evaluate() const override {
        auto leftVal = left->evaluate()->getData<int>();
        auto rightVal = right->evaluate()->getData<int>();
        int result = leftVal || rightVal;

        return std::make_shared<PyInt>(result);
    }

private:
    BaseExpressionNode* left;
    BaseExpressionNode* right;
};

class BoolExprNot : public BaseExpressionNode {
public:
    explicit BoolExprNot(BaseExpressionNode* _node) : BaseExpressionNode(), node(_node) {}

    ~BoolExprNot() override { delete node; }

    std::shared_ptr<PyObject> evaluate() const override {
        auto result = !(node->evaluate()->getData<int>());

        return std::make_shared<PyInt>(result);
    }

private:
    BaseExpressionNode* node;
};

class BoolExprNotEq : public BaseExpressionNode {
public:
    BoolExprNotEq(BaseExpressionNode* _left, BaseExpressionNode* _right) :
        BaseExpressionNode(), left(_left), right(_right) {}

    ~BoolExprNotEq() override {
        delete left;
        delete right;
    }

    std::shared_ptr<PyObject> evaluate() const override {
        auto leftVal = left->evaluate()->getData<int>();
        auto rightVal = right->evaluate()->getData<int>();

        int result = leftVal != rightVal;

        return std::make_shared<PyInt>(result);
    }

private:
    BaseExpressionNode* left;
    BaseExpressionNode* right;
};

class BoolExprEq : public BaseExpressionNode {
public:
    BoolExprEq(BaseExpressionNode* _left, BaseExpressionNode* _right) :
        BaseExpressionNode(), left(_left), right(_right) {};

    ~BoolExprEq() override {
        delete left;
        delete right;
    }

    std::shared_ptr<PyObject> evaluate() const override {
        auto leftVal = left->evaluate()->getData<int>();
        auto rightVal = right->evaluate()->getData<int>();

        int result = leftVal == rightVal;

        return std::make_shared<PyInt>(result);
    }

private:
    BaseExpressionNode* left;
    BaseExpressionNode* right;
};

class BoolExprLessThan : public BaseExpressionNode {
public:
    BoolExprLessThan(BaseExpressionNode* _left, BaseExpressionNode* _right, bool _alsoEq = false) :
        BaseExpressionNode(), left(_left), right(_right), alsoEq(_alsoEq) {};

    ~BoolExprLessThan() override {
        delete left;
        delete right;
    }

    std::shared_ptr<PyObject> evaluate() const override {
        auto leftVal = left->evaluate()->getData<int>();
        auto rightVal = right->evaluate()->getData<int>();

        int result;

        if(alsoEq) {
            result = leftVal <= rightVal;
        } else {
            result = leftVal < rightVal;
        }

        return std::make_shared<PyInt>(result);
    }

private:
    BaseExpressionNode* left;
    BaseExpressionNode* right;

    bool alsoEq;
};

class BoolExprGreaterThan : public BaseExpressionNode {
public:
    BoolExprGreaterThan(BaseExpressionNode* _left, BaseExpressionNode* _right, bool _alsoEq = false) :
            BaseExpressionNode(), left(_left), right(_right), alsoEq(_alsoEq) {};

    ~BoolExprGreaterThan() override {
        delete left;
        delete right;
    }

    std::shared_ptr<PyObject> evaluate() const override {
        auto leftVal = left->evaluate()->getData<int>();
        auto rightVal = right->evaluate()->getData<int>();

        int result;

        if(alsoEq) {
            result = leftVal >= rightVal;
        } else {
            result = leftVal > rightVal;
        }

        return std::make_shared<PyInt>(result);
    }

private:
    BaseExpressionNode* left;
    BaseExpressionNode* right;

    bool alsoEq;
};

class ExprContext {
public:
    std::vector<BaseExpressionNode*> expressions;

    ~ExprContext() {
        clearExpressions();
    }

    void clearExpressions() {
        for (auto exp : expressions) {
            delete exp;
        }

        expressions.clear();
    }
};

#endif //MYPYTHON_BOOLEXPR_HPP
