
define new_module
LOCAL_MODULE :=
LOCAL_SRC :=
LOCAL_C_FLAGS :=
LOCAL_CXX_FLAGS :=
LOCAL_LD_FLAGS :=
LOCAL_LIBRARIES :=
LOCAL_OBJS :=
endef

define uniq
  $(eval seen :=)
  $(foreach _,$1,$(if $(filter $_,${seen}),,$(eval seen += $_)))
  ${seen}
endef

define c_to_o
$(eval src_file := $(1))
$(eval c_flags := $(addprefix -I,$(includes)) $(LOCAL_C_FLAGS) $(C_FLAGS) )
$(eval LOCAL_BUILD_DIR := $(patsubst %/,%,$(BUILD_DIR)/$(dir $(1))))
$(eval BUILD_DIRS += $(LOCAL_BUILD_DIR))
$(eval dep_file := $$(LOCAL_BUILD_DIR)/$(basename $(notdir $(1))).d)
$(eval LOCAL_OBJ := $$(LOCAL_BUILD_DIR)/$(basename $(notdir $(1))).o)
$(eval LOCAL_OBJS += $$(LOCAL_OBJ))

$(LOCAL_OBJ): $(src_file) $(dep_file) $(compile_deps) | $(LOCAL_BUILD_DIR)
	$$(CC) -c -o $$@ $(c_flags) $(src_file) -MT $$@ -MMD -MP -MF $(dep_file)

$(dep_file):

CLEAN_LIST += $$(LOCAL_OBJ)

include $(dep_file)

endef

define cpp_to_o
$(eval src_file := $(1))
$(eval cxx_flags := $(addprefix -I,$(includes)) $(LOCAL_CXX_FLAGS) $(CXX_FLAGS) )
$(eval LOCAL_BUILD_DIR := $(patsubst %/,%,$(BUILD_DIR)/$(dir $(1))))
$(eval BUILD_DIRS += $(LOCAL_BUILD_DIR))
$(eval dep_file := $$(LOCAL_BUILD_DIR)/$(basename $(notdir $(1))).d)
$(eval LOCAL_OBJ := $$(LOCAL_BUILD_DIR)/$(basename $(notdir $(1))).o)
$(eval LOCAL_OBJS += $$(LOCAL_OBJ))

$(LOCAL_OBJ): $(src_file) $(dep_file) $(compile_deps) | $(LOCAL_BUILD_DIR)
	$$(CXX) -c -o $$@ $(cxx_flags) $(src_file) -MT $$@ -MMD -MP -MF $(dep_file)

$(dep_file):

CLEAN_LIST += $$(LOCAL_OBJ)

include $(dep_file)

endef

define lib_to_exported_headers
$$($(1)_HEADER_EXPORTS) $(foreach lib,$(value $(1)_LIBRARIES),$(call lib_to_exported_headers,$(lib)))
endef

define lib_to_link_deps
$$($(1)_LINK_DEPS) $(foreach lib,$(value $(1)_LIBRARIES),$(call lib_to_link_deps,$(lib)))
endef

define lib_to_compile_dep
$$($(1)_COMPILE_DEPS) $(foreach lib,$(value $(1)_LIBRARIES),$(call lib_to_compile_dep,$(lib)))
endef

define lib_to_ld_flags
$$($(1)_STATIC_LIB) $$($(1)_LD_FLAGS) $(foreach lib,$(value $(1)_LIBRARIES),$(call lib_to_ld_flags,$(lib)))
endef

define all_includes
$(foreach lib,$(LOCAL_LIBRARIES),$(call lib_to_exported_headers,$(lib)))
endef

define all_compile_deps
$(foreach lib,$(LOCAL_LIBRARIES),$(call lib_to_compile_dep,$(lib)))
endef

define all_link_deps
$(sort $(foreach lib,$(LOCAL_LIBRARIES),$(call lib_to_link_deps,$(lib))))
endef

define all_ld_flags
$(foreach lib,$(LOCAL_LIBRARIES),$(call lib_to_ld_flags,$(lib)))
endef

define build_module
ALL_MODULES += $(LOCAL_MODULE)

$(eval compile_deps := $(call all_compile_deps))
$(eval includes := $$(LOCAL_INCLUDES) $(call all_includes))

$(eval c_srcs := $(filter %.c,$(LOCAL_SRC)))
$(foreach src,$(c_srcs),$(eval $(call c_to_o,$(src))))

$(eval cxx_srcs := $(filter %.cpp %.cc %.cxx,$(LOCAL_SRC)))
$(foreach src,$(cxx_srcs),$(eval $(call cpp_to_o,$(src))))
endef

define build_lib
$(eval LOCAL_INCLUDES += $(LOCAL_HEADER_EXPORTS))
$(eval $(call build_module))
$(eval $(LOCAL_MODULE)_HEADER_EXPORTS := $(LOCAL_HEADER_EXPORTS) )
$(eval $(LOCAL_MODULE)_LIBRARIES := $(LOCAL_LIBRARIES))
$(eval $(LOCAL_MODULE)_STATIC_LIB := $(BUILD_DIR)/lib/$(LOCAL_MODULE).a)
$(eval $(LOCAL_MODULE)_LD_FLAGS := $(LOCAL_LD_FLAGS))
$(eval $(LOCAL_MODULE)_LINK_DEPS := $(value $(LOCAL_MODULE)_STATIC_LIB))

$(value $(LOCAL_MODULE)_STATIC_LIB): $(LOCAL_OBJS) | $(BUILD_DIR)/lib
	$$(AR) -rcs $$@ $(LOCAL_OBJS)

endef

define build_exe

$(eval $(call build_module))
$(eval LOCAL_EXE := $(BUILD_DIR)/exe/$(LOCAL_MODULE))
$(eval $(LOCAL_MODULE)_ld_flags := $(foreach lib,$(LOCAL_LIBRARIES),$(call lib_to_ld_flags,$(lib))))
$(eval $(LOCAL_MODULE)_ld_flags += $(LOCAL_LD_FLAGS))
$(eval link_deps := $(call all_link_deps))

$(LOCAL_EXE): $(LOCAL_OBJS) $(link_deps) | $(BUILD_DIR)/exe	
	$$(CXX) -o $$@ $(LOCAL_OBJS) $(value $(LOCAL_MODULE)_ld_flags)

CLEAN_LIST += $(LOCAL_EXE)
endef

define build_test

$(eval LOCAL_LIBRARIES += gtest)
$(eval $(call build_module))
$(eval LOCAL_EXE := $(BUILD_DIR)/test/$(LOCAL_MODULE))
$(eval $(LOCAL_MODULE)_ld_flags := $(foreach lib,$(LOCAL_LIBRARIES),$(call lib_to_ld_flags,$(lib))))
$(eval $(LOCAL_MODULE)_ld_flags += $(LOCAL_LD_FLAGS))
$(eval link_deps := $(call all_link_deps))

$(LOCAL_EXE): $(LOCAL_OBJS) $$(GTEST_OBJS) $(link_deps) | $(BUILD_DIR)/test	
	$$(CXX) -o $$@ $(LOCAL_OBJS) $$(GTEST_OBJS) $(value $(LOCAL_MODULE)_ld_flags) -lpthread

CLEAN_LIST += $(LOCAL_EXE)
TEST_LIST += $(LOCAL_EXE)

endef

$(BUILD_DIR)/test:
	mkdir -p $(BUILD_DIR)/test

$(BUILD_DIR)/lib:
	mkdir -p $(BUILD_DIR)/lib

$(BUILD_DIR)/exe:
	mkdir -p $(BUILD_DIR)/exe