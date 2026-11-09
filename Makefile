
CXX        ?=   g++
STD   	   :=   -std=gnu++26 
MAKEFLAGS  :=   -j$(shell nproc)

OPT        :=   -O3 -march=native -fno-plt -pipe
LTO        :=   -flto=auto -fuse-linker-plugin
WARN 	   :=   -Wall -Weffc++ -Wextra -Wpedantic -Wconversion -Wshadow -Wsign-conversion\
                -Wmismatched-new-delete -Wdouble-promotion -Wfloat-equal
CXXFLAGS   :=   $(STD) $(WARN) $(OPT) $(LTO) -g
CPPFLAGS   :=   -MMD -MP

DEPPACKS   :=   sdl3
CPPFLAGS   +=   $(shell pkg-config --cflags $(DEPPACKS) 2>/dev/null)
LDLIBS     +=   $(shell pkg-config --libs   $(DEPPACKS) 2>/dev/null)

BUILD_DIR  :=   build/makefile
TARGET     :=   $(BUILD_DIR)/main
SRC        :=   $(wildcard src/*.cpp)
OBJ        :=   $(patsubst src/%,$(BUILD_DIR)/%,$(SRC:.cpp=.o))
DEP        :=   $(OBJ:.o=.d)

PGO_DIR    :=   build/makefile_pgo
PGO_TARGET :=   $(PGO_DIR)/main
PGO_GEN    :=   -fprofile-dir=$(PGO_DIR) -fprofile-generate
PGO_USE    :=   -fprofile-dir=$(PGO_DIR) -fprofile-use -fprofile-correction

.PHONY: clean all pgo run run_pgo

all: $(TARGET)

run:
	$(TARGET)


$(TARGET): $(OBJ) | $(BUILD_DIR)
	@echo "----#$@"
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BUILD_DIR)/%.o: src/%.cpp | $(BUILD_DIR)
	@echo "----#$@"
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@ 

$(BUILD_DIR):
	mkdir -p $@

pgo:
	@echo "----#Preparing for pgo."
	rm -rf $(PGO_DIR)
	mkdir -p $(PGO_DIR)
	@echo "----#Generating profiling executable."
	$(CXX) $(PGO_GEN) $(CXXFLAGS) $(LDFLAGS) $(SRC) $(LDLIBS) -o $(PGO_TARGET)
	@echo "----#Gathering data."
	@timeout --preserve-status -s INT 10s $(PGO_TARGET)
	@echo "----#Building pgo executable."
	$(CXX) $(PGO_USE) $(CXXFLAGS) $(LDFLAGS) $(SRC) $(LDLIBS) -o $(PGO_TARGET)
	@echo "----#Pgo executable built!"

run_pgo:
	$(PGO_TARGET)

test: t1 t2
t1:
	timeout 30s yes
t2:
	timeout 30s yes

compilecommands:
	$(MAKE) -nB | bear parse-sh

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(PGO_DIR)

-include $(DEP)
