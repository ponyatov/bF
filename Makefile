# \ var
MODULE = $(notdir $(CURDIR))

CF = clang-format-11 -style=file -i

# \ src
C  = src/$(MODULE).cpp
H  = src/$(MODULE).hpp
S += $(C) $(H)
CP = tmp/$(MODULE).parser.cpp tmp/$(MODULE).lexer.cpp
HP = tmp/$(MODULE).parser.hpp

# \ cfg
CFLAGS = -O0 -g2 -Isrc -Itmp

# \ all
all: bin/$(MODULE) lib/$(MODULE).$(MODULE)
	$^

# \ format
tmp/format_cpp: $(C) $(H)
	$(CF) $? && touch $@

# \ rule
bin/$(MODULE): $(C) $(CP) $(H) $(HP)
	$(CXX) $(CFLAGS) -o $@ $(C) $(CP) $(L)
	$(MAKE) tmp/format_cpp
tmp/%.lexer.cpp: src/%.lex
	flex -o $@ $<
tmp/%.parser.cpp: src/%.yacc
	bison -o $@ $<

# \ doc
doc:

doxy: doxy.gen
	rm -rf docs ; doxygen $< 1>/dev/null

# \ install
install:
	$(MAKE) update
update:
	sudo apt update
	sudo apt install -yu `cat apt.txt`

# \ merge
MERGE  = Makefile README.md .gitignore apt.txt doxy.gen .clang-format $(S)
MERGE += .vscode bin doc lib src tmp

dev:
	git push -v
	git checkout $@
	git pull -v
	git checkout shadow -- $(MERGE)

shadow:
	git push -v
	git checkout $@
	git pull -v

