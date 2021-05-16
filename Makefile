# CC - compiler
# OBJ - compiled source files that should be linked
# COPT - compiler flags
# BIN - binary
CC=gcc
OBJ=cache.o dataStructure.o handler.o parser.o
COPT=-Wall -Wpedantic -g
BIN=dns_svr

# Running "make" with no argument will make the first target in the file
all: $(BIN)

# Rules of the form
#     target_to_be_made : dependencies_to_be_up-to-date_first
#     <tab>commands_to_make_target
# (Note that spaces will not work.)

$(BIN): main.c $(OBJ)
	$(CC) -o $(BIN) main.c $(OBJ) $(COPT)

# Wildcard rule to make any  .o  file,
# given a .c and .h file with the same leading filename component
%.o: %.c %.h
	$(CC) -c $< $(COPT) -g

format:
	clang-format -i *.c *.h

clean:
	rm -f dns_svr dns_svr.log $(OBJ)
