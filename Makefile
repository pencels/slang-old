.PHONY: slang clean

CC = g++
CCFLAGS = -std=c++14
WFLAGS = -Wall -Werror
SRCS = slang-lexer.cpp lexer.cpp
LEXOBJS = slang-lexer.o lexer.o

%.o: %.cpp
	@echo "Compiling each source ..."
	$(CC) -g -c $(CCFLAGS) $(WFLAGS) $<
	@echo ""

slang-lexer: $(LEXOBJS)
	@echo "Making slang-lexer ..."
	$(CC) -o slang-lexer $(LEXOBJS)
	@echo "Done!"

clean:
	rm -f *.o *.exe
	@echo "Clean."
