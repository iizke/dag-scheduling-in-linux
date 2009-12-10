## ONLY USE FOR LINUX KERNEL VERSION 2.6
MODULE_NAME := phase_sched
BUILD_DIR := build
SOURCE_FILES := *.c *.h
MAKEFILE := Makefile.build

#all: $(BUILD_DIR)/*.c $(BUILD_DIR)/*.h
all:
	@mkdir -p $(BUILD_DIR); cp -f *.h *.c $(BUILD_DIR) 
	@cp -f $(MAKEFILE) $(BUILD_DIR)/Makefile
	@$(MAKE) -C $(BUILD_DIR) SRCPATH=$(PWD)

clean:
	@rm -rf $(BUILD_DIR)
	@rm -f Module.markers Module.symvers

$(BUILD_DIR)/%.c: %.c
	@mkdir -p $(BUILD_DIR)
	cp $< $(BUILD_DIR)

$(BUILD_DIR)/%.h: %.h
	@mkdir -p $(BUILD_DIR)
	cp $< $(BUILD_DIR)