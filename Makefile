
CXX        ?=   g++
STD   	   :=   -std=gnu23 
MAKEFLAGS  :=   -j$(shell nproc)

OPT        :=   -O3 -march=native -fno-plt -pipe
LTO        :=   -flto=auto -fuse-linker-plugin
WARN 	   :=   -Wall -Wextra -Wpedantic -Wconversion -Wshadow 
CXXFLAGS   :=   $(WARN) $(OPT) $(LTO) 
CPPFLAGS   :=   -MMD -MP

DEPPACKS   :=   sdl3
CPPFLAGS   +=   $(shell pkg-config --cflags $(DEPPACKS) 2>/dev/null)
LDLIBS     +=   $(shell pkg-config --libs   $(DEPPACKS) 2>/dev/null)

BUILD_DIR  :=   build/makefile
TARGET     :=   $(BUILD_DIR)/main
SRC        :=   $(wildcard src/*.cpp)
OBJ        :=   $(patsubst src/%,$(BUILD_DIR)/%,$(SRC:.cpp=.o))
DEP        :=   $(OBJ:.o=.d)

PGO_DIR    :=   $(BUILD_DIR)/pgo
PGO_GEN    :=   -fprofile-dir=$(PGO_DIR) -fprofile-generate
PGO_USE    :=   -fprofile-dir=$(PGO_DIR) -fprofile-use -fprofile-correction

.PHONY: clean all pgo run

all: $(TARGET)

pgo:
	@echo "----#Preparing for pgo."
	$(MAKE) clean
	$(MAKE) $(BUILD_DIR)/pgo
	@echo "----#Generating profiling executable."
	$(MAKE) all CXXFLAGS="$(CXXFLAGS) $(PGO_GEN)"
	@echo "----#Generated profiling executable."
	@echo "----#Gathering data."
	@timeout --preserve-status -s INT 10s $(TARGET)
	@echo "----#Gathered data."
	@echo "----#Building pgo executable."
	$(MAKE) all CXXFLAGS="$(CXXFLAGS) $(PGO_USE)"
	@echo "----#Pgo executable built!"

run:
	$(TARGET)

$(TARGET): $(OBJ) | $(BUILD_DIR)
	@echo "----#$@"
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BUILD_DIR)/%.o: src/%.cpp | $(BUILD_DIR)
	@echo "----#$@"
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@ 

$(BUILD_DIR) $(BUILD_DIR)/pgo:
	mkdir -p $@

test: t1 t2
t1:
	timeout 30s yes
t2:
	timeout 30s yes

clean:
	rm -rf $(BUILD_DIR)

-include $(DEP)
