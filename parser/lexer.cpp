/**
 * @file Lexer.cpp
 * @brief Implementación del lexer (analizador léxico) para expresiones matemáticas.
 * 
 * Esta función divide una cadena de entrada en tokens que representan
 * números, variables, operadores, funciones matemáticas, constantes y paréntesis.
 * También detecta y marca errores léxicos mediante tokens inválidos.
 * 
 * Soporta:
 * - Funciones trigonométricas y sus inversas
 * - Constantes matemáticas como pi y e
 * - Variables (x, y, z)
 * - Notación científica (ej: 3.2e-5)
 * - Manejo de errores léxicos comunes
 * 
 * @author Sergio
 * @date 2025
 */

#include "Lexer.hpp"
#include <cctype>
#include <unordered_set>
#include <unordered_map>
#include <sstream>

/**
 * @brief Convierte una cadena de entrada en una lista de tokens.
 * 
 * @param expression Expresión matemática como cadena.
 * @return std::vector<Token> Lista de tokens generados.
 */
std::vector<token> tokenize(const std::string& expression) {
    std::vector<token> tokens;
    std::string current;

    // Conjunto de funciones matemáticas reconocidas por el analizador léxico.
    std::unordered_set<std::string> functions = {"sin", "cos", "tan", "sec", "csc", "cot", "asin", "acos", "atan", "asec", "acsc", "acot", "log", "ln", "log_base", "sqrt", "abs", "nroot", "^"};
    
    // Mapa que relaciona funciones con su aridad (cantidad de argumentos que esperan).
    std::unordered_map<std::string, double> functionsArity = {
        {"sin", 1}, {"cos", 1}, {"tan", 1}, {"sec", 1}, {"csc", 1}, {"cot", 1}, {"asin", 1}, {"acos", 1}, {"atan", 1}, {"asec", 1}, {"acsc", 1}, {"acot", 1}, {"log", 1}, {"ln", 1}, {"log_base", 2}, {"sqrt", 1}, {"abs", 1}, {"nroot", 2}, {"^", 2}
    };
    
    // Constantes predefinidas con precisión razonable (similar a una calculadora científica).
    std::unordered_map<std::string, double> constant = {
        {"pi", 3.141592653589793}, {"e", 2.718281828459045}
    };

    // Variables válidas reconocidas por el sistema.
    std::unordered_set<std::string> variables = {"x", "y", "z"};

    // Bucle principal que recorre la expresión carácter por carácter.
    for (size_t i = 0; i < expression.length(); i++) {
        char character = expression[i];

        // Ignorar espacios en blanco.
        if (std::isspace(character))
        {
            continue;
        }

        //TOKEN: NÚMERO (incluyendo notación científica como 1.23e-4)
        if (std::isdigit(character) || (character == '.' && i + 1 < expression.length() && std::isdigit(expression[i + 1])))
        {
            current = character;
            i++;

            bool seenDot = (character == '.'); // Marca si ya apareció un punto decimal
            bool seenExp = false; // Marca si ya apareció un exponente 'e' o 'E'

            while (i < expression.length())
            {
                char next = expression[i];

                if (std::isdigit(next))
                {
                    current += next;
                }
                else if (next == '.')
                {   
                    // Solo se permite un punto decimal, y no puede venir después de un exponente
                    if (seenDot || seenExp)
                    {
                        throw std::runtime_error("[Lexer Error]: Número mal formado con múltiples puntos decimales.");
                    }
                    current += next;
                    seenDot = true;
                }
                else if (next == 'e' || next == 'E')
                {
                    if (seenExp)
                    {
                        throw std::runtime_error("[Lexer Error]: Número mal formado con múltiples exponentes.");
                    }
                    current += next;
                    seenExp = true;
                    
                    // Validar el carácter después de 'e'
                    i++;
                    if (i >= expression.length())
                    {
                        throw std::runtime_error("[Lexer Error]: Exponente inválido o incompleto.");
                    }

                    char signOrDigit = expression[i];
                    if (signOrDigit == '+' || signOrDigit == '-')
                    {
                        current += signOrDigit;
                        i++;
                        if (i >= expression.length() || !std::isdigit(expression[i]))
                        {
                            throw std::runtime_error("[Lexer Error]: Exponente debe ir seguido de un dígito.");
                        }
                    }
                    else if (!std::isdigit(signOrDigit))
                    {
                        throw std::runtime_error("[Lexer Error]: Exponente debe ir seguido de un dígito.");
                    }

                    // Leer los dígitos del exponente
                    while (i < expression.length() && std::isdigit(expression[i]))
                    {
                        current += expression[i++];
                    }

                    break; // Salimos porque el número ya terminó
                }
                else
                {
                    break; // Ya no es parte del número
                }

                i++;
            }

            i--; // Corregimos para evitar saltarnos un carácter en el for principal
            tokens.emplace_back(tokenType::Number, current);
        }

        //TOKEN: FUNCIONES, CONSTANTES O VARIABLES
        else if (std::isalpha(character))
        {
            current = character;
            // Leemos toda la secuencia de letras (por ejemplo: sin, ln, pi)
            while (i + 1 < expression.length() && std::isalpha(expression[i + 1]))
            {
                current += expression[++i];
            }
            if (functions.count(current))
            {
                tokens.emplace_back(tokenType::Function, current);
            }
            else if (constant.count(current))
            {
                tokens.emplace_back(tokenType::Constant, current);
            }
            else if(variables.count(current) && current.length() == 1){
                tokens.emplace_back(tokenType::Variable, current);
            }
            else
            {
                tokens.emplace_back(tokenType::Invalid, current);
            }
        }
        //TOKEN: PARÉNTESIS
        else if (character == '('){
            tokens.emplace_back(tokenType::LeftParen, "(");
        }
        else if (character == ')'){
            tokens.emplace_back(tokenType::RightParen, ")");
        }

        //TOKEN: OPERADORES
        else if (std::string("+-*/^%=").find(character) != std::string::npos) {
            tokens.emplace_back(tokenType::Operator, std::string(1, character));
        }

        //TOKEN: DESCONOCIDO O NO SOPORTADO
        else {
            tokens.emplace_back(tokenType::Invalid, std::string(1, character));
        }              
    }            
    return tokens;
}