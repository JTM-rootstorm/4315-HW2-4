#ifndef MYPYTHON_PYRETURN_HPP
#define MYPYTHON_PYRETURN_HPP

#include "pystatement.hpp"

class PyReturn : public PyStatement {
public:
    explicit PyReturn(std::string expr);

    void evaluate() override;
};

#endif //MYPYTHON_PYRETURN_HPP
