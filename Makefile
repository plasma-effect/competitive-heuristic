.PHONY: all
all: main.o check.o

CXXFLAGS = -std=c++20 -O3 -Iboost/include -Iac-library -DLOCAL_DEBUG -Wall -Wextra -Werror=return-type

%.o: %.cpp
	g++ -MMD -MP $< $(CXXFLAGS) -o $@

.PHONY: greedy
greedy: codes/greedy.o

.PHONY: beam_search
beam_search: codes/beam_search.o

.PHONY: climbing
climbing: codes/climbing.o

.PHONY: annealing
annealing: codes/annealing.o

-include main.d

.PHONY: clean
clean:
	rm -f *.o *.d

.PHONY: clean_all
clean_all: clean
	rm -rf samples
	rm -rf tools
	rm -f *.zip *.tar.gz *.html
	rm best.txt
