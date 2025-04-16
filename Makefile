.PHONY: default
default: main_debug.o main_measure.o

WARNINGS = -Wall -Wextra -Werror=return-type
INCLUDES = -Iboost/include -Iac-library -Icompetitive-library/include
CXXFLAGS = -std=c++20 $(INCLUDES) $(WARNINGS)

%.o: %.cpp
	g++ -MMD -MP -c $< $(CXXFLAGS) -DLOCAL_DEBUG -std=c++23 -o $@

main_debug.o: main.o
	g++ $^ -lstdc++_libbacktrace -o $@

main_measure.o: main.cpp
	g++ -MMD -MP $< $(CXXFLAGS) -O3 -DLOCAL_MEASURE -o $@

main_raw.o: main.cpp
	g++ $< $(CXXFLAGS) -O3 -o $@

check_result.o: check.o
	g++ $^ -o $@

grid_slider/create.o: grid_slider/create_grid_data.o
	g++ $^ -o $@

local_headers_debug.hpp.gch: local_headers_debug.hpp
	g++ -x c++-header $< $(CXXFLAGS) -DLOCAL_DEBUG -std=c++23 -o $@

local_headers_measure.hpp.gch: local_headers_measure.hpp
	g++ -x c++-header $< $(CXXFLAGS) -DLOCAL_MEASURE -o $@

-include main.d

.PHONY: clean
clean:
	rm -f *.o *.d
	rm -f grid_slider/*.o grid_slider/*.d

.PHONY: remove
remove:
	rm -rf samples
	rm -rf samples_out
	rm -rf tools
	rm -rf backup
	rm -rf tmp
	rm -f *.zip *.tar.gz
	rm -f vis.html
	rm -f best.txt
	rm -f parallel.log

.PHONY: reset
reset: clean remove local_headers_debug.hpp.gch local_headers_measure.hpp.gch
	bazel run @competitive_library//expander -- $(PWD)/main.tmp.cpp -o $(PWD)/main.cpp

.PHONY: all_reset
all_reset: clean remove
	rm -f *.gch
