.PHONY: clean

CXXFLAGS = -std=c++14
WFLAGS = -Wall -Werror
SRCDIR = src
OBJDIR = obj
LEXSRCS := $(addprefix $(SRCDIR)/lexer/, slang-lexer.cpp lexer.cpp)
LEXOBJS := $(addprefix $(OBJDIR)/lexer/,slang-lexer.o lexer.o)

$(OBJDIR)/lexer/%.o : $(SRCDIR)/lexer/%.cpp
	@echo "Compiling each source ..."
	$(CXX) -g -c $(CXXFLAGS) $(WFLAGS) $< -o $@
	@echo ""

slang-lexer: $(OBJDIR) $(LEXOBJS)
	@echo "Making slang-lexer ..."
	$(CXX) -o slang-lexer $(LEXOBJS)
	@echo "Done!"

$(OBJDIR):
	@mkdir -p obj/lexer

clean:
	rm -f *.o *.exe
	rm -rf obj
	@echo "Clean."
