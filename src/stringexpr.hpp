#ifndef MYPYTHON_STRINGEXPR_HPP
#define MYPYTHON_STRINGEXPR_HPP

#include "expr.hpp"

class BaseStringNode : public BaseExpressionNode {
public:
    explicit BaseStringNode(std::string value) : BaseExpressionNode() {
        object = std::make_shared<PyString>(value);
    }

    std::shared_ptr<PyObject> evaluate() const override {
        return object;
    }
private:
    std::shared_ptr<PyObject> object;
};

#endif //MYPYTHON_STRINGEXPR_HPP
