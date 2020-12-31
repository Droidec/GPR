###################################################
# Project configuration
###################################################

# Compiler
CC                  = gcc
# Dynamic library flags
CCFLAGS             = -Wall -Wextra -Werror -O3 -fPIC
# Linker flags
LDFLAGS             = -shared
# Header repository
INC_REP             = include
# Document repository
DOC_REP             = doc
# Source repository
SRC_REP             = src
# Object repository
OBJ_REP             = obj
# Dynamic Library repository
LIB_REP             = lib
# Test repository
TEST_REP            = tests
# Test binaries repository
BIN_TEST_REP        = tests/bin
# Graphic repository
GRAPH_REP           = extras/graph
# Dynamic library name
LIB_NAME            = GPR
# Dynamic library target name
LIB_TARGET_NAME     = lib$(LIB_NAME)
# Dynamic library target name extension
LIB_TARGET_NAME_EXT = .so
# Dynamic library flag
LIB_FLAG_NAME       = -l$(LIB_NAME)
# Graphic script name
GRAPH_NAME          = grapher.py
# Dynamic Library installation directory
LIB_DIR             = ""
# Header installation directory
INC_DIR             = ""
# All header files from header repository
HEADERS             = $(wildcard $(INC_REP)/*.h)
# All source files from source repository
SOURCES             = $(wildcard $(SRC_REP)/*.c)
# All object files from object repository
OBJECTS             = $(patsubst $(SRC_REP)/%.c, $(OBJ_REP)/%.o, $(SOURCES))
# All test files from test repository
TESTS               = $(wildcard $(TEST_REP)/*.c)
# All test executables from test repository
EXECUTABLES         = $(patsubst $(TEST_REP)/%.c, $(BIN_TEST_REP)/%, $(TESTS))
# Indentation style
INDENT_STYLE        = "{AlignEscapedNewlines: Left, \
						AlignTrailingComments: true, \
						AllowAllParametersOfDeclarationOnNextLine: true, \
						AllowShortIfStatementsOnASingleLine: false, \
						AllowShortLoopsOnASingleLine: false, \
						AlwaysBreakBeforeMultilineStrings: false, \
						BreakBeforeBinaryOperators: false, \
						BreakBeforeBraces: Allman, \
						BreakBeforeTernaryOperators: false, \
						BinPackParameters: true, \
						ColumnLimit: 140, \
						ContinuationIndentWidth: 4, \
						Cpp11BracedListStyle: true, \
						DerivePointerBinding: false, \
						ExperimentalAutoDetectBinPacking: false, \
						IndentCaseLabels: true, \
						IndentFunctionDeclarationAfterType: false, \
						IndentWidth: 4, \
						MaxEmptyLinesToKeep: 4, \
						PenaltyBreakBeforeFirstCallParameter: 19, \
						PenaltyBreakComment: 60, \
						PenaltyBreakString: 100, \
						PenaltyBreakFirstLessLess: 120, \
						PenaltyExcessCharacter: 1000000, \
						PenaltyReturnTypeOnItsOwnLine: 60, \
						PointerAlignment: Right, \
						PointerBindsToType: false, \
						SortIncludes: false, \
						SpacesBeforeTrailingComments: 1, \
						Standard: Cpp03, \
						SpacesInParentheses: false, \
						SpacesInAngles: false, \
						SpaceInEmptyParentheses: false, \
						SpacesInCStyleCastParentheses: false, \
						SpaceAfterControlStatementKeyword: true, \
						SpaceBeforeAssignmentOperators: true, \
						TabWidth: 4, \
						UseTab: Never}"

###################################################
# OS detection
###################################################

ifeq '$(findstring ;,$(PATH))' ';'
	detected_OS := Windows
else
	detected_OS := $(shell uname 2>/dev/null || echo Unknown)
	detected_OS := $(patsubst CYGWIN%,Cygwin,$(detected_OS))
endif

ifeq ($(detected_OS),Windows)
	CCFLAGS += -D WIN32
endif
ifeq ($(detected_OS),Darwin)
	CCFLAGS += -D OSX
	LIB_DIR = /usr/local/lib
	INC_DIR = /usr/local/include/GPR
endif
ifeq ($(detected_OS),Linux)
	CCFLAGS += -D LINUX
	LIB_DIR = /usr/lib
	INC_DIR = /usr/include/GPR
endif
ifeq ($(detected_OS),Cygwin)
	CCFLAGS += -D CYGWIN
	LIB_DIR = /usr/lib
	INC_DIR = /usr/include/GPR
endif

###################################################
# Targets
###################################################

.PHONY: all
all: build

.PHONY: debug
debug: CCFLAGS += -D DEBUG -g
debug: build

build: $(OBJ_REP) $(LIB_REP) $(LIB_TARGET_NAME)$(LIB_TARGET_NAME_EXT)

$(LIB_TARGET_NAME)$(LIB_TARGET_NAME_EXT): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(LIB_REP)/$@ $^

$(OBJ_REP)/%.o : $(SRC_REP)/%.c
	$(CC) $(CCFLAGS) -o $@ -c $< -I $(INC_REP)

.PHONY: install
install: $(INC_DIR) $(LIB_DIR)
	cp $(INC_REP)/*.h $(INC_DIR)/
	cp $(LIB_REP)/$(LIB_TARGET_NAME)$(LIB_TARGET_NAME_EXT) $(LIB_DIR)/
	chmod 0755 $(LIB_DIR)/$(LIB_TARGET_NAME)$(LIB_TARGET_NAME_EXT)

.PHONY: uninstall
uninstall:
	rm -rf $(INC_DIR)/
	rm $(LIB_DIR)/$(LIB_TARGET_NAME)$(LIB_TARGET_NAME_EXT)

.PHONY: graph
graph:
	python3 $(GRAPH_REP)/$(GRAPH_NAME) $(INC_REP) $(SRC_REP) -n $(LIB_NAME) -m module -o $(GRAPH_REP) --known-only --view

.PHONY: doc
doc:
	mkdir -p $(DOC_REP)
	doxygen Doxyfile

.PHONY: test
test: $(TEST_REP) $(BIN_TEST_REP) $(EXECUTABLES)

$(BIN_TEST_REP)/%: $(TEST_REP)/%.c
	$(CC) $(CCFLAGS) -o $@ $< $(LIB_FLAG_NAME)

.PHONY: indent
indent:
	@clang-format -i -verbose -style=$(INDENT_STYLE) $(HEADERS) $(SOURCES) $(TESTS) > /dev/null

.PHONY: clean
clean:
	rm -f $(OBJ_REP)/*.o $(LIB_REP)/*
	rm -rf $(BIN_TEST_REP)

.PHONY: cleangraph
cleangraph:
	rm -f $(GRAPH_REP)/*.gv $(GRAPH_REP)/*.pdf

.PHONY: cleandoc
cleandoc:
	rm -rf $(DOC_REP)

.PHONY: clear
clear: clean cleandoc cleangraph

$(OBJ_REP):
	mkdir -p $(OBJ_REP)

$(TEST_REP):
	mkdir -p $(TEST_REP)

$(LIB_REP):
	mkdir -p $(LIB_REP)

$(BIN_TEST_REP):
	mkdir -p $(BIN_TEST_REP)

$(INC_DIR):
	mkdir -p $(INC_DIR)

$(LIB_DIR):
	mkdir -p $(LIB_DIR)
