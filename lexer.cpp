#include <cctype>
#include "lexer.hpp"

using namespace std;

inline char Lexer::eat() {
	return (curr_char = file.get());
}

inline char Lexer::peek() {
	return file.peek();
}

Token Lexer::get_token() {
    // Loop through the stream of characters.
    while (true) {
        // Skip whitespace.
        if (isblank(curr_char)) {
            eat();
            continue;
        }

        // Skip line comments.
        if (curr_char == '-' && peek() == '-') {
            // Eat rest of line for comment.
            do
                eat();
            while (curr_char != EOF &&
                   curr_char != '\n' &&
                   curr_char != '\r');

            if (curr_char == EOF)
                return Token(TOK_EOF);

            continue;
        }

        // Identifiers: [A-Za-z_][A-Za-z0-9_]*
        if (isalpha(curr_char) || curr_char == '_') {
            lex_str = curr_char;
            while (isalnum(eat()) || curr_char == '_')
                lex_str += curr_char;

            // Check against keywords:
            if (lex_str == "let")
                return Token(TOK_LET);
            else if (lex_str == "def")
                return Token(TOK_DEF);
            else if (lex_str == "if")
                return Token(TOK_IF);
            else if (lex_str == "then")
                return Token(TOK_THEN);
            else if (lex_str == "else")
                return Token(TOK_ELSE);
            else if (lex_str == "while")
                return Token(TOK_WHILE);
            else if (lex_str == "each")
                return Token(TOK_EACH);
            else if (lex_str == "return")
                return Token(TOK_RETURN);

            // Technically operators, but are alpha:
            else if (lex_str == "div")
                return Token(TOK_DIV);
            else if (lex_str == "mod")
                return Token(TOK_MOD);
            else if (lex_str == "not")
                return Token(TOK_NOT);
            else if (lex_str == "both")
                return Token(TOK_BOTH);
            else if (lex_str == "either")
                return Token(TOK_EITHER);
            else
                return Token(TOK_IDENTIFIER, lex_str);
        }

        // Strings: '"' (\\.|[^\\"])* '"'
        if (curr_char == '"') {
            lex_str = eat(); // Get first char of string.
            while (eat() != '"') {
                if (curr_char == '\\') {
                    lex_str += curr_char;
                    eat(); // Let by escape chars.
                }
                // TODO: code reduplication
                lex_str += curr_char;
            }

            eat(); // Eat '"'
            return Token(TOK_STRING, lex_str);
        }

        // Characters: '\' '(\\.|[^\\'])' '\''
        if (curr_char == '\'') {
            lex_str = eat(); // Get first char.
            while (eat() != '\'') {
                if (curr_char == '\\') {
                    lex_str += curr_char;
                    eat(); // Let by escape chars.
                }
                // TODO: code reduplication
                lex_str += curr_char;
            }

            eat(); // Eat '\''
            return Token(TOK_CHAR, lex_str);
        }

        // Numbers: [0-9]*(\.[0-9]*)?
        if (isdigit(curr_char)) {
            lex_str = curr_char;
            while (isdigit(eat()))
                lex_str += curr_char;
            if (curr_char == '.' && isdigit(peek())) {
                lex_str += '.';
                while (isdigit(eat()))
                    lex_str += curr_char;
            }

            return Token(TOK_NUMBER, lex_str);
        }

        // Operators: +  | -  | * | /  | %  |
        //            =  | <  | > | <= | >= |
        //            -> | ..
        Token ret_token;
        if (curr_char == '+')
            ret_token = Token(TOK_PLUS);

        else if (curr_char == '-') {
            if (peek() == '>') {
                eat(); // eat the '>'
                ret_token = Token(TOK_ARROW);
            } else
                ret_token = Token(TOK_MINUS);
        }

        else if (curr_char == '*')
            ret_token = Token(TOK_STAR);

        else if (curr_char == '/')
            ret_token = Token(TOK_SLASH);

        else if (curr_char == '%')
            ret_token = Token(TOK_PERCENT);

        else if (curr_char == '=')
            ret_token = Token(TOK_EQ);

        else if (curr_char == '<') {
            if (peek() == '=') {
                eat(); // eat the '='
                ret_token = Token(TOK_LE);
            } else
                ret_token = Token(TOK_LT);
        }

        else if (curr_char == '>') {
            if (peek() == '=') {
                eat(); // eat the '='
                ret_token = Token(TOK_GE);
            } else
                ret_token = Token(TOK_GT);
        }

        else if (curr_char == '.') {
            if (peek() == '.') {
                eat(); // eat the '.'
                ret_token = Token(TOK_ELLIPSIS);
            }
        }

        // Delimiters: () [] {} , \n
        else if (curr_char == '(')
            ret_token = Token(TOK_LPAREN);
        else if (curr_char == ')')
            ret_token = Token(TOK_RPAREN);
        else if (curr_char == '[')
            ret_token = Token(TOK_LBRACKET);
        else if (curr_char == ']')
            ret_token = Token(TOK_RBRACKET);
        else if (curr_char == '{')
            ret_token = Token(TOK_LBRACE);
        else if (curr_char == '}')
            ret_token = Token(TOK_RBRACE);
        else if (curr_char == ',')
            ret_token = Token(TOK_COMMA);
        else if (curr_char == '\n' || curr_char == '\r') {
			if (curr_char == '\r') eat(); // eat subsequent '\n'
			ret_token = Token(TOK_NL);
		}
        else
            ret_token = Token(TOK_UNK, curr_char);

        // EOF, no need to eat.
        if (curr_char == EOF)
            return Token(TOK_EOF);

        // Eat the operator/delimiter character(s) and return.
        eat();
        return ret_token;
    }

}

string Token::as_string() {
    switch (type) {
        case TOK_EOF:    return "EOF";
        case TOK_LET:    return "LET";
        case TOK_DEF:    return "DEF";
        case TOK_IF:     return "IF";
        case TOK_THEN:   return "THEN";
        case TOK_ELSE:   return "ELSE";
        case TOK_WHILE:  return "WHILE";
        case TOK_EACH:   return "EACH";
        case TOK_RETURN: return "RETURN";

        case TOK_IDENTIFIER: return "IDENTIFIER(" + lexeme + ")";
        case TOK_NUMBER:     return "NUMBER(" + lexeme + ")";
        case TOK_CHAR:       return "CHAR(" + lexeme + ")";
        case TOK_STRING:     return "STRING(" + lexeme + ")";

        case TOK_PLUS:    return "PLUS";
        case TOK_MINUS:   return "MINUS";
        case TOK_STAR:    return "STAR";
        case TOK_SLASH:   return "SLASH";
        case TOK_DIV:     return "DIV";
        case TOK_MOD:     return "MOD";
        case TOK_PERCENT: return "PERCENT";

        case TOK_EQ:     return "EQ";
        case TOK_LT:     return "LT";
        case TOK_GT:     return "GT";
        case TOK_LE:     return "LE";
        case TOK_GE:     return "GE";
        case TOK_NOT:    return "NOT";
        case TOK_BOTH:   return "BOTH";
        case TOK_EITHER: return "EITHER";

        case TOK_ARROW:    return "ARROW";
        case TOK_ELLIPSIS: return "ELLIPSIS";

        case TOK_LPAREN:   return "LPAREN";
        case TOK_RPAREN:   return "RPAREN";
        case TOK_LBRACE:   return "LBRACE";
        case TOK_RBRACE:   return "RBRACE";
        case TOK_LBRACKET: return "LBRACKET";
        case TOK_RBRACKET: return "RBRACKET";
        case TOK_COMMA:    return "COMMA";
        case TOK_NL:       return "NL";

        default:           return "@(" + lexeme + ")";
    }
}
