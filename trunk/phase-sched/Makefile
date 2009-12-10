## ONLY USE FOR LINUX KERNEL VERSION 2.6

MODULE_NAME := phase_sched
BUILD_DIR := build

#C_FILES := $(BUILD_DIR)/dag.c $(BUILD_DIR)/phase_task.c $(BUILD_DIR)/phase_cpu.c $(BUILD_DIR)/phase_sched_mod.c
#H_FILES := $(BUILD_DIR)/dag.h $(BUILD_DIR)/phase_task.h $(BUILD_DIR)/phase_cpu.h $(BUILD_DIR)/phase_sched_mod.h $(BUILD_DIR)/phase_def.h
#BUILD_SRCFILES:= $(C_FILES) $(H_FILES)

MAKEFILE := Makefile.build
SRCFILES=$(shell ls *.h *.c )
BUILD_SRCFILES=$(foreach fi, $(SRCFILES), $(shell echo $(BUILD_DIR)/$(fi)))

all: $(BUILD_SRCFILES)
	@cp -f $(MAKEFILE) $(BUILD_DIR)/Makefile
	@$(MAKE) -C $(BUILD_DIR) SRCPATH=$(PWD)/$(BUILD_DIR)

clean:
	@rm -rf $(BUILD_DIR)
	@rm -f Module.markers Module.symvers

$(BUILD_DIR)/%.c: %.c
	@mkdir -p $(BUILD_DIR)
	cp $< $(BUILD_DIR)

$(BUILD_DIR)/%.h: %.h
	@mkdir -p $(BUILD_DIR)
	cp $< $(BUILD_DIR)
