#pragma once

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif

#include <iostream>
#include <memory>
#include <fstream>
#include "grammar.tab.hpp"

//-------------------------------THIS_YYLEX_WILL_BE_CALLED---------------------------------

#define YY_DECL int my_lexer_t::yylex (yy::parser::location_type* loc)

//-----------------------------------------------------------------------------------------

class my_lexer_t : public yyFlexLexer 
{
        std::string lex_type;
        yy::parser::location_type yylloc;

        public:
                virtual int yylex (yy::parser::location_type* loc);

                yy::parser::location_type get_location () const {return yylloc;};
                std::string get_lex_type () const {return lex_type;};
        
                void set_location (yy::parser::location_type* loc) 
                {
                        if (loc)
                                yylloc = *loc;
                }

        private:

        int make_plus () 
        {
                lex_type = "operator";
                return yy::parser::token_type::PLUS;
        }

        int make_minus () 
        {
                lex_type = "operator";
                return yy::parser::token_type::MINUS;
        }

        int make_mul () 
        {
                lex_type = "operator";
                return yy::parser::token_type::MUL;
        }

        int make_div () 
        {
                lex_type = "operator";
                return yy::parser::token_type::DIV;
        }

        int make_modulo () 
        {
                lex_type = "operator";
                return yy::parser::token_type::MODULO;
        }

        int make_scolon () 
        {
                lex_type = "eoe";
                return yy::parser::token_type::SCOLON;
        }

        int make_equal ()
        {
                lex_type = "operator";
                return yy::parser::token_type::EQUAL;
        }

        int make_not_equal () 
        {
                lex_type = "operator";
                return yy::parser::token_type::NOT_EQUAL;
        }

        int make_ls_equal () 
        {
                lex_type = "operator";
                return yy::parser::token_type::LS_EQUAL;
        }

        int make_gr_equal () 
        {
                lex_type = "operator";
                return yy::parser::token_type::GR_EQUAL;
        }

        int make_less () 
        {
                lex_type = "operator";
                return yy::parser::token_type::LESS;
        }

        int make_greater () 
        {
                lex_type = "operator";
                return yy::parser::token_type::GREATER;
        }

        int make_assign () 
        {
                lex_type = "operator";

                return yy::parser::token_type::ASSIGN;
        }

        int make_if () 
        {
                lex_type = "cond operator";
                return yy::parser::token_type::IF;
        }

        int make_while () 
        {
                lex_type = "cond operator";
                return yy::parser::token_type::WHILE;
        }

        int make_else () 
        {
                lex_type = "cond operator";
                return yy::parser::token_type::ELSE;
        }

        int make_output () 
        {
                lex_type = "iostream";
                return yy::parser::token_type::OUTPUT;
        }

        int make_input () 
        {
                lex_type = "iostream";
                return yy::parser::token_type::INPUT;
        }

        int make_op_pr () 
        {
                lex_type = "bracket";
                return yy::parser::token_type::OP_PR;
        }

        int make_cl_pr () 
        {
                lex_type = "bracket";
                return yy::parser::token_type::CL_PR;
        }

        int make_op_br () 
        {
                lex_type = "bracket";
                return yy::parser::token_type::OP_BR;
        }

        int make_cl_br () 
        {
                lex_type = "bracket";
                return yy::parser::token_type::CL_BR;
        }

        int make_err () 
        {
                lex_type = "ERROR";
                return yy::parser::token_type::ERR;
        }

        int make_num () 
        {
                lex_type = "number";
                return yy::parser::token_type::NUMBER;;
        }

        int make_id () 
        {
                lex_type = "name";
                return yy::parser::token_type::ID;
        }

        int make_type_id () 
        {
                lex_type = "type name";
                return yy::parser::token_type::TYPE_ID;
        }

        int make_and () 
        {
                lex_type = "cond_operator";
                return yy::parser::token_type::AND;
        }

        int make_or () 
        {
                lex_type = "cond_operator";
                return yy::parser::token_type::OR;
        }

        int make_negative () 
        {
                lex_type = "negative";
                return yy::parser::token_type::NEGATIVE;
        }
};
