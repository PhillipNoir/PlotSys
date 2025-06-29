/**
 * @file Lexer.hpp
 * @brief Declaración de la función tokenize y definición del enum tokenType.
 */
#ifndef LEXER_HPP
#define LEXER_HPP

#include "Token.hpp"
#include <vector>
#include <string>

/**
 * @brief Convierte una cadena de entrada en una lista de tokens.
 * 
 * @param expression Expresión matemática como cadena.
 * @return std::vector<Token> Lista de tokens generados.
 */
std::vector<token> tokenize(const std::string& expression);

#endif // LEXER_HPP
