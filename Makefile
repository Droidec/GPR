# Compiler
CC=gcc
# Compiler flags
CFLAGS=-Wall -Wextra -Werror -O3
# Linker flags
LDFLAGS=-shared
# Header repository
INC_REP=include/
# Source repository
SRC_REP=src/
# Object repository
OBJ_REP=obj/
# Library repository
LIB_REP=lib/
# Graphic repository
GRAPH_REP=extras/graph/
# Library targeted name
LIB_NAME=libGPR.so
# Library installation directory
LIB_DIR=/usr/lib/
# Header installation directory
INC_DIR=/usr/include/GPR/
# Concatenated list of source files
SRCS=$(shell ls $(SRC_REP))
# Concatenared list of object files
OBJS=$(OBJ_REP)$(SRCS:.c=.o)

$(LIB_NAME): $(OBJS)
	$(CC) $(LDFLAGS) -o $(LIB_REP)$@ $^

$(OBJ_REP)%.o : $(SRC_REP)%.c
	$(CC) $(CFLAGS) -o $@ -c $< -I $(INC_REP)

.PHONY: install
install:
	mkdir -p $(INC_DIR)
	cp $(INC_REP)*.h $(INC_DIR)
	cp $(LIB_REP)$(LIB_NAME) $(LIB_DIR)
	chmod 0755 $(LIB_DIR)$(LIB_NAME)
	ldconfig

.PHONY: graph
graph:
	cp $(INC_REP)*.h $(SRC_REP)*.c $(GRAPH_REP)
	cd $(GRAPH_REP) ; ./cinclude2dot.pl --merge module > graph.dot
	dot -Tps $(GRAPH_REP)graph.dot -o $(GRAPH_REP)graph.pdf
	rm $(GRAPH_REP)*.h $(GRAPH_REP)*.c $(GRAPH_REP)*.dot
	xdg-open $(GRAPH_REP)graph.pdf

.PHONY: uninstall
uninstall:
	rm -rf $(INC_DIR)
	rm $(LIB_DIR)$(LIB_NAME)

.PHONY: clean
clean:
	rm $(OBJ_REP)*.o $(LIB_REP)*.so
