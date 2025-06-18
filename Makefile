.PHONY: default
default: main_debug.o main_measure.o

WARNINGS = -Wall -Wextra -Werror=return-type
INCLUDES = -Iboost/include -Iac-library -Icompetitive-library/include -Ijson/include
CXXFLAGS = -std=c++23 $(INCLUDES) $(WARNINGS)

%.o: %.cpp
	g++-14 -MMD -MP -g -c $< $(CXXFLAGS) -DLOCAL_DEBUG -std=c++23 -o $@

main_debug.o: main.o
	g++-14 $^ -g -lstdc++exp -o $@

main_measure.o: main.cpp
	g++-12 -MMD -MP $< $(CXXFLAGS) -O3 -DLOCAL_MEASURE -o $@

main_raw.o: main.cpp
	g++-12 $< $(CXXFLAGS) -O3 -o $@

check_result.o: check.o
	g++-14 $^ -o $@

manager.o: score_manager.o
	g++-14 $^ -o $@

grid_slider/create.o: grid_slider/create_grid_data.o
	g++-14 $^ -o $@

experimental.o: experiment.o
	g++-14 $^ -o $@

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
reset: clean remove
	cmake -S competitive-library/ -B build/competitive-library
	cmake --build build/competitive-library --target expander
	./build/competitive-library/expander/expander main.tmp.cpp -o main.cpp

.PHONY: all_reset
all_reset: clean remove
	rm -f *.gch
