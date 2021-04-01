SRC_DIR  = src
INT_DIR  = build
TEST_DIR = build/test
TARGET  = alma

CXX = g++
## -MMD creates dependency list, but ignores system includes ## -MF specifies where to create the dependency file name
## -MP creates phony targets for headers (deals with deleted headers after
##  obj file has been compiled)
## -MT specifies the dependency target (path qualified obj file name)
DEP_FLAGS = -std=c++11 -MT $@ -MMD -MP -MF $(@:.o=.d) -ggdb
CXXFLAGS = $(DEP_FLAGS) -Wall -Werror
LDFLAGS = -lstdc++
# conditional linker flags based on OS (Linux, Darwin = MacOS)
UNAME := $(shell uname)
ifeq ($(UNAME), Linux)

# use ncursesw6-config utility to find libraries when available
# therwhise go for a best guess
ifeq (, $(shell which ncursesw6-config))
LDFLAGS += -lncurses -lncursesw
else
LDFLAGS += $(shell ncursesw6-config --libs)
endif

endif
ifeq ($(UNAME), Darwin)
LDFLAGS += -lncurses
DEP_FLAGS += -D_XOPEN_SOURCE_EXTENDED
endif

# Things to build
ALL_FILES := $(wildcard $(SRC_DIR)/**/**/*.cpp) $(wildcard $(SRC_DIR)/**/*.cpp) $(wildcard $(SRC_DIR)/*.cpp)
CPP_FILES := $(filter-out %.test.cpp, $(ALL_FILES))
TEST_FILES := $(filter %.test.cpp, $(ALL_FILES))

ALL_OBJ_FILES := $(ALL_FILES:$(SRC_DIR)/%.cpp=$(INT_DIR)/%.o)
CPP_OBJ_FILES := $(CPP_FILES:$(SRC_DIR)/%.cpp=$(INT_DIR)/%.o)
TEST_OBJ_FILES := $(TEST_FILES:$(SRC_DIR)/%.cpp=$(INT_DIR)/%.o)
AUX_OBJ_FILES := $(filter-out build/main.o, $(CPP_OBJ_FILES))

DEP_FILES := $(ALL_FILES:$(SRC_DIR)/%.cpp=$(INT_DIR)/%.d)
TEST_TARGETS := $(TEST_OBJ_FILES:$(INT_DIR)/%.o=$(TEST_DIR)/%)

SUB_FOLDERS := $(filter-out src, $(patsubst src/%,%, $(shell find src -type d)))
OBJ_FOLDERS := $(addprefix build/, $(SUB_FOLDERS)) $(addprefix build/test/, $(SUB_FOLDERS))
.PHONY: test debug info clean format run $(TEST_TARGETS)

all: info $(TARGET)
test: info $(TEST_TARGETS)
debug: CXXFLAGS += -ggdb
debug: clean all test
static: LDFLAGS += -static -static-libstdc++ -static-libgcc
static: info all

info:
	@echo
	@echo "CXX = $(CXX)"
	@echo "CXXFLAGS = $(CXXFLAGS)"
	@echo "LDFLAGS = $(LDFLAGS)"
	@echo

clean:
	@echo "RMRF\tbuild $(TARGET)"
	@rm -rf build $(TARGET)

format:
	clang-format -i $(ALL_FILES)

run: all
	@echo "RUN\t$(TARGET)"
	@./$(TARGET)

$(TEST_TARGETS): $(TEST_DIR)/%: $(INT_DIR)/%.o | $(ALL_OBJ_FILES)
	@echo "LD\t$<"
	@$(CXX) $^ $(AUX_OBJ_FILES) $(LDFLAGS) -o $@
	@echo "RUN\t$@"
	@$@
	@echo "SUCCESS\t$@"

$(ALL_OBJ_FILES): $(INT_DIR)/%.o: $(SRC_DIR)/%.cpp $(INT_DIR)/%.d | $(OBJ_FOLDERS)
	@echo "CC\t$<"
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

$(TARGET): $(CPP_OBJ_FILES)
	@echo "LD\t$@"
	@$(CXX) $^ $(LDFLAGS) -o $@

$(DEP_FILES): $(INT_DIR)/%.d: ;

$(OBJ_FOLDERS):
	@echo "MKDIR\t$@"
	@mkdir -p $@

-include $(DEP_FILES)
