# \ var
MODULE = $(notdir $(CURDIR))
OS      = $(shell uname -o|tr / _)
NOW     = $(shell date +%d%m%y)
REL     = $(shell git rev-parse --short=4 HEAD)
BRANCH  = $(shell git rev-parse --abbrev-ref HEAD)

# \ tool
CURL = curl -L -o
CF   = clang-format-11 -style=file -i

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
.PHONY: doxy doc
doxy: doxy.gen
	rm -rf docs ; doxygen $< 1>/dev/null
doc:

# \ install
.PHONY: install update gz
install: $(OS)_install gz doc
	$(MAKE) update
update: $(OS)_update

.PHONY: GNU_Linux_install GNU_Linux_update
GNU_Linux_install GNU_Linux_update:
	sudo apt update
	sudo apt install -yu `cat apt.txt`

gz:

# \ merge
MERGE  = Makefile README.md .gitignore apt.txt doxy.gen .clang-format $(S)
MERGE += .vscode bin doc lib src tmp

.PHONY: dev shadow release zip

dev:
	git push -v
	git checkout $@
	git pull -v
	git checkout shadow -- $(MERGE)
	$(MAKE) doxy ; git add -f docs

shadow:
	git push -v
	git checkout $@
	git pull -v

.PHONY: release
release:
	git tag $(NOW)-$(REL)
	git push -v --tags
	$(MAKE) shadow

ZIP = tmp/$(MODULE)_$(BRANCH)_$(NOW)_$(REL).src.zip
zip:
	git archive --format zip --output $(ZIP) HEAD
	$(MAKE) doxy ; zip -r $(ZIP) docs
