AUTOMAKE_OPTIONS = foreign no-dependencies

if BUILD_DEBUG_MODE
EXTRA_CPPFLAGS = -g3 -O0 -DDEBUG
else
EXTRA_CPPFLAGS = -g -O3 
endif
bin_PROGRAMS = toy_grep

toy_grep_SOURCES = \
				   main.cpp \
				   base.h \
				   dfa.hpp \
				   matcher.hpp \
				   debug.hpp \
				   stree.hpp

CXXFLAGS = -Wall -Werror $(EXTRA_CPPFLAGS)
