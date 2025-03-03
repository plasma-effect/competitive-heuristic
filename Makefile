.PHONY: all
all: main.o

WARNINGS = -Wall -Wextra -Werror=return-type
INCLUDES = -Iboost/include -Iac-library
CXXFLAGS = -std=c++20 -O3 $(INCLUDES) $(WARNINGS)

%.o: %.cpp
	g++ -MMD -MP $< $(CXXFLAGS) -DLOCAL_DEBUG -std=c++23 -o $@

main_measure.o: main.cpp
	g++ -MMD -MP $< $(CXXFLAGS) -o $@

-include main.d

.PHONY: clean
clean:
	rm -f *.o *.d

.PHONY: clean_all
clean_all: clean
	rm -rf samples
	rm -rf tools
	rm -rf backup
	rm -f *.zip *.tar.gz *.html
	rm -f best.txt
	rm -f data.json
