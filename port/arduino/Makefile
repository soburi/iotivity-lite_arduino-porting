include setup.mk

ROOT_DIR =../..

ifneq ($(ARCH),avr)
ifneq ($(ARCH),samd)
ifneq ($(ARCH),sam)
$(error Target Architecture must be define to proceed!)
endif
endif
endif

ifeq ($(APP),server)
  LOCAL_CPP_SRCS 	+= server_arduino.cpp
  CXXFLAGS 	+= -DOC_SERVER
  TARGET = $(ARCH)_server
ifeq ($(XMEM),1)
  CXXFLAGS 		    += -DOC_XMEM
endif
else ifeq ($(APP),client)
  CXXFLAGS 	+= -DOC_CLIENT
ifeq ($(XMEM),1)
  CXXFLAGS 	+= -DOC_XMEM
endif
  LOCAL_CPP_SRCS 	+= client_arduino.cpp
  TARGET = $ARCH_client
else
$(error Must define an app server/client to proceed!)
endif

ifeq ($(VERBOSE),1)
        CXXFLAGS += -DOC_DEBUG -DOC_LOG_L1
else ifeq ($(VERBOSE),2)
        CXXFLAGS += -DOC_DEBUG  -DOC_LOG_L2
endif

ifeq ($(DYNAMIC),1)
	CXXFLAGS += -DOC_DYNAMIC_ALLOCATION
endif

ifeq ($(XMEM),1)
	CXXFLAGS += -DOC_XMEM
endif

ifeq ($(IPV4),1)
	CXXFLAGS += -DOC_IPV4
endif

ifeq ($(COLLECTIONS),1)
	CXXFLAGS += -DOC_COLLECTIONS
endif



### Iotivity contrained includes
CORE_HEADERS +=$(addprefix -I$(ROOT_DIR)/, . messaging/coap util util/pt include  api port security deps/mbedtls/include/mbedtls)
LIB_HEADERS  +=-I$(ARDUINO_DIR)/libraries/SdFat/src/FatLib
CXXFLAGS += $(DEPS_HEADERS) $(CORE_HEADERS) $(LIB_HEADERS) -Iadapter -Iapps/include

ifeq ($(NO_MAIN),1)
	CXXFLAGS += -DNO_MAIN
endif

ifeq ($(SECURE),1)
	CXXFLAGS	+=$(addprefix -I$(ROOT_DIR)/deps/mbedtls/, include include/mbedtls)
	CXXFLAGS	+= -DOC_SECURITY
endif

SERVER_ARCHIVE 	= build-$(BOARD_TAG)/libarduino-adapter.a
SERVER_OBJ     = adapter/build-$(BOARD_TAG)/libarduino-adapter.a
OTHER_OBJS    += $(SERVER_OBJ)

VPATH=apps/server:$(ROOT_DIR)/apps:

ifeq ($(ARCH),avr)
	include avr.mk

else ifeq ($(ARCH),sam)
	include sam.mk

else ifeq ($(ARCH),samd)
	include samd.mk
else
$(error Target Architecture must be define to proceed!)
endif


$(SERVER_OBJ):
	$(MAKE) -C adapter $(SERVER_ARCHIVE)

clean::
	$(MAKE) -C adapter clean
