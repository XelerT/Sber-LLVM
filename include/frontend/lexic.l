%option c++
%option yylineno
%option debug
%option yyclass="my_lexer_t"

%{
#include "lexer.hpp"
int n_comments = 0;
%}

WS      [ \v]
TABUL   [\t]
LINE    [\n]+
WINDOWS_R_LINE [\r]+
DIGIT   [0-9]
DIGIT1  [1-9]
ID      [a-z][a-zA-Z_0-9]*
TYPE_ID [A-Z][a-zA-Z_0-9]*

IF      ["i"|"I"]["f"|"F"]
THEN    ["t"|"T"]["h"|"H"]["e"|"E"]["n"|"N"]
ELSE    ["e"|"E"]["l"|"L"]["s"|"S"]["e"|"E"]
WHILE   ["w"|"W"]["h"|"H"]["i"|"I"]["l"|"L"]["e"|"E"]
PRINT   ["p"|"P"]["r"|"R"]["i"|"I"]["n"|"N"]["t"|"T"]
PRINTLN ["p"|"P"]["r"|"R"]["i"|"I"]["n"|"N"]["t"|"T"]["l"|"L"]["n"|"N"]

STRING \"[^\n"]*\"

LINE_COMMENT [-][-]

%s IN_COMMENT

%%

%{
    loc->columns(yyleng);
    loc->step();
    set_location(loc);
%}

{TABUL}           {loc->columns(4);    loc->step();};
{WS}              {loc->columns(1);    loc->step();};
{LINE}            {loc->lines(yyleng); loc->step();};
{WINDOWS_R_LINE}  {};

"/*" { BEGIN(IN_COMMENT);}
<IN_COMMENT>
{
    "*/"      n_comments--; if (!n_comments) BEGIN(INITIAL);
    "/*"      n_comments++;
    [^*\n]+
    "*"    
    \n        yylineno++;
}

{LINE_COMMENT}.*\n

"+"               {return make_plus();};
"-"               {return make_minus();};
"*"               {return make_mul();};
"/"               {return make_div();};
"%"               {return make_modulo();};
";"               {return make_scolon();};
"=="              {return make_equal();};
"<="              {return make_ls_equal();};
">="              {return make_gr_equal();};
"!="              {return make_not_equal();};
">"               {return make_greater();};
"<"               {return make_less();};
"="               {return make_assign();};
{IF}              {return make_if();};
{THEN}            {return make_then();};
{ELSE}            {return make_else();};
{WHILE}           {return make_while();};
{PRINT}           {return make_print();};
{PRINTLN}         {return make_println();};
"true"            {return make_true();};
"false"           {return make_false();};
"!"               {return make_negative();};
"?"               {return make_input();};
"("               {return make_op_pr();};
")"               {return make_cl_pr();};
"{"               {return make_op_br();};
"}"               {return make_cl_br();};

"||"              {return make_or();};
"&&"              {return make_and();};
{ID}              {return make_id();};
{TYPE_ID}         {return make_type_id();};
{DIGIT1}{DIGIT}*  {return make_num();};
{DIGIT}           {return make_num();};

{STRING}          {return make_string();}
.			      {return make_err();};


%%

