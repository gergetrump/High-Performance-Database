// ---------------------------------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------------------------------
%skeleton "lalr1.cc"
%require "3.0.4"
// ---------------------------------------------------------------------------------------------------
// Write a parser header file
%defines
// Define the parser class name
%define api.parser.class {FooParser}
// Create the parser in our namespace
%define api.namespace { foo }
// Use C++ variant to store the values and get better type warnings (compared to "union")
%define api.value.type variant
// With variant-based values, symbols are handled as a whole in the scanner
%define api.token.constructor
// Prefix all tokens
%define api.token.prefix {FOO_}
// Check if variants are constructed and destroyed properly
%define parse.assert
// Trace the parser
%define parse.trace
// Use verbose parser errors
%define parse.error verbose
// Enable location tracking.
%locations
// Pass the compiler as parameter to yylex/yyparse.
%param { foo::FooParseContext &sc }
// ---------------------------------------------------------------------------------------------------
// Added to the header file and parser implementation before bison definitions.
// We include string for string tokens and forward declare the FooParseContext.
%code requires {
#include <string>
#include "data/foo/foo_parse_context.h"
}
// ---------------------------------------------------------------------------------------------------
// Import the compiler header in the implementation file
%code {
foo::FooParser::symbol_type yylex(foo::FooParseContext& sc);
}
// ---------------------------------------------------------------------------------------------------
// Token definitions
%token <int>            INTEGER_VALUE    "integer_value"
%token <std::string>    SCONST           "string_literal"
%token <std::string>    IDENTIFIER       "identifier"
%token LCB              "left_curly_brackets"
%token RCB              "right_curly_brackets"
%token SEMICOLON        "semicolon"
%token INTEGER          "integer"
%token CHAR             "char"
%token COMMA            "comma"
%token FOO              "foo"
%token BAR              "bar"
%token EOF 0            "eof"
// ---------------------------------------------------------------------------------------------------
%type <std::vector<foo::SomeDeclaration>> some_declaration_list;
%type <foo::SomeDeclaration> some_declaration;
%type <foo::Type> some_type;
// ---------------------------------------------------------------------------------------------------
%%

%start foo_statement_list;

foo_statement_list:
    foo_statement_list foo_statment
 |  %empty
    ;

foo_statment:
    FOO IDENTIFIER LCB some_declaration_list RCB SEMICOLON         { sc.defineFoo($2, $4); }
    ;

some_declaration_list:
    some_declaration_list COMMA some_declaration        { $1.push_back($3); std::swap($$, $1); }
 |  some_declaration                                    { $$ = std::vector<foo::SomeDeclaration> { $1 }; }
 |  %empty                                              {}
    ;

some_declaration:
    IDENTIFIER some_type                                { $$ = foo::SomeDeclaration($1, $2); }

some_type:
    INTEGER                                             { $$ = Type::Integer(); }
 |  CHAR LCB INTEGER_VALUE RCB                          { $$ = Type::Char($3); }

%%
// ---------------------------------------------------------------------------------------------------
// Define error function
void foo::FooParser::error(const location_type& l, const std::string& m) {
    sc.Error(l.begin.line, l.begin.column, m);
}
// ---------------------------------------------------------------------------------------------------

