vpath %.cpp src
vpath %.hpp include

CXX = g++
CXXFLAGS = -std=c++11
INCDIR = -Iinclude -Ilib
SRCS = $(wildcard src/*.cpp)
OBJS = $(patsubst %.cpp, obj/%.o, $(notdir $(SRCS)))
CONSHPP_DEPS = Cell.hpp IntCell.hpp DoubleCell.hpp \
 SymbolCell.hpp ConsCell.hpp ProcedureCell.hpp
CMD = $(CXX) $(CXXFLAGS) $(INCDIR) -c $< -o $@

main: $(OBJS) obj/main.o
	$(CXX) $(CXXFLAGS) -o $@ obj/*.o

$(OBJS): | obj

obj:
	@mkdir -p $@

obj/main.o: main.cpp Parser.hpp $(CONSHPP_DEPS)
	$(CMD)

obj/Parser.o:obj/%.o: %.cpp %.hpp $(CONSHPP_DEPS)
	$(CMD)

obj/SymbolCell.o: SymbolCell.cpp SymbolCell.hpp Cell.hpp Scope.hpp
	$(CMD)

obj/ProcedureCell.o: ProcedureCell.cpp ProcedureCell.hpp Cell.hpp ConsCell.hpp Scope.hpp
	$(CMD)

obj/Cell.o: Cell.cpp $(wildcard include/*.hpp)
	$(CMD)

obj/%.o: %.cpp %.hpp Cell.hpp
	$(CMD)

test_all: test_sim test_g test_ari

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

clean:
	rm -rf main obj/ testfiles/testoutput_*
