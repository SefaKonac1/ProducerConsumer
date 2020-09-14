CXX      := -c++
CXXFLAGS := -errors -Wall -pthread
LDFLAGS  := -L/usr/lib -std=c++11 -lm
BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
TARGET   := WorkProject
SRC      :=                      \
   $(wildcard src/*.cpp) 		

OBJECTS  := $(SRC:%.cpp=$(OBJ_DIR)/%.o)

all: build $(TARGET)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@ $(LDFLAGS)

$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $^ $(LDFLAGS)

.PHONY: all build clean debug release

build:
	@mkdir -p $(OBJ_DIR)

debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O2
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*. TARGET
