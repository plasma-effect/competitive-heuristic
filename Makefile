.PHONY: all
all: main.o

WARNINGS = -Wall -Wextra -Werror=return-type -Wno-unused-parameter -Wno-unused-variable
INCLUDES = -Iboost/include -Iac-library
CXXFLAGS = -std=c++20 -O3  -DLOCAL_DEBUG $(INCLUDES) $(WARNINGS)

%.o: %.cpp
	g++ -MMD -MP $< $(CXXFLAGS) -o $@

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
