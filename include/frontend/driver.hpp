#pragma once

#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include <memory>
#include <FlexLexer.h>
#include "lexer.hpp"

#include "error_hadl.hpp"
#include "grammar.tab.hpp"

//-----------------------------------------------------------------------------------------

namespace yy 
{

class driver_config_t 
{
        private:
                std::ifstream f_input = std::ifstream();
        public:
                std::istream& input = std::cin;

                driver_config_t () {};
                driver_config_t (int argc, const char** argv) :
                                f_input(get_f_input(argc, argv)), input(f_input) 
                {

                        if (!input.good())
                                throw (std::runtime_error("Stasyan: No such file or directory: " + std::string(argv[1])));
                }

                std::ifstream get_f_input (int argc, const char** argv) 
                {
                        for (int i = 1; i < argc; i++) {
                                std::string arg = argv[i];
                                std::ifstream file_input(argv[i]);
                                
                                return file_input;
                        }
                        return std::ifstream();
                }
                ~driver_config_t () 
                {
                        f_input.close();
                }
};

//-----------------------------------------------------------------------------------------

class driver_t 
{
        private:
                std::unique_ptr<driver_config_t> dr_config_;
                std::unique_ptr<my_lexer_t> plex_;
                std::vector<std::string> prog_data_;
        public:
                std::vector<frontend::error_t> compil_err;
                bool compil_err_flag = false;

                driver_t (std::unique_ptr<my_lexer_t> plex, 
                          std::unique_ptr<driver_config_t> dr_config):
                        plex_(std::move(plex)), dr_config_(std::move(dr_config)) 
                {
                        plex_->my_lexer_t::switch_streams(dr_config_->input, std::cout);

                        std::string prog_str;
                        while (getline(dr_config_->input, prog_str)) {
                                prog_data_.push_back(prog_str);
                        }

                        dr_config_->input.clear();
                        dr_config_->input.seekg(0, std::ios::beg);
                }

                parser::token_type yylex (parser::semantic_type* yylval, parser::location_type* loc) 
                {
                        parser::token_type tt = static_cast<parser::token_type>(plex_->yylex(loc));

                        if (tt) {
                                std::cout << "Token type: ";
                                print_token_type(tt);
                                std::cout << ", Token value: ";
                                std::cout << plex_->YYText() << std::endl;
                        }
                        return tt;
                }

                void print_token_type (parser::token_type tt)
                {
                        using ptt = parser::token_type;

                        switch (tt)
                        {
                                case ptt::IF:        std::cout << "IF";            break;        
                                case ptt::ELSE:      std::cout << "ELSE";          break;        
                                case ptt::WHILE:     std::cout << "WHILE";         break;        
                                case ptt::PRINT:     std::cout << "OUTPUT";        break;        
                                case ptt::PRINTLN:   std::cout << "OUTPUT";        break;        
                                case ptt::INPUT:     std::cout << "INPUT";         break;        
                                case ptt::ASSIGN:    std::cout << "ASSIGN";        break;        
                                case ptt::SCOLON:    std::cout << "SCOLON";        break;        
                                case ptt::ID:        std::cout << "ID";            break;         
                                case ptt::TYPE_ID:   std::cout << "TYPE_ID";       break;        
                                case ptt::NUMBER:    std::cout << "NUMBER";        break;        
                                case ptt::MUL:       std::cout << "MUL";           break;        
                                case ptt::PLUS:      std::cout << "PLUS";          break;        
                                case ptt::MINUS:     std::cout << "MINUS";         break;        
                                case ptt::DIV:       std::cout << "DIV";           break;        
                                case ptt::MODULO:    std::cout << "MODULO";        break;        
                                case ptt::THEN:      std::cout << "SYNT";          break;        
                                case ptt::OP_PR:     std::cout << "OP_PR";         break;        
                                case ptt::CL_PR:     std::cout << "CL_PR";         break;        
                                case ptt::EQUAL:     std::cout << "EQUAL";         break;        
                                case ptt::NOT_EQUAL: std::cout << "NOT_EQUAL";     break;        
                                case ptt::NEGATIVE:  std::cout << "NEGATIVE";      break;        
                                case ptt::GREATER:   std::cout << "GREATER";       break;        
                                case ptt::LESS:      std::cout << "LESS";          break;        
                                case ptt::LS_EQUAL:  std::cout << "LS_EQUAL";      break;        
                                case ptt::GR_EQUAL:  std::cout << "GR_EQUAL";      break;        
                                case ptt::OP_BR:     std::cout << "OP_BR";         break;        
                                case ptt::CL_BR:     std::cout << "CL_BR";         break;        
                                case ptt::AND:       std::cout << "AND";           break;        
                                case ptt::OR:        std::cout << "OR";            break;
                                case ptt::TRUE:      std::cout << "TRUE";          break;
                                case ptt::FALSE:     std::cout << "FALSE";         break;
                                case ptt::STRING:    std::cout << "STRING";        break;
                                case ptt::ERR:       std::cout << "You have some syntax errors:"; break;  
                                case 0:                                             break;     
                                default:             std::cout << "UNKOWN TOKEN!";  break;        
                        }
                }

                bool parse () 
                {
                        parser parser(this);
                        bool res = parser.parse();

                        return !res;
                }

                void error (const class location& loc, const std::string& msg)
                {
                        compil_err_flag = true;

                        yy::parser::location_type my_loc = plex_->get_location();
                        const frontend::error_t my_error {msg, prog_data_[my_loc.begin.line -1],
                                                          plex_->get_lex_type(), my_loc};
                        compil_err.push_back(std::move(my_error));
                }

                int print_errors() const 
                {
                        for (const auto& err : compil_err) {
                                err.pretty_error_msg();
                        }
                        return 1;
                }
};

}
