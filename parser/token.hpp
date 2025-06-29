#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

/**
 * @file Token.hpp
 * @brief Define los tipos de tokens y la estructura de un token individual.
 */

/**
 * @enum TokenType
 * @brief Tipos posibles de tokens reconocidos por el analizador léxico.
 */
enum class tokenType{
    Number,
    Operator,
    LeftParen,
    RightParen,
    Function,
    Constant,
    Variable,
    Invalid,
};

/**
 * @struct Token
 * @brief Representa un token individual con tipo y valor.
 */
struct token {
    tokenType type; ///< Tipo del token
    std::string value; ///< Valor textual del token
    /**
     * @brief Constructor del token.
     * @param t Tipo del token.
     * @param v Valor textual del token.
     */
    token(tokenType t, const std::string& v) : type(t), value(v) {}
};

#endif