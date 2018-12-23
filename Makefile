CC := g++
LD := g++

SRC := src
BUILD_BASE := build
BUILD := build
BIN := bin
TARGET := bin/float_128



SOURCES := $(shell find $(SRC) -type f -name "*.cpp")
OBJS := $(SOURCES:$(SRC)/%.cpp=build/%.o)

FLAGS := -Wall -std=c++11

all: $(TARGET)
	@true

$(TARGET): $(OBJS)
	@mkdir -p bin
	@mkdir -p $(@D)
	@$(LD) $+ $(FLAGS) -o $@
	@echo "Building $@"

$(BUILD)/%.o: $(SRC)/%.cpp 
	@mkdir -p build
	@mkdir -p $(BUILD)
	@mkdir -p $(@D)
	$(CC) $(FLAGS) $< -c -o $@
	@echo "Building $@"


clean:
	@rm -f $(OBJS)  
	@rm -f $(TARGET) 
	-@find $(BIN) -type d -empty -delete 2> /dev/null || true
	-@find $(BUILD_BASE) -type d -empty -delete 2> /dev/null || true

.PHONY: all 
.SECONDARY: $(OBJS) 
