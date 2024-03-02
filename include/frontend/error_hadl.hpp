#pragma once
#include <iostream>
#include <string>
#include <cstring>

#include "utils/utils.hpp"

//-----------------------------------------------------------------------------------------

namespace frontend 
{

struct error_t 
{
        std::string msg_ {};
        const std::string prog_str_ {};
        std::string lex_type_ {};
        yy::parser::location_type loc_;
        
        const CONSOLE_CLRS colors {};

        char arrow_len = 5;

        error_t (std::string msg, const std::string prog_str, 
                 std::string lex_type, yy::parser::location_type loc):
                msg_(msg), prog_str_(prog_str), loc_(loc), lex_type_(lex_type) {}

        error_t (std::string msg, std::string lex_type, yy::parser::location_type loc):
                msg_(msg), loc_(loc), lex_type_(lex_type) {}

        void print_h_boarder () const 
        {
                for (auto i = msg_.length(); i > 0; i--)
                        std::cerr << "-";
                std::cerr << "\n";
        }

        void pretty_error_msg () const 
        {
                print_h_boarder();

                std::cerr << colors.RED << msg_ << colors.DEFAULT << std::endl;
                std::cerr << "In line: " << loc_.end.line << '\n';
                std::cerr << "Lexem type : " << lex_type_ << '\n';

                for (int i = 0; i < sizeof(" ") + + loc_.begin.column - arrow_len; i++)
                        std::cerr << " ";

                for (int i = 0; i < arrow_len; i++)
                        std::cerr << colors.RED << "-" << colors.DEFAULT;

                std::cerr << colors.RED << "^" << colors.DEFAULT << std::endl;

                print_h_boarder();
        }
};

}
