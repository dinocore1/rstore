
GTEST_PROJECT_DIR := $(BUILD_DIR)/googletest/googletest

GTEST_SRC := \
	$(GTEST_PROJECT_DIR)/src/gtest-all.cc \
	$(GTEST_PROJECT_DIR)/src/gtest_main.cc

GTEST_OBJS := \
	$(GTEST_PROJECT_DIR)/src/gtest-all.o \
	$(GTEST_PROJECT_DIR)/src/gtest_main.o

gtest_COMPILE_DEPS := $(GTEST_SRC)
gtest_LINK_DEPS := $(GTEST_OBJS)
gtest_HEADER_EXPORTS := $(GTEST_PROJECT_DIR)/include

$(GTEST_SRC):
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && \
	git clone https://github.com/google/googletest.git && \
	cd googletest && \
	git checkout release-1.10.0


GTEST_LIB := $(BUILD_DIR)/googletest/googletest/gtest.a

$(GTEST_PROJECT_DIR)/src/gtest-all.o: $(GTEST_PROJECT_DIR)/src/gtest-all.cc
	$(CXX) -c -o $@ $(CXX_FLAGS) -I $(GTEST_PROJECT_DIR) -I $(GTEST_PROJECT_DIR)/include $<

$(GTEST_PROJECT_DIR)/src/gtest_main.o: $(GTEST_PROJECT_DIR)/src/gtest_main.cc
	$(CXX) -c -o $@ $(CXX_FLAGS) -I $(GTEST_PROJECT_DIR) -I $(GTEST_PROJECT_DIR)/include $<

$(GTEST_LIB): $(GTEST_PROJECT_DIR)/src/gtest-all.o
	$(AR) -rcs $@ $<

CLEAN_LIST += \
	$(GTEST_PROJECT_DIR)/src/gtest-all.o \
	$(GTEST_PROJECT_DIR)/src/gtest_main.o \
	$(GTEST_LIB)