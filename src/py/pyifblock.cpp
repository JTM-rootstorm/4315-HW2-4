#include "pyifblock.hpp"

PyIfBlock::PyIfBlock(std::string block) : PyStatement() {

}

void PyIfBlock::evaluate() {
    PyStatement::evaluate();
}
