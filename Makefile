
CC := cc
CXX := c++
C_FLAGS := -g -O0
CXX_FLAGS := -std=c++14 -g -O0
BUILD_DIR := build
ALL_MODULES :=
CLEAN_LIST :=
TEST_LIST :=
BUILD_DIRS :=

.PHONY: all clean distclean test format

all:
	make $(TEST_LIST)

format:
	astyle -n --options=astyle.cfg --recursive "src/*.cpp" "src/*.h"

distclean:
	rm -rf $(BUILD_DIR)

include gtest.mk
include build.mk

$(eval $(call new_module))
LOCAL_MODULE := rstore
LOCAL_SRC := \
	src/rstore/src/rollsum.c
LOCAL_INCLUDES :=
LOCAL_HEADER_EXPORTS := src/rstore/headers
$(eval $(call build_lib))

$(eval $(call new_module))
LOCAL_MODULE := rstore_test
LOCAL_LIBRARIES := rstore
LOCAL_SRC := src/rstore/test/test.cpp
$(eval $(call build_test))


test: $(TEST_LIST)
	$(foreach exe,$(TEST_LIST),./$(exe))

clean:
	rm -rf $(sort $(CLEAN_LIST))

define build_dir_rule
$(1):
	mkdir -p $(1)

endef

BUILD_DIRS += build

$(foreach build_dir,$(sort $(BUILD_DIRS)),$(eval $(call build_dir_rule,$(build_dir))))