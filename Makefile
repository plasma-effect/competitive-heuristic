.PHONY: all
all: main.o

CXXFLAGS = -std=c++20 -O3 -Iboost/include -Iac-library -DLOCAL_DEBUG -Wall -Wextra

%.o: %.cpp
	g++ -MMD -MP $< $(CXXFLAGS) -o $@

-include main.d

.PHONY: clean
clean:
	rm -f *.o *.d
