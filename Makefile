# Compiler
CC              = gcc
# Dynamic library flags
CFLAGS          = -Wall -Wextra -Werror -O3 -fPIC
# Linker flags
LDFLAGS         = -shared
# Header repository
INC_REP         = include
# Source repository
SRC_REP         = src
# Object repository
OBJ_REP         = obj
# Dynamic Library repository
LIB_REP         = lib
# Test repository
TEST_REP        = tests
# Test binaries repository
BIN_TEST_REP    = tests/bin
# Graphic repository
GRAPH_REP       = extras/graph
# Dynamic library name
LIB_NAME        = GPR
# Dynamic library target name
LIB_TARGET_NAME = lib$(LIB_NAME)
# Dynamic library flag
LIB_FLAG_NAME   = -l$(LIB_NAME)
# Graphic script name
GRAPH_NAME      = cinclude2dot
# Dynamic Library installation directory
LIB_DIR         = /usr/lib
# Header installation directory
INC_DIR         = /usr/include/GPR
# All header files from header repository
HEADERS         = $(wildcard $(INC_REP)/*.h)
# All source files from source repository
SOURCES         = $(wildcard $(SRC_REP)/*.c)
# All object files from object repository
OBJECTS         = $(patsubst $(SRC_REP)/%.c, $(OBJ_REP)/%.o, $(SOURCES))
# All test files from test repository
TESTS           = $(wildcard $(TEST_REP)/*.c)
# All test executables from test repository
EXECUTABLES     = $(patsubst $(TEST_REP)/%.c, $(BIN_TEST_REP)/%, $(TESTS))
# Indentation style
INDENT_STYLE    = "{AlignEscapedNewlines: Left, \
	                AlignTrailingComments: true, \
	                AllowAllParametersOfDeclarationOnNextLine: true, \
	                AllowShortIfStatementsOnASingleLine: false, \
	                AllowShortLoopsOnASingleLine: false, \
	                AlwaysBreakBeforeMultilineStrings: false, \
	                BreakBeforeBinaryOperators: false, \
	                BreakBeforeTernaryOperators: false, \
	                BinPackParameters: true, \
	                ColumnLimit: 140, \
	                DerivePointerBinding: false, \
	                ExperimentalAutoDetectBinPacking: false, \
	                IndentCaseLabels: true, \
	                MaxEmptyLinesToKeep: 4, \
	                PenaltyBreakBeforeFirstCallParameter: 19, \
	                PenaltyBreakComment: 60, \
	                PenaltyBreakString: 100, \
	                PenaltyBreakFirstLessLess: 120, \
	                PenaltyExcessCharacter: 1000000, \
	                PenaltyReturnTypeOnItsOwnLine: 60, \
	                PointerBindsToType: false, \
	                SpacesBeforeTrailingComments: 1, \
	                Cpp11BracedListStyle: true, \
	                Standard: Cpp03, \
	                IndentWidth: 4, \
	                TabWidth: 4, \
	                UseTab: Never, \
	                BreakBeforeBraces: Allman, \
	                IndentFunctionDeclarationAfterType: false, \
	                SpacesInParentheses: false, \
	                SpacesInAngles: false, \
	                SpaceInEmptyParentheses: false, \
	                SpacesInCStyleCastParentheses: false, \
	                SpaceAfterControlStatementKeyword: true, \
	                SpaceBeforeAssignmentOperators: true, \
	                ContinuationIndentWidth: 4}"

.PHONY: all
all: $(OBJ_REP) $(LIB_REP) $(LIB_TARGET_NAME).so

$(LIB_TARGET_NAME).so: $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(LIB_REP)/$@ $^

$(OBJ_REP)/%.o : $(SRC_REP)/%.c
	$(CC) $(CFLAGS) -o $@ -c $< -I $(INC_REP)

.PHONY: install
install:
	mkdir -p $(INC_DIR)
	cp $(INC_REP)/*.h $(INC_DIR)/
	cp $(LIB_REP)/$(LIB_TARGET_NAME).so $(LIB_DIR)/
	chmod 0755 $(LIB_DIR)/$(LIB_TARGET_NAME).so
	ldconfig

.PHONY: uninstall
uninstall:
	rm -rf $(INC_DIR)/
	rm $(LIB_DIR)/$(LIB_TARGET_NAME).so

.PHONY: graph
graph:
	cp $(INC_REP)/*.h $(SRC_REP)/*.c $(GRAPH_REP)/
	cd $(GRAPH_REP)/ ; ./$(GRAPH_NAME).pl --merge module > graph.dot
	dot -Tpdf $(GRAPH_REP)/graph.dot -o $(GRAPH_REP)/graph.pdf
	rm $(GRAPH_REP)/*.h $(GRAPH_REP)/*.c $(GRAPH_REP)/*.dot
	xdg-open $(GRAPH_REP)/graph.pdf

.PHONY: test
test: $(TEST_REP) $(BIN_TEST_REP) $(EXECUTABLES)

$(BIN_TEST_REP)/%: $(TEST_REP)/%.c
	$(CC) $(CFLAGS) -o $@ $< $(LIB_FLAG_NAME)

.PHONY: indent
indent:
	@clang-format -verbose -style=$(INDENT_STYLE) $(HEADERS) $(SOURCES) $(TESTS) > /dev/null

.PHONY: clean
clean:
	rm -f $(OBJ_REP)/*.o $(LIB_REP)/*.so $(GRAPH_REP)/graph.pdf
	rm -rf $(BIN_TEST_REP)

$(OBJ_REP):
	mkdir -p $(OBJ_REP)

$(TEST_REP):
	mkdir -p $(TEST_REP)

$(LIB_REP):
	mkdir -p $(LIB_REP)

$(BIN_TEST_REP):
	mkdir -p $(BIN_TEST_REP)
