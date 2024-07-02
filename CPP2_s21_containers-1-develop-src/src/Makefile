CXX = g++
CFLAGS = -Wall -Wextra -Werror -lstdc++ -std=c++17 -g
V_COMMAND = valgrind  --tool=memcheck --track-fds=yes --trace-children=yes --track-origins=yes --leak-check=full --show-leak-kinds=all -s
L_COMMAND = leaks -atExit --

PATH_TO_OBJ = objects/
PATH_TO_TESTS = tests/
COV_REPORT = report/
EXEC_T = tests.cc

SRC_T = $(wildcard $(PATH_TO_TESTS)*.cpp)
OBJ_T = $(patsubst %.cpp, $(PATH_TO_OBJ)%.o, $(notdir $(SRC_T)))

OS := $(shell uname -s)

ifeq ($(OS), Darwin)
	CFLAGS+=$(shell pkg-config --cflags check)
	LIBS+=$(shell pkg-config --libs check)
endif

all: test

test: create_obj_dir coverage_flag $(OBJ_T)
ifeq ($(OS), Darwin)
	$(CXX) $(CFLAGS) $(OBJ_T) $(LIBS) -o $(PATH_TO_TESTS)$(EXEC_T)
	$(PATH_TO_TESTS)./$(EXEC_T) || true
else
	$(CXX) $(CFLAGS) $(OBJ_T) -o $(PATH_TO_TESTS)$(EXEC_T) -lgtest -lm
	$(PATH_TO_TESTS)./$(EXEC_T) || true
endif

create_obj_dir:
	mkdir -p $(PATH_TO_OBJ)

$(PATH_TO_OBJ)%.o: $(PATH_TO_TESTS)%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

format-check:
	@cp ../materials/linters/.clang-format ./
	@find . -name '*.cpp' -o -name '*.tpp' -o -name '*.h' | xargs clang-format -n
	@rm .clang-format

format:
	@cp ../materials/linters/.clang-format ./
	@find . -name '*.cpp' -o -name '*.tpp' -o -name '*.h' | xargs clang-format -i
	@rm .clang-format

cppcheck:
	@find . -name '*.cpp' -o -name '*.tpp' -o -name '*.h' | xargs cppcheck --enable=all --suppress=missingIncludeSystem --language=c++ --std=c++17

valgrind:
	@if [ -f "$(PATH_TO_TESTS)$(EXEC_T)" ]; then \
        $(V_COMMAND) ./$(PATH_TO_TESTS)$(EXEC_T); \
    else \
        echo "Test executable not found."; \
    fi

leaks:
	@if [ -f "$(PATH_TO_TESTS)$(EXEC_T)" ]; then \
        $(L_COMMAND) ./$(PATH_TO_TESTS)$(EXEC_T); \
    else \
        echo "Test executable not found."; \
    fi

coverage_flag:
	$(eval CFLAGS += --coverage)

gcov_report: coverage_flag test
	rm -rf $(PATH_TO_OBJ)$(PATH_TO_TESTS)*.gcno
	rm -rf $(PATH_TO_OBJ)$(PATH_TO_TESTS)*.gcda
	gcov -b -l -p -c $(OBJ_T)
	gcovr -o $(COV_REPORT)gcov_report.html --html --html-details
	rm -rf *.info && rm -rf *.gcov
	@if command -v xdg-open > /dev/null; then \
        xdg-open $(COV_REPORT)gcov_report.html; \
    elif command -v open > /dev/null; then \
        open $(COV_REPORT)gcov_report.html; \
    else \
        echo "Cannot open browser. Please open '$(COV_REPORT)gcov_report.html' manually."; \
    fi

clean:
	find $(PATH_TO_OBJ) -name '*.o' -exec rm {} +
	find $(PATH_TO_OBJ) -name '*.gcno' -exec rm {} +
	find $(PATH_TO_OBJ) -name '*.gcda' -exec rm {} +
	rm -rf $(PATH_TO_TESTS)$(EXEC_T)
	rm -rf $(COV_REPORT)*.css && rm -rf $(COV_REPORT)*.html
	rm -rf *.info && rm -rf *.gcov
	rm -rf RESULT_VALGRIND.txt gcov_*

.PHONY: all cppcheck format format-check test valgrind leaks clean gcov_report
