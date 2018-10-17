cmake_minimum_required(VERSION 2.8)
project(mypython)

# macro from https://stackoverflow.com/a/31010221
macro(use_cxx11)
    if(CMAKE_VERSION VERSION_LESS "3.1")
        if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
        endif()
    else()
        set(CMAKE_CXX_STANDARD 11)
    endif()
endmacro(use_cxx11)

use_cxx11()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

find_package(Boost COMPONENTS regex REQUIRED)

find_package(BISON REQUIRED)
set(BisonOutput
        ${CMAKE_SOURCE_DIR}/src/rcdgen/parser.tab.hh
        ${CMAKE_SOURCE_DIR}/src/rcdgen/parser.tab.cc
        ${CMAKE_SOURCE_DIR}/src/rcdgen/location.hh
        ${CMAKE_SOURCE_DIR}/src/rcdgen/position.hh
        ${CMAKE_SOURCE_DIR}/src/rcdgen/stack.hh)
if(BISON_FOUND)
    #--report=state
    add_custom_command(
            OUTPUT ${BisonOutput}
            COMMAND ${BISON_EXECUTABLE}
                    --output-file=${CMAKE_SOURCE_DIR}/src/rcdgen/parser.tab.cc
                    --defines=${CMAKE_SOURCE_DIR}/src/rcdgen/parser.tab.hh
                    --warnings=all
                    --feature=all
                    --report=state
                    ${CMAKE_SOURCE_DIR}/src/grammar/parser.yxx
            COMMENT "Generating parser.tab.cc"
    )
endif(BISON_FOUND)

find_package(FLEX REQUIRED)
set(FlexOutput ${CMAKE_SOURCE_DIR}/src/rcdgen/lexer.yy.cc)
if(FLEX_FOUND)
    add_custom_command(
            OUTPUT ${FlexOutput}
            COMMAND ${FLEX_EXECUTABLE}
                    --outfile=${CMAKE_SOURCE_DIR}/src/rcdgen/lexer.yy.cc
                    ${CMAKE_SOURCE_DIR}/src/grammar/lexer.lxx
            COMMENT "Generating lexer.yy.cc"
    )
endif(FLEX_FOUND)

set(SOURCES
        ${BisonOutput} ${FlexOutput}
        src/main.cpp

        src/boolexpr.hpp
        src/expr.hpp
        src/stringexpr.hpp
        src/scanner.hpp
        src/driver.hpp                                      src/driver.cpp

        src/py/pyenvironment.hpp                            src/py/pyenvironment.cpp
        src/py/pyconsole.hpp                                src/py/pyconsole.cpp
        src/py/pyobject.hpp
        src/py/pyconstants.hpp
        src/py/pyerrors.hpp
        src/py/pyfunction.hpp                               src/py/pyfunction.cpp
        src/py/pyfunctionbuilder.hpp                        src/py/pyfunctionbuilder.cpp
        src/py/statements/pystatement.hpp                   src/py/statements/pystatement.cpp
        src/py/statements/pyifblock.hpp                     src/py/statements/pyifblock.cpp

        src/py/env-modules/pymodule.hpp
        src/py/env-modules/mod_functionhelper.hpp           src/py/env-modules/mod_functionhelper.cpp

        src/py/statements/pyreturn.hpp                      src/py/statements/pyreturn.cpp

        src/py/stdfunction/stdinclude.hpp
        src/py/stdfunction/stdprint.hpp                     src/py/stdfunction/stdprint.cpp)

add_executable(mypython ${SOURCES})
target_include_directories(mypython PRIVATE ${Boost_INCLUDE_DIR})
target_link_libraries(mypython ${Boost_LIBRARIES})