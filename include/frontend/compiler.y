%language "c++"
%skeleton "lalr1.cc"
%defines
%define api.value.type variant
%define parse.error    verbose
%locations
%param     {yy::driver_t* driver}

//-----------------------------------------------------------------------------------------

%code requires {

namespace yy { class driver_t; }

}

%code {

#include "frontend/driver.hpp"
#include <iostream>

namespace yy 
{

parser::token_type yylex(parser::semantic_type* yylval, parser::location_type* loc, driver_t* driver);

}

}

//-----------------------------------------------------------------------------------------

%token
    <std::string> IF 
    <std::string> ELSE
    <std::string> WHILE    
    <std::string> PRINT    
    <std::string> PRINTLN    
    <std::string> THEN
    
    INPUT     "?"
    ASSIGN    "="
    SCOLON    ";"
    <std::string> ID
    <std::string> TYPE_ID
    <std::string> STRING
    <int> NUMBER
    MUL       "*"
    PLUS      "+"
    MINUS     "-"
    DIV       "/"
    MODULO    "%"
    OP_PR     "("
    CL_PR     ")"
    EQUAL     "=="
    NOT_EQUAL "!="
    GREATER   ">"
    NEGATIVE  "!"
    LESS      "<"
    LS_EQUAL  "<="
    GR_EQUAL  ">="
    OP_BR     "{"
    CL_BR     "}"

    AND      "&&"
    OR       "||"

    TRUE     "true"
    FALSE    "false"

    ERR
;

%right ASSIGN
%left  EQUAL NOT_EQUAL GREATER LESS GR_EQUAL LS_EQUAL NEGATIVE
%left  PLUS MINUS OR
%left  MUL DIV MODULO AND
%left INPUT

%left UMINUS

%nterm<std::string>  stms
%nterm<std::string>  stm

%%

program: %empty
        | stms { if (driver->compil_err_flag) {
                    std::cout << "No compilation\n";
                    return driver->print_errors();
                }
                };

stms: stms stm     {}
     | stm         {}
;

stm:  oper           {}
    | no_cond_stm    {}
    | error OP_BR    {}
    | error SCOLON   {}
    | error          {}
;

no_cond_stm:
      expr   SCOLON  {}
    | output SCOLON  {}
    | SCOLON         {} //one shift/reduce

// ------------------------SCOPE_INSTRUCTIONS-----------------------------------------------

block: OP_BR stms CL_BR {}
      | OP_BR CL_BR {};


open_sc:  %empty   {}

close_sc: %empty {}

//------------------------OPERATORS_INSTRUCTIONS-------------------------------------------

oper: if            {}
    | while         {}
;

if:  IF condition  open_sc  act_body close_sc          {};
    |IF condition  open_sc  act_body close_sc ELSE oper {}
    |IF condition  open_sc  act_body close_sc ELSE
                   open_sc  no_cond_stm {} close_sc {}
    |IF condition  open_sc  act_body close_sc ELSE open_sc  block close_sc {}
;
while: WHILE condition  open_sc act_body  close_sc  {};
;

act_body: block                     {}
        | stm                      {}
;

//------------------------ASSIGNMENT_INSTRUCTIONS------------------------------------------

assign: lvalue ASSIGN expr {};

lvalue: ID                 {}
      | TYPE_ID ID         {};

//------------------------EXPRESSIONS_INSTRUCTIONS-----------------------------------------

condition: expr THEN {};
;

comp: expr EQUAL     expr {}
    | expr NOT_EQUAL expr {}
    | expr GREATER   expr {}
    | expr LESS      expr {}
    | expr LS_EQUAL  expr {}
    | expr GR_EQUAL  expr {}
    | expr AND       expr {}
    | expr OR        expr {}
;

expr: NUMBER           {}
    | STRING           {}
    | ID               {}
    | FALSE            {}
    | TRUE             {}
    | TYPE_ID     expr {}
    | expr PLUS   expr {}
    | expr MINUS  expr {}
    | expr MUL    expr {}
    | expr DIV    expr {}
    | expr MODULO expr {}
    | OP_PR expr CL_PR {}
    | comp             {}
    | assign           {}
    | input            {}
    | MINUS expr %prec UMINUS {}
    | NEGATIVE expr {}
;

//------------------------IO_INSTRUCTIONS--------------------------------------------------

output: PRINT   expr {}
      | PRINTLN expr {}
input : INPUT      {}



%%

namespace yy
{

parser::token_type yylex(parser::semantic_type* yylval, parser::location_type* loc, driver_t* driver) {
    return driver->yylex(yylval, loc);
}

void parser::error(const parser::location_type& loc, const std::string& msg)
{
    driver->error(loc, msg);
}

}
