CC = g++
FLAGS = -std=c++11
TARGET = main
OBJS = Cell.o ConsCell.o DoubleCell.o eval.o IntCell.o main.o parse.o SymbolCell.o \
		ProcedureCell.o PrimitiveProcedureCell.o

${TARGET}: ${OBJS}
	${CC} ${FLAGS} -o ${TARGET} $^

Cell.o: Cell.hpp
	${CC} ${FLAGS} -c Cell.cpp

ConsCell.o: ConsCell.cpp Cell.hpp
	${CC} ${FLAGS} -c ConsCell.cpp

DoubleCell.o: DoubleCell.cpp Cell.hpp
	${CC} ${FLAGS} -c DoubleCell.cpp

eval.o: eval.cpp eval.hpp cons.hpp Cell.hpp
	${CC} ${FLAGS} -c eval.cpp

IntCell.o: IntCell.cpp Cell.hpp
	${CC} ${FLAGS} -c IntCell.cpp

main.o: main.cpp parse.hpp cons.hpp Cell.hpp eval.hpp
	${CC} ${FLAGS} -c main.cpp

parse.o: parse.cpp parse.hpp cons.hpp Cell.hpp
	${CC} ${FLAGS} -c parse.cpp

SymbolCell.o: SymbolCell.cpp Cell.hpp
	${CC} ${FLAGS} -c SymbolCell.cpp

ProcedureCell.o: ProcedureCell.cpp Cell.hpp
	${CC} ${FLAGS} -c ProcedureCell.cpp

PrimitiveProcedureCell.o: PrimitiveProcedureCell.cpp Cell.hpp
	${CC} ${FLAGS} -c PrimitiveProcedureCell.cpp

doc:
	doxygen doxygen.config

test_all: test_sim test_g test_gen test_ari test_easy test_a4

test_sim:
	rm -f testoutput_simple.txt
	./main testinput_simple.txt > testoutput_simple.txt
	diff testref_simple.txt testoutput_simple.txt

test_g:
	rm -f testoutput_general.txt
	./main testinput_general.txt > testoutput_general.txt
	diff testref_general.txt testoutput_general.txt

test_ari:
	rm -f testoutput_arithmetic.txt
	./main testinput_arithmetic.txt > testoutput_arithmetic.txt
	diff testref_arithmetic.txt testoutput_arithmetic.txt

test_easy:
	rm -f testoutput_easy.txt
	./main testinput_easy.txt > testoutput_easy.txt
	diff testref_easy.txt testoutput_easy.txt

test_gen:
	rm -f testoutput_gen.txt
	./main testinput_gen.txt > testoutput_gen.txt
	diff testref_gen.txt testoutput_gen.txt

test_a4:
	rm -f testoutput_a4.txt
	./main testinput_a4.txt > testoutput_a4.txt
	diff testref_a4.txt testoutput_a4.txt

clean:
	rm -f ${TARGET} ${OBJS} testoutput*.txt

