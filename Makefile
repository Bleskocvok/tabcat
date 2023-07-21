CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -march=native -DNDEBUG

TARGET = tabcat
SRC = src/tabcat.cpp
OBJ = $(SRC:.cpp=.o)

DEPEND = $(OBJ:.o=.d)


all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $^

%.o: src/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

%.o: CXXFLAGS += -MMD -MP

-include $(DEPEND)


clean:
	$(RM) $(OBJ) $(DEPEND)

distclean: clean
	$(RM) $(TARGET)

.PHONY: all clean distclean
