all: SRE_console
clean:
	$(RM) -rf $(BUILDDIR)/SRE_console
	

# Path 
############################################################################

VPATH += .

BUILDDIR ?= build
DMAPI ?= dip
CC = gcc
SRE_DEPS_DIR := libs/linux

ifdef FREJA
	POKY=/opt/poky/2.0.2
	export PATH:=$(PATH):$(POKY)/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi
	CROSS_COMPILER=arm-poky-linux-gnueabi-
	CPPFLAGS+=-fdiagnostics-color=auto
	CPPFLAGS+=-march=armv7-a -mfloat-abi=hard -mfpu=neon -mtune=cortex-a9 --sysroot=$(POKY)/sysroots/cortexa9hf-vfp-neon-poky-linux-gnueabi
	CPPFLAGS+=-DFREJA
	export CPPFLAGS
	export CC=$(CROSS_COMPILER)gcc 
	export CXX=$(CROSS_COMPILER)g++
	export BUILDDIR=FREJA
	SRE_DEPS_DIR := libs/arm
endif

EXAMPLE_DIR_NAME := ./SRE_Console

ifneq ($(DMAPI) , mock)
ifneq ($(DMAPI) , building)
ifneq ($(DMAPI) , dip)
$(error Unknown value for DMAPI=$(DMAPI), only "mock" and "building" are supported)
endif
endif
endif

SRE_ENABLE_ONTOLOGY         = ON
SRE_ENABLE_SECURED_MODE     = OFF
SRE_ENABLE_CLOUD_INTERFACE  = OFF
SRE_ENABLE_LOG_TO_CLOUD     = OFF
SRE_ENABLE_LOG              = ON
SRE_BUILD_UNIT_TESTS        = OFF
include SRE/SRE_Makefile.mk

ifdef FREJA
SRE_CFLAGS += \
	-I$(EXAMPLE_DIR_NAME)/ecp_ual/include \
	-I$(EXAMPLE_DIR_NAME)/ecp_ccsk \
	-I$(EXAMPLE_DIR_NAME)/safeclib/include \
	-I$(EXAMPLE_DIR_NAME)/libcurl/include/curl

SRE_LDLIBS += \
	$(EXAMPLE_DIR_NAME)/ecp_ble_lab/libBle.a \
	$(EXAMPLE_DIR_NAME)/iothubAgent/lib/libiothubAgent.a \
	$(EXAMPLE_DIR_NAME)/iothubAgent/lib/libiothub_client.a \
	$(EXAMPLE_DIR_NAME)/iothubAgent/lib/libiothub_client_amqp_transport.a \
	$(EXAMPLE_DIR_NAME)/iothubAgent/lib/libuamqp.a \
	$(EXAMPLE_DIR_NAME)/iothubAgent/lib/libaziotsharedutil.a \
	$(EXAMPLE_DIR_NAME)/iothubAgent/lib/libwebsockets.a \
	$(EXAMPLE_DIR_NAME)/libcurl/lib/libcurl.a
	
endif

# Includes and Sources
############################################################################

SRE_DMAPI_DIR := $(EXAMPLE_DIR_NAME)/dmapi/$(DMAPI)
DMAPI_SOURCES := $(shell find $(SRE_DMAPI_DIR)/src -name '*.c') $(shell find $(SRE_DMAPI_DIR)/src -name '*.cpp')
DMAPI_OBJS := $(addprefix $(BUILDDIR)/,$(patsubst %.c,%.o,$(patsubst %.cpp,%.o, $(DMAPI_SOURCES))))

ifeq ($(SRE_ENABLE_CLOUD_INTERFACE) , ON)
SRE_CLOUD_INTERFACE_DIR     := $(SRE_DIR)/abstractLayer/cloud/src/DSP_Agent
SRE_CLOUD_INTERFACE_SOURCES := $(shell find $(SRE_CLOUD_INTERFACE_DIR)/src -name '*.c')
SRE_CLOUD_INTERFACE_OBJS := $(addprefix $(BUILDDIR)/,$(SRE_CLOUD_INTERFACE_SOURCES:%.c=%.o))
endif

CPPFLAGS += \
	-g -Wall -fPIC -fprofile-arcs -ftest-coverage \
   -DEXAMPLE_DIR_NAME=\"$(EXAMPLE_DIR_NAME)\" \
	$(SRE_CFLAGS) \
	-I$(SRE_DMAPI_DIR)/include \
	

SRE_OBJS :=  $(addprefix $(BUILDDIR)/,$(SRE_SOURCES:%.c=%.o))

APP_OBJS += \
	 $(BUILDDIR)/$(EXAMPLE_DIR_NAME)/SRE_Console.o

ENGINE_LIB+=  ${SRE_OBJS} ${DMAPI_OBJS}
OBJS= $(APP_OBJS) $(ENGINE_LIB)


# dirs
############################################################################
DIRS = $(sort $(dir $(OBJS)))

dirs: $(DIRS)
$(DIRS):
	@mkdir -p $@


LDLIBS += -lrt -lpthread -ldl $(SRE_LDLIBS) -lmodbus \
	-lcurl -lssl -lcrypto -lpthread -lz -lgcov 


#CPPFLAGS += -fprofile-arcs -ftest-coverage
CFLAGS   += -std=gnu99 -g -fPIC

# Dependency tracking
############################################################################
DEPS = $(OBJS:.o=.d)
CFLAGS +=-MMD -MP
-include $(DEPS)

# VERBOSE and COLOR
############################################################################
ifeq ($(VERBOSE),1)
    ECHO=
else
    ECHO=@
endif

ifeq ($(COLOR),1)
	PRINTF=@printf '[\e[7m%s\e[0m] %s\n'
	CFLAGS+=-fdiagnostics-color=auto
else
	PRINTF=@printf '[%s] %s\n'
endif


# Rewrite the built in rules
############################################################################

SRE_console: $(APP_OBJS) $(ENGINE_LIB)
	$(PRINTF) LD $@
	$(ECHO)	$(LINK.cpp) $^ $(LOADLIBES)  -o $(BUILDDIR)/$@  $(LDLIBS)

$(OBJS): | $(DIRS)

$(BUILDDIR)/%.o: %.c
	$(PRINTF) $(CC) $@
	$(ECHO)	$(COMPILE.c) $(OUTPUT_OPTION) $<
	
$(BUILDDIR)/%.o: %.cpp
	$(PRINTF) $(CXX) $@
	$(ECHO)	$(COMPILE.cpp) $(OUTPUT_OPTION) $<
