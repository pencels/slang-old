# Slang
Slang is a small language I'm designing for fun, but also with the goal of
learning more about how compilers work (lexing, parsing, code generation...
all that good stuff!).

## Can I try it?
Sure! Only the lexer is finished though. The parser will come later, when I'm
finished writing it.

To check out the lexer, you'll need `g++` and `make` installed. Clone the repo
and run:
```shell-script
$ make slang-lexer
```

You can then feed the executable any `.slang` source files and it'll spit out a
stream of tokens!
```shell-script
$ ./slang-lexer examples/hello_world.slang
LET IDENTIFIER(str) STRING(hello world!) NL IDENTIFIER(print) IDENTIFIER(str) NL
```
Have fun (:

## What's next?
- Parser and grammar refinements
- LLVM IR Code Generation
