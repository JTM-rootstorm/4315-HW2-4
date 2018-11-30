#ifndef MYPYTHON_PYIFSTATEMENT_HPP
#define MYPYTHON_PYIFSTATEMENT_HPP

#include <memory>
#include <vector>

#include "pystatement.hpp"

class PyIfBlock : public PyStatement {
public:
    explicit PyIfBlock(std::string expr);

    void evaluate() override;

private:
    std::vector<std::shared_ptr<PyStatement>> trueBlock;
    std::vector<std::shared_ptr<PyStatement>> falseBlock;

    friend class PyFunction;
};

#endif //MYPYTHON_PYIFSTATEMENT_HPP
