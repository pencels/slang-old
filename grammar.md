# Slang Grammar
This is a short grammar that will serve as a guideline to how I write Slang's
parser. The grammar is written in a syntax like
EBNF ([Extended Backus-Naur Form](https://en.wikipedia.org/wiki/Extended_Backus%E2%80%93Naur_Form#Basics)).
It's not perfect (and in some parts may be naïve), but it's nice to have
some foundation to help with the daunting task ahead!

## Statements
Programs are made of a bunch of statements, or newlines ('\n'), until reaching
end-of-file.
```antlr
program: ('\n' | statement)* EOF;
```

Statements are ended by newlines, and vary in being simple statements
([lists of] expressions) or compound statements
(control flow, function/object definitions).
```antlr
statement
    : statement_list '\n'
    | compound_stmt
    ;

statement_list: small_statement (',' small_statement)* [','];

small_statement
    : expression
    | 'return' expression
	| /* May expand to others (e.g. break, continue) */
    ;
```

## Expressions
Expressions include unary, binary (including infix) operations, object member
access, function calls, and atomics (identifiers, literals, and iterables).
Ideally, the precedence for infix operators (as of now only `..`) is the lowest
-- e.g. `+ 1 2 .. - 100 9` binds as `((+ 1 2) .. (- 100 9))`.
There are some ambiguities I'm unsure of (`call` is one) that I'll have to work
out later.
```antlr
expr
	: '(' expr ')'
	| unary_expression
	| binary_expression
	| infix_expression
	| member_access
	| call
	| atom
	;

unary_expression: unop expression;
binary_expression: binop expression expression;
infix_expression: expression infixop expression;

primary: '(' primary ')' | atom | member_access | call;
member_access: '[' primary expression ']';

call: ['('] primary (expression)* [')'];

atom
	: identifier /* Matches regex: [A-Za-z_][A-Za-z0-9_]* */
	| literal
	| enclosure
	;

literal
    : number  /* Matches regex: [0-9]*(\.[0-9]*)? */
    | string  /* Quotes with inner: (\\.|[^\\"])* */
	| char    /* Quotes with inner: (\\.|[^\\'])  */
    | 'True' | 'False'
    ;

enclosure
	: '[' [expr_list] ']'
	| /* May expand to others (e.g. tuple) */
	;

expr_list: expr (',' expr)* [','] ;

unop: 'not';

binop
    : '+'  | '-' | '*' | '/'  | '%'  | 'mod'  | 'div'
    | '='  | '<' | '>' | '<=' | '>=' | 'both' | 'either'
    | '->' | 'let'
    ;

infixop: '..';
```

## Compound statements and blocks
```antlr
compound_stmt
	: block
	| if_stmt
	| while_stmt
	| each_stmt
	| func_def
	| object_def
	;

block: '{' ['\n'] statement* '}';
if_stmt: 'if' expression block ['else' block];
while_stmt: 'while' expression block;
each_stmt: 'each' expression (expression | block);
func_def: 'def' identifier (identifier)* block;
```

## Objects
```antlr
object_def: 'object' identifier object_block;
object_block: '{' ['\n'] object_stmt* '}';

object_stmt
	: object_expr (',' object_expr)* [','] '\n'
	| func_def
	;

object_expr
	: 'has' identifier
	| let_expr
	;
let_expr: 'let' (identifier | member_access) expression;
```
