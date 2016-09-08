#include <fstream>
#include <iostream>
#include <string>

using namespace std;

enum TokenType {
    // File characters
    TOK_EOF,

    // Keywords
    TOK_LET,         // let
    TOK_DEF,         // def
    TOK_IF,          // if
    TOK_THEN,        // then
    TOK_ELSE,        // else
    TOK_WHILE,       // while
    TOK_EACH,        // each
    TOK_RETURN,      // return

    // Primary
    TOK_IDENTIFIER,  // [A-Za-z_][A-Za-z0-9_]*
    TOK_NUMBER,      // [0-9]*(\.[0-9]*)?
    TOK_CHAR,        // '\' (\\.|[^\\']) '\''
    TOK_STRING,      // '"' (\\.|[^\\"])* '"'

    // Operators
    TOK_PLUS,        // +
    TOK_MINUS,       // -
    TOK_STAR,        // *
    TOK_SLASH,       // /
    TOK_DIV,         // div
    TOK_MOD,         // mod
    TOK_PERCENT,     // %

    TOK_EQ,          // =
    TOK_LT,          // <
    TOK_GT,          // >
    TOK_LE,          // <=
    TOK_GE,          // >=
    TOK_NOT,         // not
    TOK_BOTH,        // both
    TOK_EITHER,      // either

    TOK_ARROW,       // ->
    TOK_ELLIPSIS,    // ..

    // Delimiters
    TOK_LPAREN,      // (
    TOK_RPAREN,      // )
    TOK_LBRACE,      // {
    TOK_RBRACE,      // }
    TOK_LBRACKET,    // [
    TOK_RBRACKET,    // ]
    TOK_COMMA,       // ,
    TOK_NL,          // \n

    TOK_UNK = -1
};

class Token {
public:
    Token(TokenType type = TOK_UNK, string lexeme = "")
        : type(type), lexeme(lexeme) {}
    Token(TokenType type, char c)
        : type(type), lexeme(string(1, c)) {}
    string as_string();
private:
    TokenType type;
    string lexeme;
};

class Lexer {
public:
    Lexer(ifstream& file) : file(file) {}
    Token get_token();

private:
    ifstream& file;
    char curr_char = ' ';
    string lex_str;
};
