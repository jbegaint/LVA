# LVA global makefile

include config.mk

SRC := $(shell find $(LVA_DIR) -name '*.c')
OBJC := $(SRC:%.c=%.o)
OBJC_DBG := $(SRC:%.c=%.dbg.o)

SRCXX := $(shell find $(LVA_DIR) -name '*.cpp')
OBJXX := $(SRCXX:%.cpp=%.o)
OBJXX_DBG := $(SRCXX:%.cpp=%.dbg.o)

OBJ = $(OBJC) $(OBJXX)

all: demos

demos: demo_cycle demo_one_row demo_matrix demo_pattern_7x5

options:
	@echo build options:
	@echo "CC = ${CC}"
	@echo "CCX = ${CCX}"
	@echo "CFLAGS = ${CFLAGS}"
	@echo "CFLAGS_GTK = ${CFLAGS_GTK}"
	@echo "LDFLAGS = ${LDFLAGS}"
	@echo "LDFLAGS_GTK = ${LDFLAGS_GTK}"

# GPIOs demos
demo_pattern_7x5: $(DEMO_DIR)/demo_pattern_7x5.o $(OBJ)
	$(CC) $^ $(LDFLAGS) -o $(BUILD_DIR)/$@ 

demo_matrix: $(DEMO_DIR)/demo_matrix.o $(OBJ)
	$(CC) $^ $(LDFLAGS) -o $(BUILD_DIR)/$@ 

demo_one_row: $(DEMO_DIR)/demo_one_row.o $(OBJ)
	$(CC) $^ $(LDFLAGS) -o $(BUILD_DIR)/$@ 

demo_cycle: $(DEMO_DIR)/demo_cycle.o $(OBJ)
	$(CC) $^ $(LDFLAGS) -o $(BUILD_DIR)/$@ 

# Simulator
gleds: $(OBJ) GLeds/gleds.o
	@mkdir -p $(BUILD_DIR)
	@$(CC) $^ $(LDFLAGS) $(LDFLAGS_GTK) -o $(BUILD_DIR)/$@ 

%.o: %.c
	@$(CC) $< $(CFLAGS_GTK) -c -o $@

%.o: %.cpp
	@$(CXX) $< $(CFLAGS) -c -o $@ 

doc:
	@doxygen

clean:
	@rm -rf $(shell find . -name '*.o')
	@rm -rf $(BUILD_DIR)
	@rm -rf doxygen

.PHONY: all options clean doc
