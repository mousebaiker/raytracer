CC=g++
CFLAGS=-I.
OBJDIR=obj

GTEST_DIR = ./googletest/googletest
GTEST_CPPFLAGS = -isystem $(GTEST_DIR)/include
GTEST_CXXFLAGS = -g -Wall -Wextra -pthread
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

# For simplicity and to avoid depending on Google Test's
# implementation details, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Test
# compiles fast and for ordinary users its source rarely changes.

$(OBJDIR)/gtest-all.o : $(GTEST_SRCS_)
	mkdir -p $(OBJDIR)
	$(CXX) $(GTEST_CPPFLAGS) -I$(GTEST_DIR) $(GTEST_CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc -o $@


$(OBJDIR)/gtest_main.o : $(GTEST_SRCS_)
	mkdir -p $(OBJDIR)
	$(CXX) $(GTEST_CPPFLAGS) -I$(GTEST_DIR) $(GTEST_CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc -o $@


$(OBJDIR)/gtest.a : $(addprefix $(OBJDIR)/, gtest-all.o)
	$(AR) $(ARFLAGS) $@  $^


$(OBJDIR)/gtest_main.a : $(addprefix $(OBJDIR)/, gtest-all.o gtest_main.o)
	$(AR) $(ARFLAGS) $@  $^

$(OBJDIR)/%_test.o : %_test.cc $(GTEST_HEADERS)
	mkdir -p $(addprefix $(<D)/, $(OBJDIR))
	$(CC) $(CFLAGS) $(GTEST_CPPFLAGS) $(GTEST_CXXFLAGS) -c $< -o $@

linal_test : $(addprefix $(OBJDIR)/, gtest_main.a linal_test.o)
	$(CC) $(GTEST_CPPFLAGS) $(GTEST_CXXFLAGS) $^ -o $@


.PHONY : clean

clean :
	rm -f *.o
	rm -f ${OBJDIR}/*
	rm -rf ${OBJDIR}
