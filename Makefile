vpath %.cpp src:lib
vpath %.hpp include:lib

CXX = g++
CXXFLAGS = -std=c++11
INCDIR = -Iinclude -Ilib
SRCS = $(wildcard src/*.cpp lib/*.cpp)
OBJS = $(patsubst %.cpp, obj/%.o, $(notdir $(SRCS)))
CONSHPP_DEPS = cons.hpp Cell.hpp IntCell.hpp DoubleCell.hpp \
 SymbolCell.hpp ConsCell.hpp ProcedureCell.hpp
CMD = $(CXX) $(CXXFLAGS) $(INCDIR) -c $< -o $@

main: $(OBJS) obj/main.o
	$(CXX) $(CXXFLAGS) -o $@ obj/*.o

$(OBJS): | obj

obj:
	@mkdir -p $@

obj/main.o: main.cpp parse.hpp eval.hpp $(CONSHPP_DEPS)
	$(CMD)

obj/parse.o: parse.cpp parse.hpp $(CONSHPP_DEPS)
	$(CMD)

obj/eval.o: eval.cpp eval.hpp $(CONSHPP_DEPS)
	$(CMD)

obj/SymbolCell.o: SymbolCell.cpp SymbolCell.hpp Cell.hpp Scope.hpp
	$(CMD)

obj/ProcedureCell.o: ProcedureCell.cpp ProcedureCell.hpp Cell.hpp ConsCell.hpp Scope.hpp
	$(CMD)

obj/PrimitiveProcedureCell.o: PrimitiveProcedureCell.cpp PrimitiveProcedureCell.hpp \
 Cell.hpp IntCell.hpp DoubleCell.hpp SymbolCell.hpp ConsCell.hpp ProcedureCell.hpp Scope.hpp
	$(CMD)

obj/%.o: %.cpp %.hpp Cell.hpp
	$(CMD)

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
	rm -rf main obj/
