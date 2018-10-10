/* modified from http://www.jonathanbeard.io/tutorials/FlexBisonC++ with a bit of
 * https://github.com/bingmann/flex-bison-cpp-example/blob/master/src/scanner.h sprinkled in */

#ifndef MYPYTHON_SCANNER_HPP
#define MYPYTHON_SCANNER_HPP 1

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "rcdgen/parser.tab.hh"
#include "rcdgen/location.hh"

namespace Py {
    class Scanner : public yyFlexLexer {
    public:
        Scanner(std::istream* arg_yyin) : yyFlexLexer(arg_yyin) {
            yylloc = new Py::Parser::location_type();
        };

        using FlexLexer::yylex;

        virtual int yylex(Py::Parser::semantic_type * const lval, Py::Parser::location_type * location);

    private:
        Py::Parser::semantic_type *yylval = nullptr;
        Py::Parser::location_type *yylloc = nullptr;
    };
}

#endif //MYPYTHON_SCANNER_HPP
