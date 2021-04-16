CC = g++
COMP_FLAG = -std=c++11
PART_A_SOURCE = part_a.cpp tokens.hpp lex.yy.c
PART_B_SOURCE = part_b.cpp tokens.hpp lex.yy.c
PART_A_OBJS = part_a.o lex.yy.o
PART_B_OBJS = part_b.o lex.yy.o
PART_A_EXEC = part_a.out
PART_B_EXEC = part_b.out
OBJS = ${PART_A_OBJS} ${PART_B_OBJS}
EXECS = ${PART_A_EXEC} ${PART_B_EXEC}

all: part_a.out part_b.out

part_a.out: ${PART_A_OBJS}
	${CC} ${COMP_FLAG} ${PART_A_OBJS} -o $@

part_b.out: ${PART_B_OBJS}
	${CC} ${COMP_FLAG} ${PART_B_OBJS} -o $@

part_a.o: part_a.cpp lex.yy.o tokens.hpp
	${CC} ${COMP_FLAG} -c ${PART_A_SOURCE}

part_b.o: part_b.cpp lex.yy.o tokens.hpp
	${CC} ${COMP_FLAG} -c ${PART_B_SOURCE}

lex.yy.o: lex.yy.c
	${CC} ${COMP_FLAG} -c lex.yy.c

lex.yy.c: scanner.lex tokens.hpp
	flex scanner.lex

clean:
	rm -f ${OBJS} ${EXECS} lex.yy.c tokens.hpp.gch