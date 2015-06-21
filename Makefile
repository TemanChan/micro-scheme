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
	rm -f testfiles/testoutput_simple.txt
	./main testfiles/testinput_simple.txt > testfiles/testoutput_simple.txt
	diff testfiles/testref_simple.txt testfiles/testoutput_simple.txt

test_g:
	rm -f testfiles/testoutput_general.txt
	./main testfiles/testinput_general.txt > testfiles/testoutput_general.txt
	diff testfiles/testref_general.txt testfiles/testoutput_general.txt

test_ari:
	rm -f testfiles/testoutput_arithmetic.txt
	./main testfiles/testinput_arithmetic.txt > testfiles/testoutput_arithmetic.txt
	diff testfiles/testref_arithmetic.txt testfiles/testoutput_arithmetic.txt

test_easy:
	rm -f testfiles/testoutput_easy.txt
	./main testfiles/testinput_easy.txt > testfiles/testoutput_easy.txt
	diff testfiles/testref_easy.txt testfiles/testoutput_easy.txt

test_gen:
	rm -f testfiles/testoutput_gen.txt
	./main testfiles/testinput_gen.txt > testfiles/testoutput_gen.txt
	diff testfiles/testref_gen.txt testfiles/testoutput_gen.txt

test_a4:
	rm -f testfiles/testoutput_a4.txt
	./main testfiles/testinput_a4.txt > testfiles/testoutput_a4.txt
	diff testfiles/testref_a4.txt testfiles/testoutput_a4.txt

clean:
	rm -f ${TARGET} ${OBJS} testfiles/testoutput*.txt

