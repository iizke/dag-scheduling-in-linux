all: module
MODULE_NAME := phase_sched_mod

ifeq ($(PATCHLEVEL),)
 PATCHLEVEL := $(shell uname -r | cut -d. -f2)
endif

RESMAN_CORE_OBJS := phase_sched_mod.o
$(MODULE_NAME)-objs := $(RESMAN_CORE_OBJS) 

ifdef TOPDIR
obj-m += $(MODULE_NAME).o
endif

EXTRA_CFLAGS += -I$(src)

ifdef KERNDIR
 KERNEL_SOURCES := $(KERNDIR)
else
 KERNEL_SOURCES := /lib/modules/$(shell uname -r)/build
endif

ifdef MODDIR
 MODULE_ROOT := $(MODDIR)
else
 MODULE_ROOT := /lib/modules/$(shell uname -r)/extra
endif

ifdef KERNOUT
	KERNEL_OUTPUT := KBUILD_OUTPUT=$(KERNOUT)
else
	KERNEL_OUTPUT :=
endif

src ?= .
obj ?= .

ifeq ($(PATCHLEVEL), 4)
 MODULE_OBJECT := $(MODULE_NAME).o
 MODULE_CONF := /etc/modules.conf
else
 MODULE_OBJECT := $(MODULE_NAME).ko
 MODULE_CONF := /etc/modprobe.conf
endif

ifeq ($(PATCHLEVEL), 4)
$(obj)/$(MODULE_NAME).o: $($(MODULE_NAME)-objs)
	$(LD) $(EXTRA_LDFLAGS) -r -o $@ $($(MODULE_NAME)-objs)
endif

KBUILD_PARAMS := -C $(KERNEL_SOURCES) SUBDIRS=$(PWD) $(KERNEL_OUTPUT)

module:
	@$(MAKE) $(KBUILD_PARAMS) modules; \
	if ! [ -f $(MODULE_OBJECT) ]; then \
	echo "$(MODULE_OBJECT) failed to build!"; \
	exit 1; \
	fi

clean:
	@rm -f $(RESMAN_CORE_OBJS) .*.{cmd,flags}
	@rm -f $(MODULE_NAME).{o,ko,mod.{o,c}} built-in.o $(VERSION_HEADER) *~
	@rm *.o
	@rm -fr .tmp_versions

modules_install:
ifeq ($(PATCHLEVEL),4)
	if ! [ -f $(MODULE_OBJECT) ]; then \
	module; \
	fi
	@echo "install '$(MODULE_OBJECT)' to $(MODULE_ROOT)"
	install -m 755 -o 0 -g 0 -d $(MODULE_ROOT)
	install -m 644 -o 0 -g 0 $(MODULE_OBJECT) $(MODULE_ROOT)
	/sbin/depmod -a
else
	echo "2.6 module install"
	make $(KBUILD_PARAMS) modules_install
	/sbin/depmod -a
endif

ifeq ($(PATCHLEVEL), 4)
include $(KERNEL_SOURCES)/Rules.make
endif
