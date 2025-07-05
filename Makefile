default: main

#directories and files
MODULES := mcsl app frozen

ALL_CODE_DIRS := $(foreach dir, $(MODULES),${shell find -L $(dir) -type d -print})
ALL_SRC_FILES := $(foreach dir, $(ALL_CODE_DIRS), $(wildcard $(dir)/*.cpp))
ALL_HEADER_FILES := $(foreach dir, $(ALL_CODE_DIRS), $(wildcard $(dir)/*.hpp))

ALL_TESTS := $(wildcard test/src/*.cpp)

ALL_AUTO_MAKEFILES := $(ALL_SRC_FILES:%.cpp=_build/%.mk) $(ALL_TESTS:%.cpp=_build/%.mk)
ALL_OBJ_FILES := $(ALL_SRC_FILES:%.cpp=_build/%.o)

#compiler
COMPILER := clang++ -std=c++23
FLAGS := -g -Wall -Wextra -pedantic -pedantic-errors -ftemplate-backtrace-limit=4 -fdiagnostics-show-category=name -Wno-gcc-compat -Wno-trigraphs -Wno-pessimizing-move -Wno-dtor-name -Wno-nested-anon-types -Wno-gnu-anonymous-struct
# FLAGS := -g -Wall -Wextra -pedantic -pedantic-errors -ftemplate-backtrace-limit=4 -fdiagnostics-show-category=name -Wno-gcc-compat -Wno-trigraphs -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls
# COMPILER := g++ -std=c++23
# FLAGS := -g -Wall -Wextra -pedantic -pedantic-errors -ftemplate-backtrace-limit=4 -Wno-trigraphs -Wno-attributes -Wno-parentheses -Wno-class-memaccess -Wno-pessimizing-move

#compile commands
COMPILE := $(COMPILER) $(FLAGS) $(addprefix -I, $(MODULES) $(filter %/header, $(ALL_CODE_DIRS)))


#clean
.PHONY: clean
clean:
	rm -rf _build

#set up directory structure
.PHONY: setup
setup:
	mkdir -p $(addprefix _build/,$(ALL_CODE_DIRS)) _build/test/src _build/out

#generate prereq makefiles
$(ALL_AUTO_MAKEFILES): _build/%.mk : %.cpp | setup
	(($(COMPILE) -MM $^) | (sed -E 's/([^ ]*)\.o([^u])?/_build\/\1.o _build\/\1.mk\2/') > $@) && echo $@

#include prereq makefiles
-include $(ALL_AUTO_MAKEFILES)

#include unit test makefiles
-include $(wildcard _build/test/*.mk)

#compile object files
$(ALL_OBJ_FILES): _build/%.o : %.cpp | _build/%.mk setup
	($(COMPILE) -c $^ -o $@) && echo $@


.PHONY: objects
objects: $(ALL_OBJ_FILES)

#compile unit test files
.PHONY: main
main: _build/app/main.mk app/main.cpp | $(ALL_OBJ_FILES) setup
	$(COMPILE) $(shell find _build -name *.o) -o _build/out/$@.out