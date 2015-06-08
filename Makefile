CC = g++
FLAGS = -std=c++11
TARGET = main
OBJS = ConsCell.o DoubleCell.o eval.o IntCell.o main.o parse.o SymbolCell.o

${TARGET}: ${OBJS}
	${CC} ${FLAGS} -o ${TARGET} $^

ConsCell.o: ConsCell.cpp ConsCell.hpp Cell.hpp
	${CC} ${FLAGS} -c ConsCell.cpp

DoubleCell.o: DoubleCell.cpp DoubleCell.hpp Cell.hpp
	${CC} ${FLAGS} -c DoubleCell.cpp

eval.o: eval.cpp eval.hpp cons.hpp Cell.hpp IntCell.hpp \
 DoubleCell.hpp SymbolCell.hpp ConsCell.hpp
	${CC} ${FLAGS} -c eval.cpp

IntCell.o: IntCell.cpp IntCell.hpp Cell.hpp
	${CC} ${FLAGS} -c IntCell.cpp

main.o: main.cpp parse.hpp cons.hpp Cell.hpp IntCell.hpp \
 DoubleCell.hpp SymbolCell.hpp ConsCell.hpp eval.hpp
	${CC} ${FLAGS} -c main.cpp

parse.o: parse.cpp parse.hpp cons.hpp Cell.hpp IntCell.hpp \
 DoubleCell.hpp SymbolCell.hpp ConsCell.hpp
	${CC} ${FLAGS} -c parse.cpp

SymbolCell.o: SymbolCell.cpp SymbolCell.hpp Cell.hpp
	${CC} ${FLAGS} -c SymbolCell.cpp

doc:
	doxygen doxygen.config

test_sim:
	rm -f testoutput_simple.txt
	./main testinput_simple.txt > testoutput_simple.txt
	diff testreference_simple.txt testoutput_simple.txt

test_gen:
	rm -f testoutput_general.txt
	./main testinput_general.txt > testoutput_general.txt
	diff testreference_general.txt testoutput_general.txt

test_ari:
	rm -f testoutput_arithmetic.txt
	./main testinput_arithmetic.txt > testoutput_arithmetic.txt
	diff testreference_arithmetic.txt testoutput_arithmetic.txt

clean:
	rm -f ${TARGET} ${OBJS} testoutput*.txt

