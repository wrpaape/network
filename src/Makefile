# ══════════════════════════════════════════════════════════════════════════════
# ──────────────────────────────────────────────────────────────────────────────
#				CONFIGURATION
# ──────────────────────────────────────────────────────────────────────────────
# ══════════════════════════════════════════════════════════════════════════════
#
#
## MISC
# ══════════════════════════════════════════════════════════════════════════════
# Development Mode
# ──────────────────────────────────────────────────────────────────────────────
# toggles utility flags, make targets
DEVELOPMENT_MODE = T


# Phony Targets
# ──────────────────────────────────────────────────────────────────────────────
.PHONY: all clean


# String Utils
# ──────────────────────────────────────────────────────────────────────────────
EMPTY	       :=
SPACE	       := $(EMPTY) $(EMPTY)
TAB	       := $(EMPTY)	$(EMPTY)
TRIM		= $(subst $(SPACE),$(EMPTY),$1)
JOIN	        = $(subst $(SPACE),$2,$(strip $1))
define NEWLINE :=


endef

SENDMAIL_PATH := $(shell which sendmail)

ifeq ($(EMPTY),$(SENDMAIL_PATH))
        $(error failed to find 'sendmail')
endif

# SYSTEM CONFIG
# ══════════════════════════════════════════════════════════════════════════════
# extended from user 'Trevor Robinson''s response to stackoverflow question:
#
# 	http://stackoverflow.com/questions/714100/os-detecting-makefile

ifeq ($(OS),Windows_NT)
        SYSTEM_WINDOWS 	:= T
        SYS_ENV_FLAGS 	:= -DWIN32

        ifeq      (AMD64,$(PROCESSOR_ARCHITECTURE))
                ARCH_X86_64	:= T
                SYS_ARCH_FLAG	:= -arch x86_64
                SYS_ENV_FLAGS	+= -DAMD_64

        else ifeq (x86,$(PROCESSOR_ARCHITECTURE))
                ARCH_I386	:= T
                SYS_ARCH_FLAG	:= -arch i386
                SYS_ENV_FLAGS	+= -DIA_32
        endif
else
        UNAME_S := $(shell uname -s)

        ifeq      ($(UNAME_S),Linux)
                SYSTEM_LINUX 	:= T
                SYS_ENV_FLAGS 	:= -DLINUX

        else ifeq ($(UNAME_S),Darwin)
                SYSTEM_OSX 	:= T
                SYS_ENV_FLAGS 	:= -DOSX
        endif

        UNAME_M := $(shell uname -m)

        ifeq      (x86_64,$(UNAME_M))
                ARCH_X86_64	:= T
                SYS_ARCH_FLAG	:= -arch x86_64
                SYS_ENV_FLAGS	+= -DAMD_64

        else ifneq (,$(filter %86,$(UNAME_M)))
                ARCH_I386	:= T
                SYS_ARCH_FLAG	:= -arch i386
                SYS_ENV_FLAGS 	+= -DIA_32

        else ifneq (,$(filter arm%,$(UNAME_M)))

                ifneq (,$(filter %32%,$(UNAME_M)))
                        ARCH_ARM_32	:= T
                        SYS_ARCH_FLAG	:= -arch AArch32
                        SYS_ENV_FLAGS 	+= -DARM_32
                else
                        ARCH_ARM_64	:= T
                        SYS_ARCH_FLAG	:= -arch AArch64
                        SYS_ENV_FLAGS 	+= -DARM_64
                endif
        endif
endif

# Path delimiter, chain commands token, executable file extension
# ──────────────────────────────────────────────────────────────────────────────
ifeq (T,$(SYSTEM_WINDOWS))
PATH_DELIM	:= \\
CMD_DELIM 	:= &
BIN_EXT		:= .exe
else
PATH_DELIM	:= /
CMD_DELIM 	:= ;
BIN_EXT		:= $(EMPTY)
endif




# DIRECTORY CONFIG
# ══════════════════════════════════════════════════════════════════════════════
PATH_JOIN		 = $(call JOIN,$1,$(PATH_DELIM))
SYSTEM_ROOT_PATH	 = $(call TRIM,$(PATH_DELIM)$(call PATH_JOIN,$1))
PROJECT_ROOT		:= ..
SOURCE_DIR		:= $(call PATH_JOIN,$(PROJECT_ROOT) src)
HEADER_DIR		:= $(SOURCE_DIR)
OBJECT_DIR		:= $(call PATH_JOIN,$(PROJECT_ROOT) obj)
BINARY_DIR		:= $(call PATH_JOIN,$(PROJECT_ROOT) bin)




# 'windows' directory, library flags
# ──────────────────────────────────────────────────────────────────────────────
ifeq (T,$(SYSTEM_WINDOWS))
        WINDOWS_DIR		:= $(call PATH_JOIN,$(PROJECT_ROOT) windows)
        THREADS_DIR		:= $(call PATH_JOIN,$(WINDOWS_DIR) pthreads_win32)
        THREADS_INC_DIR		:= $(call PATH_JOIN,$(THREADS_DIR) include)
        THREADS_LIB_ROOT	:= $(call PATH_JOIN,$(THREADS_DIR) dll)

        ifeq (T,$(ARCH_X86_64))
                        THREADS_LIB_DIR := $(call PATH_JOIN,$(THREADS_LIB_ROOT) x64)
        else
                        THREADS_LIB_DIR := $(call PATH_JOIN,$(THREADS_LIB_ROOT) x86)
        endif

        THREADS_LIB_FLAG	:= -L$(THREADS_LIB_DIR) -lpthreadGC2.dll
        THREADS_INC_FLAG	:= -I$(THREADS_INC_DIR)

        SYSTEM_LIB_FLAGS	:= -lphlpapi -lWs2_32 -lUser32 -lKernel32
else
        THREADS_LIB_FLAG	:= -lpthread
        THREADS_INC_FLAG	:= $(EMPTY)

        ifeq (T,$(SYSTEM_LINUX))
                        SYSTEM_LIB_FLAGS	:= -lrt -lm
        else
                        SYSTEM_LIB_FLAGS	:= $(EMPTY)
        endif
endif




# PATH CONFIG
# ══════════════════════════════════════════════════════════════════════════════
FILE_PATH			= $(call PATH_JOIN,$1 $(call TRIM,$2))
SCRIPT_FILE_PATH		= $(call FILE_PATH,$1,$2.rb)
SOURCE_FILE_PATH		= $(call FILE_PATH,$1,$2.c)
HEADER_FILE_PATH		= $(call FILE_PATH,$1,$2.h)
OBJECT_FILE_PATH		= $(call FILE_PATH,$1,$2.o)
BINARY_FILE_PATH		= $(call FILE_PATH,$1,$2$(BIN_EXT))

# Project
# ─────────────── source ───────────────────────────────────────────────────────
SOURCE_PATH			= $(call SOURCE_FILE_PATH,$(SOURCE_DIR),$1)# implementation
HEADER_PATH			= $(call HEADER_FILE_PATH,$(HEADER_DIR),$1)# interface
# ─────────────── build ────────────────────────────────────────────────────────
OBJECT_PATH   			= $(call OBJECT_FILE_PATH,$(OBJECT_DIR),$1)# object files
BINARY_PATH 			= $(call BINARY_FILE_PATH,$(BINARY_DIR),$1)# binary executable output

# Helper Project Module
# ─────────────── source ───────────────────────────────────────────────────────
HELPER_SOURCE_PATH		= $(call SOURCE_PATH,$(call PATH_JOIN,$1))# implementation
HELPER_HEADER_PATH		= $(call HEADER_PATH,$(call PATH_JOIN,$1))# interface
# ─────────────── build ────────────────────────────────────────────────────────
HELPER_OBJECT_PATH   		= $(call OBJECT_PATH,$(call JOIN,$1,_))# object files


# Main Project Module
# ─────────────── source ───────────────────────────────────────────────────────
PROJECT_SOURCE_PATH		= $(call SOURCE_PATH,$(PROJECT)_$1)# implementation
PROJECT_HEADER_PATH		= $(call HEADER_PATH,$(PROJECT)_$1)# interface
# ─────────────── build ────────────────────────────────────────────────────────
PROJECT_OBJECT_PATH   		= $(call OBJECT_PATH,$(PROJECT)_$1)# object files





# UTILITY CONFIG
# ══════════════════════════════════════════════════════════════════════════════
# C Compiler
# ──────────────────────────────────────────────────────────────────────────────
CC		:= gcc
CC_ENV_FLAGS	:= -D__USE_FIXED_PROTOTYPES__ -D_GNU_SOURCE -DROOT_ABSPATH=$(PROJECT_ROOT_ABSOLUTE) $(SYS_ENV_FLAGS)
CC_BASE_FLAGS	:= -std=gnu99 -march=native $(CC_ENV_FLAGS) -I$(HEADER_DIR) $(THREADS_INC_FLAG)

ifeq (T,$(DEVELOPMENT_MODE))
CC_FLAGS	:= -g -Wall $(CC_BASE_FLAGS)
else
CC_FLAGS	:= -O2 -funroll-loops $(CC_BASE_FLAGS)
endif
# Linker
# ──────────────────────────────────────────────────────────────────────────────
ifeq (T,$(SYSTEM_OSX))
LD		:= ld
LD_FLAGS	:= -macosx_version_min 10.11 -no_pie $(SYS_ARCH_FLAG)
else
LD		:= gcc
LD_FLAGS	:= $(EMPTY)
endif

LD_LIB_FLAGS	:= $(THREADS_LIB_FLAG) $(SYSTEM_LIB_FLAGS) -lc

# Make Directory
# ──────────────────────────────────────────────────────────────────────────────
MKDIR	    	:= mkdir
MKDIR_FLAGS 	:= $(EMPTY)

ifeq (T,$(SYSTEM_WINDOWS))
MKDIR_IF_DNE	= if not exist $1 $(MKDIR) $(MKDIR_FLAGS) $1
else
MKDIR_IF_DNE	= $(MKDIR) $(MKDIR_FLAGS) -p $1
endif

# Cleaner
# ──────────────────────────────────────────────────────────────────────────────
ifeq (T,$(SYSTEM_WINDOWS))
# 'clean' command exceeds 8192 character limit for windows
RM		:= cmd \/C del
RM_FLAGS	:= /F /Q
# RM		:= rm
# RM_FLAGS	:= -rf
else
RM		:= rm
RM_FLAGS	:= -rf
endif
# Ruby
# ──────────────────────────────────────────────────────────────────────────────
RUBY		:= ruby
RUBY_FLAGS	:= $(EMPTY)



# independent headers
# ──────────────────────────────────────────────────────────────────────────────
UTILS_TYPES_WORD_HDR		:= $(call HELPER_HEADER_PATH,utils types word)
UTILS_TYPES_OCTET_HDR		:= $(call HELPER_HEADER_PATH,utils types octet)
UTILS_TYPES_UINT128_HDR		:= $(call HELPER_HEADER_PATH,utils types uint128)
UTILS_TYPES_WIDTH_HDR		:= $(call HELPER_HEADER_PATH,utils types width)
UTILS_TYPES_BOOL_STATUS_HDR	:= $(call HELPER_HEADER_PATH,utils types bool_status)
UTILS_TYPES_ROUTINE_HDR		:= $(call HELPER_HEADER_PATH,utils types routine)
UTILS_TYPES_PROCEDURE_HDR	:= $(call HELPER_HEADER_PATH,utils types procedure)
UTILS_TYPES_HANDLER_HDR		:= $(call HELPER_HEADER_PATH,utils types handler)
UTILS_TYPES_WORD_PATTERN_HDR	:= $(call HELPER_HEADER_PATH,utils types word_pattern)
UTILS_TYPES_ACTION_HDR		:= $(call HELPER_HEADER_PATH,utils types action)
UTILS_CHAR_ATTRS_HDR		:= $(call HELPER_HEADER_PATH,utils char_attrs)
UTILS_WORD_ATTRS_HDR		:= $(call HELPER_HEADER_PATH,utils word_attrs)
UTILS_FAIL_SWITCH_HDR		:= $(call HELPER_HEADER_PATH,utils fail_switch)
MEMORY_MEMORY_SET_WIDTH_HDR	:= $(call HELPER_HEADER_PATH,memory memory_set_width)
MEMORY_MEMORY_GET_WIDTH_HDR	:= $(call HELPER_HEADER_PATH,memory memory_get_width)
MEMORY_MEMORY_PUT_WIDTH_HDR	:= $(call HELPER_HEADER_PATH,memory memory_put_width)
MEMORY_MEMORY_SWAP_WIDTH_HDR	:= $(call HELPER_HEADER_PATH,memory memory_swap_width)
MEMORY_WORD_REM_UTILS_HDR	:= $(call HELPER_HEADER_PATH,memory word_rem_utils)
STRING_INT_STRING_ATTRS_HDR	:= $(call HELPER_HEADER_PATH,string int_string_attrs)
STRING_PTR_STRING_ATTRS_HDR	:= $(call HELPER_HEADER_PATH,string ptr_string_attrs)
STRING_STRING_TOKEN_HDR		:= $(call HELPER_HEADER_PATH,string string_token)
STRING_STRING_MACROS_HDR	:= $(call HELPER_HEADER_PATH,string string_macros)


## HELPER MODULES
# ══════════════════════════════════════════════════════════════════════════════
# utils/utils
# ──────────────────────────────────────────────────────────────────────────────
UTILS_UTILS_SRC 		:= $(call HELPER_SOURCE_PATH,utils utils)
UTILS_UTILS_HDR			:= $(call HELPER_HEADER_PATH,utils utils)
UTILS_UTILS_OBJ 		:= $(call HELPER_OBJECT_PATH,utils utils)
# ─────────────── target prequisites ───────────────────────────────────────────
UTILS_UTILS_OBJ_PREQS		:= $(UTILS_UTILS_SRC) 			\
			   	   $(UTILS_UTILS_HDR)
UTILS_UTILS_OBJ_GROUP		:= $(UTILS_UTILS_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS				+= $(UTILS_UTILS_OBJ)



# utils/closure
# ──────────────────────────────────────────────────────────────────────────────
UTILS_CLOSURE_SRC 		:= $(call HELPER_SOURCE_PATH,utils closure)
UTILS_CLOSURE_HDR		:= $(call HELPER_HEADER_PATH,utils closure)
UTILS_CLOSURE_OBJ 		:= $(call HELPER_OBJECT_PATH,utils closure)
# ─────────────── target prequisites ───────────────────────────────────────────
UTILS_CLOSURE_OBJ_PREQS		:= $(UTILS_CLOSURE_SRC) 	\
			   	   $(UTILS_CLOSURE_HDR)		\
			   	   $(UTILS_TYPES_ROUTINE_HDR)	\
				   $(UTILS_TYPES_PROCEDURE_HDR)	\
				   $(UTILS_TYPES_HANDLER_HDR)
UTILS_CLOSURE_OBJ_GROUP		:= $(UTILS_CLOSURE_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS				+= $(UTILS_CLOSURE_OBJ)



# utils/uint128_ops
# ──────────────────────────────────────────────────────────────────────────────
UTILS_UINT128_OPS_SRC 		:= $(call HELPER_SOURCE_PATH,utils uint128_ops)
UTILS_UINT128_OPS_HDR		:= $(call HELPER_HEADER_PATH,utils uint128_ops)
UTILS_UINT128_OPS_OBJ 		:= $(call HELPER_OBJECT_PATH,utils uint128_ops)
# ─────────────── target prequisites ───────────────────────────────────────────
UTILS_UINT128_OPS_OBJ_PREQS	:= $(UTILS_UINT128_OPS_SRC) 	\
			   	   $(UTILS_UINT128_OPS_HDR)	\
			   	   $(UTILS_TYPES_UINT128_HDR)
UTILS_UINT128_OPS_OBJ_GROUP	:= $(UTILS_UINT128_OPS_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS				+= $(UTILS_UINT128_OPS_OBJ)



# time/time_utils
# ──────────────────────────────────────────────────────────────────────────────
TIME_TIME_UTILS_SRC 		:= $(call HELPER_SOURCE_PATH,time time_utils)
TIME_TIME_UTILS_HDR		:= $(call HELPER_HEADER_PATH,time time_utils)
TIME_TIME_UTILS_OBJ 		:= $(call HELPER_OBJECT_PATH,time time_utils)
# ─────────────── target prequisites ───────────────────────────────────────────
TIME_TIME_UTILS_OBJ_PREQS	:= $(TIME_TIME_UTILS_SRC) 	\
			   	   $(TIME_TIME_UTILS_HDR)	\
				   $(UTILS_FAIL_SWITCH_HDR)	\
				   $(UTILS_CLOSURE_HDR)
TIME_TIME_UTILS_OBJ_GROUP	:= $(TIME_TIME_UTILS_OBJ)	\
				   $(UTILS_CLOSURE_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS				+= $(TIME_TIME_UTILS_OBJ)



# bitwise/bitwise_utils
# ──────────────────────────────────────────────────────────────────────────────
BITWISE_BITWISE_UTILS_SRC 	:= $(call HELPER_SOURCE_PATH,bitwise bitwise_utils)
BITWISE_BITWISE_UTILS_HDR	:= $(call HELPER_HEADER_PATH,bitwise bitwise_utils)
BITWISE_BITWISE_UTILS_OBJ 	:= $(call HELPER_OBJECT_PATH,bitwise bitwise_utils)
# ─────────────── target prequisites ───────────────────────────────────────────
BITWISE_BITWISE_UTILS_OBJ_PREQS	:= $(BITWISE_BITWISE_UTILS_SRC) \
			      	   $(BITWISE_BITWISE_UTILS_HDR)	\
			   	   $(UTILS_WORD_ATTRS_HDR)
BITWISE_BITWISE_UTILS_OBJ_GROUP	:= $(BITWISE_BITWISE_UTILS_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS				+= $(BITWISE_BITWISE_UTILS_OBJ)



# memory/memory_utils
# ──────────────────────────────────────────────────────────────────────────────
MEMORY_MEMORY_UTILS_SRC 	:= $(call HELPER_SOURCE_PATH,memory memory_utils)
MEMORY_MEMORY_UTILS_HDR		:= $(call HELPER_HEADER_PATH,memory memory_utils)
MEMORY_MEMORY_UTILS_OBJ 	:= $(call HELPER_OBJECT_PATH,memory memory_utils)
# ─────────────── target prequisites ───────────────────────────────────────────
MEMORY_MEMORY_UTILS_OBJ_PREQS	:= $(MEMORY_MEMORY_UTILS_SRC) 	\
			      	   $(MEMORY_MEMORY_UTILS_HDR)	\
			      	   $(UTILS_UTILS_HDR)		\
			      	   $(UTILS_TYPES_WIDTH_HDR)
MEMORY_MEMORY_UTILS_OBJ_GROUP	:= $(MEMORY_MEMORY_UTILS_OBJ) 	\
				   $(UTILS_UTILS_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS				+= $(MEMORY_MEMORY_UTILS_OBJ)



# memory/memory_zero
# ──────────────────────────────────────────────────────────────────────────────
MEMORY_MEMORY_ZERO_SRC 		:= $(call HELPER_SOURCE_PATH,memory memory_zero)
MEMORY_MEMORY_ZERO_HDR		:= $(call HELPER_HEADER_PATH,memory memory_zero)
MEMORY_MEMORY_ZERO_OBJ 		:= $(call HELPER_OBJECT_PATH,memory memory_zero)
# ─────────────── target prequisites ───────────────────────────────────────────
MEMORY_MEMORY_ZERO_OBJ_PREQS	:= $(MEMORY_MEMORY_ZERO_SRC) 		\
			      	   $(MEMORY_MEMORY_ZERO_HDR)		\
			      	   $(MEMORY_WORD_REM_UTILS_HDR)
MEMORY_MEMORY_ZERO_OBJ_GROUP	:= $(MEMORY_MEMORY_ZERO_OBJ)		\
				   $(MEMORY_MEMORY_UTILS_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS				+= $(MEMORY_MEMORY_ZERO_OBJ)



# memory/memory_copy_array
# ──────────────────────────────────────────────────────────────────────────────
MEMORY_MEMORY_COPY_ARRAY_SRC 		:= $(call HELPER_SOURCE_PATH,memory memory_copy_array)
MEMORY_MEMORY_COPY_ARRAY_HDR		:= $(call HELPER_HEADER_PATH,memory memory_copy_array)
MEMORY_MEMORY_COPY_ARRAY_OBJ 		:= $(call HELPER_OBJECT_PATH,memory memory_copy_array)
# ─────────────── target prequisites ───────────────────────────────────────────
MEMORY_MEMORY_COPY_ARRAY_OBJ_PREQS	:= $(MEMORY_MEMORY_COPY_ARRAY_SRC) 		\
			   	   	   $(MEMORY_MEMORY_COPY_ARRAY_HDR)		\
			   	   	   $(MEMORY_MEMORY_SET_WIDTH_HDR)		\
			   	   	   $(MEMORY_WORD_REM_UTILS_HDR)
MEMORY_MEMORY_COPY_ARRAY_OBJ_GROUP	:= $(MEMORY_MEMORY_COPY_ARRAY_OBJ)		\
					   $(MEMORY_MEMORY_UTILS_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS					+= $(MEMORY_MEMORY_COPY_ARRAY_OBJ)


# memory/memory_put_array
# ──────────────────────────────────────────────────────────────────────────────
MEMORY_MEMORY_PUT_ARRAY_SRC 		:= $(call HELPER_SOURCE_PATH,memory memory_put_array)
MEMORY_MEMORY_PUT_ARRAY_HDR		:= $(call HELPER_HEADER_PATH,memory memory_put_array)
MEMORY_MEMORY_PUT_ARRAY_OBJ 		:= $(call HELPER_OBJECT_PATH,memory memory_put_array)
# ─────────────── target prequisites ───────────────────────────────────────────
MEMORY_MEMORY_PUT_ARRAY_OBJ_PREQS	:= $(MEMORY_MEMORY_PUT_ARRAY_SRC) 		\
			   	   	   $(MEMORY_MEMORY_PUT_ARRAY_HDR)		\
			   	   	   $(MEMORY_MEMORY_PUT_WIDTH_HDR)		\
			   	   	   $(MEMORY_WORD_REM_UTILS_HDR)
MEMORY_MEMORY_PUT_ARRAY_OBJ_GROUP	:= $(MEMORY_MEMORY_PUT_ARRAY_OBJ)		\
					   $(MEMORY_MEMORY_UTILS_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS					+= $(MEMORY_MEMORY_PUT_ARRAY_OBJ)



# memory/memory_swap
# ──────────────────────────────────────────────────────────────────────────────
MEMORY_MEMORY_SWAP_SRC 		:= $(call HELPER_SOURCE_PATH,memory memory_swap)
MEMORY_MEMORY_SWAP_HDR		:= $(call HELPER_HEADER_PATH,memory memory_swap)
MEMORY_MEMORY_SWAP_OBJ 		:= $(call HELPER_OBJECT_PATH,memory memory_swap)
# ─────────────── target prequisites ───────────────────────────────────────────
MEMORY_MEMORY_SWAP_OBJ_PREQS	:= $(MEMORY_MEMORY_SWAP_SRC) 		\
			      	   $(MEMORY_MEMORY_SWAP_HDR)		\
			      	   $(MEMORY_MEMORY_SWAP_WIDTH_HDR)	\
				   $(MEMORY_MEMORY_COPY_ARRAY_HDR)
MEMORY_MEMORY_SWAP_OBJ_GROUP	:= $(MEMORY_MEMORY_SWAP_OBJ)		\
				   $(MEMORY_MEMORY_COPY_ARRAY_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS				+= $(MEMORY_MEMORY_SWAP_OBJ)



# random/pcg_random
# ──────────────────────────────────────────────────────────────────────────────
RANDOM_PCG_RANDOM_SRC 		:= $(call HELPER_SOURCE_PATH,random pcg_random)
RANDOM_PCG_RANDOM_HDR		:= $(call HELPER_HEADER_PATH,random pcg_random)
RANDOM_PCG_RANDOM_OBJ 		:= $(call HELPER_OBJECT_PATH,random pcg_random)
# ─────────────── target prequisites ───────────────────────────────────────────
RANDOM_PCG_RANDOM_OBJ_PREQS	:= $(RANDOM_PCG_RANDOM_SRC) 		\
			   	   $(RANDOM_PCG_RANDOM_HDR)		\
			   	   $(BITWISE_BITWISE_UTILS_HDR)		\
			   	   $(UTILS_UINT128_OPS_HDR)
RANDOM_PCG_RANDOM_OBJ_GROUP	:= $(RANDOM_PCG_RANDOM_OBJ)		\
			   	   $(BITWISE_BITWISE_UTILS_OBJ_GROUP)	\
			   	   $(UTILS_UINT128_OPS_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS				+= $(RANDOM_PCG_RANDOM_OBJ)



# random/random
# ──────────────────────────────────────────────────────────────────────────────
RANDOM_RANDOM_SRC 		:= $(call HELPER_SOURCE_PATH,random random)
RANDOM_RANDOM_HDR		:= $(call HELPER_HEADER_PATH,random random)
RANDOM_RANDOM_OBJ 		:= $(call HELPER_OBJECT_PATH,random random)
# ─────────────── target prequisites ───────────────────────────────────────────
RANDOM_RANDOM_OBJ_PREQS		:= $(RANDOM_RANDOM_SRC) 		\
			   	   $(RANDOM_RANDOM_HDR)			\
			   	   $(RANDOM_PCG_RANDOM_HDR)		\
			   	   $(MEMORY_MEMORY_SWAP_HDR)		\
			   	   $(TIME_TIME_UTILS_HDR)
RANDOM_RANDOM_OBJ_GROUP		:= $(RANDOM_RANDOM_OBJ)			\
			   	   $(RANDOM_PCG_RANDOM_OBJ_GROUP)	\
			   	   $(MEMORY_MEMORY_SWAP_OBJ_GROUP)	\
			   	   $(TIME_TIME_UTILS_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS				+= $(RANDOM_RANDOM_OBJ)



# random/random_generator
# ──────────────────────────────────────────────────────────────────────────────
RANDOM_RANDOM_GENERATOR_SRC 		:= $(call HELPER_SOURCE_PATH,random random_generator)
RANDOM_RANDOM_GENERATOR_HDR		:= $(call HELPER_HEADER_PATH,random random_generator)
RANDOM_RANDOM_GENERATOR_OBJ 		:= $(call HELPER_OBJECT_PATH,random random_generator)
# ─────────────── target prequisites ───────────────────────────────────────────
RANDOM_RANDOM_GENERATOR_OBJ_PREQS	:= $(RANDOM_RANDOM_GENERATOR_SRC) 	\
			   	   	   $(RANDOM_RANDOM_GENERATOR_HDR)	\
			   	   	   $(RANDOM_RANDOM_HDR)
RANDOM_RANDOM_GENERATOR_OBJ_GROUP	:= $(RANDOM_RANDOM_GENERATOR_OBJ)	\
			   	   	   $(RANDOM_RANDOM_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS					+= $(RANDOM_RANDOM_GENERATOR_OBJ)



# string/ascii_utils
# ──────────────────────────────────────────────────────────────────────────────
STRING_ASCII_UTILS_SRC 		:= $(call HELPER_SOURCE_PATH,string ascii_utils)
STRING_ASCII_UTILS_HDR		:= $(call HELPER_HEADER_PATH,string ascii_utils)
STRING_ASCII_UTILS_OBJ 		:= $(call HELPER_OBJECT_PATH,string ascii_utils)
# ─────────────── target prequisites ───────────────────────────────────────────
STRING_ASCII_UTILS_OBJ_PREQS	:= $(STRING_ASCII_UTILS_SRC) 	\
			   	   $(STRING_ASCII_UTILS_HDR)	\
				   $(UTILS_TYPES_OCTET_HDR)
STRING_ASCII_UTILS_OBJ_GROUP	:= $(STRING_ASCII_UTILS_OBJ)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS				+= $(STRING_ASCII_UTILS_OBJ)



# string/utf8_utils
# ──────────────────────────────────────────────────────────────────────────────
STRING_UTF8_UTILS_SRC 		:= $(call HELPER_SOURCE_PATH,string utf8_utils)
STRING_UTF8_UTILS_HDR		:= $(call HELPER_HEADER_PATH,string utf8_utils)
STRING_UTF8_UTILS_OBJ 		:= $(call HELPER_OBJECT_PATH,string utf8_utils)
# ─────────────── target prequisites ───────────────────────────────────────────
STRING_UTF8_UTILS_OBJ_PREQS	:= $(STRING_UTF8_UTILS_SRC) 	\
			   	   $(STRING_UTF8_UTILS_HDR)	\
				   $(STRING_STRING_MACROS_HDR)	\
				   $(UTILS_CLOSURE_HDR)		\
				   $(UTILS_TYPES_OCTET_HDR)	\
				   $(UTILS_CHAR_ATTRS_HDR)
STRING_UTF8_UTILS_OBJ_GROUP	:= $(STRING_UTF8_UTILS_OBJ)	\
				   $(UTILS_CLOSURE_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS				+= $(STRING_UTF8_UTILS_OBJ)



# string/string_utils
# ──────────────────────────────────────────────────────────────────────────────
STRING_STRING_UTILS_SRC 	:= $(call HELPER_SOURCE_PATH,string string_utils)
STRING_STRING_UTILS_HDR		:= $(call HELPER_HEADER_PATH,string string_utils)
STRING_STRING_UTILS_OBJ 	:= $(call HELPER_OBJECT_PATH,string string_utils)
# ─────────────── target prequisites ───────────────────────────────────────────
STRING_STRING_UTILS_OBJ_PREQS	:= $(STRING_STRING_UTILS_SRC) 		\
			   	   $(STRING_STRING_UTILS_HDR)		\
			   	   $(MEMORY_MEMORY_PUT_ARRAY_HDR)	\
			   	   $(UTILS_TYPES_WORD_PATTERN_HDR)	\
			   	   $(STRING_INT_STRING_ATTRS_HDR)	\
			   	   $(STRING_PTR_STRING_ATTRS_HDR)	\
			   	   $(STRING_ASCII_UTILS_HDR)		\
			   	   $(STRING_UTF8_UTILS_HDR)		\
			   	   $(STRING_STRING_MACROS_HDR)
STRING_STRING_UTILS_OBJ_GROUP	:= $(STRING_STRING_UTILS_OBJ)		\
			   	   $(MEMORY_MEMORY_PUT_ARRAY_OBJ_GROUP)	\
				   $(STRING_ASCII_UTILS_OBJ_GROUP)	\
				   $(STRING_UTF8_UTILS_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(STRING_STRING_UTILS_OBJ)



# time/time_string
# ──────────────────────────────────────────────────────────────────────────────
TIME_TIME_STRING_SRC 		:= $(call HELPER_SOURCE_PATH,time time_string)
TIME_TIME_STRING_HDR		:= $(call HELPER_HEADER_PATH,time time_string)
TIME_TIME_STRING_OBJ 		:= $(call HELPER_OBJECT_PATH,time time_string)
# ─────────────── target prequisites ───────────────────────────────────────────
TIME_TIME_STRING_OBJ_PREQS	:= $(TIME_TIME_STRING_SRC) 		\
			   	   $(TIME_TIME_STRING_HDR)		\
			   	   $(TIME_TIME_UTILS_HDR)		\
			   	   $(STRING_STRING_UTILS_HDR)
TIME_TIME_STRING_OBJ_GROUP	:= $(TIME_TIME_STRING_OBJ)		\
				   $(TIME_TIME_UTILS_OBJ_GROUP)		\
				   $(STRING_STRING_UTILS_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS				+= $(TIME_TIME_STRING_OBJ)



# system/system_utils
# ──────────────────────────────────────────────────────────────────────────────
SYSTEM_SYSTEM_UTILS_SRC 	:= $(call HELPER_SOURCE_PATH,system system_utils)
SYSTEM_SYSTEM_UTILS_HDR		:= $(call HELPER_HEADER_PATH,system system_utils)
SYSTEM_SYSTEM_UTILS_OBJ 	:= $(call HELPER_OBJECT_PATH,system system_utils)
# ─────────────── target prequisites ───────────────────────────────────────────
SYSTEM_SYSTEM_UTILS_OBJ_PREQS	:= $(SYSTEM_SYSTEM_UTILS_SRC) 	\
			   	   $(SYSTEM_SYSTEM_UTILS_HDR)	\
			   	   $(UTILS_CLOSURE_HDR)		\
			   	   $(UTILS_FAIL_SWITCH_HDR)
SYSTEM_SYSTEM_UTILS_OBJ_GROUP	:= $(SYSTEM_SYSTEM_UTILS_OBJ)	\
				   $(UTILS_CLOSURE_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS			+= $(SYSTEM_SYSTEM_UTILS_OBJ)


# system/file_utils
# ──────────────────────────────────────────────────────────────────────────────
SYSTEM_FILE_UTILS_SRC 		:= $(call HELPER_SOURCE_PATH,system file_utils)
SYSTEM_FILE_UTILS_HDR		:= $(call HELPER_HEADER_PATH,system file_utils)
SYSTEM_FILE_UTILS_OBJ 		:= $(call HELPER_OBJECT_PATH,system file_utils)
# ─────────────── target prequisites ───────────────────────────────────────────
SYSTEM_FILE_UTILS_OBJ_PREQS	:= $(SYSTEM_FILE_UTILS_SRC) 		\
			   	   $(SYSTEM_FILE_UTILS_HDR)		\
			   	   $(UTILS_UTILS_HDR)			\
			   	   $(SYSTEM_SYSTEM_UTILS_HDR)		\
				   $(STRING_STRING_UTILS_HDR)		\
				   $(UTILS_TYPES_BOOL_STATUS_HDR)
SYSTEM_FILE_UTILS_OBJ_GROUP	:= $(SYSTEM_FILE_UTILS_OBJ)		\
			   	   $(UTILS_UTILS_OBJ_GROUP)		\
				   $(SYSTEM_SYSTEM_UTILS_OBJ_GROUP)	\
				   $(STRING_STRING_UTILS_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS				+= $(SYSTEM_FILE_UTILS_OBJ)



# system/stdio_utils
# ──────────────────────────────────────────────────────────────────────────────
SYSTEM_STDIO_UTILS_SRC 		:= $(call HELPER_SOURCE_PATH,system stdio_utils)
SYSTEM_STDIO_UTILS_HDR		:= $(call HELPER_HEADER_PATH,system stdio_utils)
SYSTEM_STDIO_UTILS_OBJ 		:= $(call HELPER_OBJECT_PATH,system stdio_utils)
# ─────────────── target prequisites ───────────────────────────────────────────
SYSTEM_STDIO_UTILS_OBJ_PREQS	:= $(SYSTEM_STDIO_UTILS_SRC) 		\
			   	   $(SYSTEM_STDIO_UTILS_HDR)		\
			   	   $(SYSTEM_FILE_UTILS_HDR)
SYSTEM_STDIO_UTILS_OBJ_GROUP	:= $(SYSTEM_STDIO_UTILS_OBJ)		\
			   	   $(SYSTEM_FILE_UTILS_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS				+= $(SYSTEM_STDIO_UTILS_OBJ)



# system/exit_utils
# ──────────────────────────────────────────────────────────────────────────────
SYSTEM_EXIT_UTILS_SRC 		:= $(call HELPER_SOURCE_PATH,system exit_utils)
SYSTEM_EXIT_UTILS_HDR		:= $(call HELPER_HEADER_PATH,system exit_utils)
SYSTEM_EXIT_UTILS_OBJ 		:= $(call HELPER_OBJECT_PATH,system exit_utils)
# ─────────────── target prequisites ───────────────────────────────────────────
SYSTEM_EXIT_UTILS_OBJ_PREQS	:= $(SYSTEM_EXIT_UTILS_SRC) 		\
			   	   $(SYSTEM_EXIT_UTILS_HDR)		\
			   	   $(SYSTEM_FILE_UTILS_HDR)		\
				   $(UTILS_TYPES_ACTION_HDR)
SYSTEM_EXIT_UTILS_OBJ_GROUP	:= $(SYSTEM_EXIT_UTILS_OBJ)		\
				   $(SYSTEM_FILE_UTILS_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS				+= $(SYSTEM_EXIT_UTILS_OBJ)



# thread/thread_utils
# ──────────────────────────────────────────────────────────────────────────────
THREAD_THREAD_UTILS_SRC 	:= $(call HELPER_SOURCE_PATH,thread thread_utils)
THREAD_THREAD_UTILS_HDR		:= $(call HELPER_HEADER_PATH,thread thread_utils)
THREAD_THREAD_UTILS_OBJ 	:= $(call HELPER_OBJECT_PATH,thread thread_utils)
# ─────────────── target prequisites ───────────────────────────────────────────
THREAD_THREAD_UTILS_OBJ_PREQS	:= $(THREAD_THREAD_UTILS_SRC)		\
			   	   $(THREAD_THREAD_UTILS_HDR)		\
			   	   $(TIME_TIME_UTILS_HDR)		\
			   	   $(UTILS_CLOSURE_HDR)			\
			   	   $(UTILS_FAIL_SWITCH_HDR)		\
			   	   $(UTILS_TYPES_BOOL_STATUS_HDR)
THREAD_THREAD_UTILS_OBJ_GROUP	:= $(THREAD_THREAD_UTILS_OBJ)		\
			   	   $(TIME_TIME_UTILS_OBJ_GROUP)		\
			   	   $(UTILS_CLOSURE_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS				+= $(THREAD_THREAD_UTILS_OBJ)



# thread/task_queue
# ──────────────────────────────────────────────────────────────────────────────
THREAD_TASK_QUEUE_SRC 		:= $(call HELPER_SOURCE_PATH,thread task_queue)
THREAD_TASK_QUEUE_HDR		:= $(call HELPER_HEADER_PATH,thread task_queue)
THREAD_TASK_QUEUE_OBJ 		:= $(call HELPER_OBJECT_PATH,thread task_queue)
# ─────────────── target prequisites ───────────────────────────────────────────
THREAD_TASK_QUEUE_OBJ_PREQS	:= $(THREAD_TASK_QUEUE_SRC) 		\
			   	   $(THREAD_TASK_QUEUE_HDR) 		\
			   	   $(THREAD_THREAD_UTILS_HDR)
THREAD_TASK_QUEUE_OBJ_GROUP	:= $(THREAD_TASK_QUEUE_OBJ)		\
				   $(THREAD_THREAD_UTILS_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS				+= $(THREAD_TASK_QUEUE_OBJ)



# thread/thread_log
# ──────────────────────────────────────────────────────────────────────────────
THREAD_THREAD_LOG_SRC 		:= $(call HELPER_SOURCE_PATH,thread thread_log)
THREAD_THREAD_LOG_HDR		:= $(call HELPER_HEADER_PATH,thread thread_log)
THREAD_THREAD_LOG_OBJ 		:= $(call HELPER_OBJECT_PATH,thread thread_log)
# ─────────────── target prequisites ───────────────────────────────────────────
THREAD_THREAD_LOG_OBJ_PREQS	:= $(THREAD_THREAD_LOG_SRC) 		\
			   	   $(THREAD_THREAD_LOG_HDR) 		\
			   	   $(THREAD_THREAD_UTILS_HDR)		\
			   	   $(STRING_STRING_UTILS_HDR)		\
			   	   $(SYSTEM_FILE_UTILS_HDR)
THREAD_THREAD_LOG_OBJ_GROUP	:= $(THREAD_THREAD_LOG_OBJ)		\
				   $(THREAD_THREAD_UTILS_OBJ_GROUP)	\
				   $(STRING_STRING_UTILS_OBJ_GROUP)	\
				   $(SYSTEM_FILE_UTILS_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS				+= $(THREAD_THREAD_LOG_OBJ)



# thread/thread_pool
# ──────────────────────────────────────────────────────────────────────────────
THREAD_THREAD_POOL_SRC 		:= $(call HELPER_SOURCE_PATH,thread thread_pool)
THREAD_THREAD_POOL_HDR		:= $(call HELPER_HEADER_PATH,thread thread_pool)
THREAD_THREAD_POOL_OBJ 		:= $(call HELPER_OBJECT_PATH,thread thread_pool)
# ─────────────── target prequisites ───────────────────────────────────────────
THREAD_THREAD_POOL_OBJ_PREQS	:= $(THREAD_THREAD_POOL_SRC) 		\
			   	   $(THREAD_THREAD_POOL_HDR) 		\
			   	   $(THREAD_THREAD_UTILS_HDR)		\
			   	   $(THREAD_THREAD_LOG_HDR)
THREAD_THREAD_POOL_OBJ_GROUP	:= $(THREAD_THREAD_POOL_OBJ)		\
				   $(THREAD_THREAD_LOG_OBJ_GROUP)	\
				   $(THREAD_TASK_QUEUE_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS				+= $(THREAD_THREAD_POOL_OBJ)



# uuid/uuid_utils
# ──────────────────────────────────────────────────────────────────────────────
UUID_UUID_UTILS_SRC 		:= $(call HELPER_SOURCE_PATH,uuid uuid_utils)
UUID_UUID_UTILS_HDR		:= $(call HELPER_HEADER_PATH,uuid uuid_utils)
UUID_UUID_UTILS_OBJ 		:= $(call HELPER_OBJECT_PATH,uuid uuid_utils)
# ─────────────── target prequisites ───────────────────────────────────────────
UUID_UUID_UTILS_OBJ_PREQS	:= $(UUID_UUID_UTILS_SRC) 		\
			   	   $(UUID_UUID_UTILS_HDR)		\
			   	   $(THREAD_THREAD_UTILS_HDR)		\
				   $(SYSTEM_FILE_UTILS_HDR)		\
				   $(RANDOM_RANDOM_HDR)			\
				   $(STRING_STRING_UTILS_HDR)
UUID_UUID_UTILS_OBJ_GROUP	:= $(UUID_UUID_UTILS_OBJ)		\
				   $(THREAD_THREAD_UTILS_OBJ_GROUP)	\
				   $(SYSTEM_FILE_UTILS_OBJ_GROUP)	\
				   $(RANDOM_RANDOM_OBJ_GROUP)		\
				   $(STRING_STRING_UTILS_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS				+= $(UUID_UUID_UTILS_OBJ)



# network/network_utils
# ──────────────────────────────────────────────────────────────────────────────
NETWORK_NETWORK_UTILS_SRC 	:= $(call HELPER_SOURCE_PATH,network network_utils)
NETWORK_NETWORK_UTILS_HDR	:= $(call HELPER_HEADER_PATH,network network_utils)
NETWORK_NETWORK_UTILS_OBJ 	:= $(call HELPER_OBJECT_PATH,network network_utils)
# ─────────────── target prequisites ───────────────────────────────────────────
NETWORK_NETWORK_UTILS_OBJ_PREQS	:= $(NETWORK_NETWORK_UTILS_SRC) 	\
			   	   $(NETWORK_NETWORK_UTILS_HDR)		\
			   	   $(SYSTEM_EXIT_UTILS_HDR)
NETWORK_NETWORK_UTILS_OBJ_GROUP	:= $(NETWORK_NETWORK_UTILS_OBJ)		\
				   $(SYSTEM_EXIT_UTILS_OBJ_GROUP)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS				+= $(NETWORK_NETWORK_UTILS_OBJ)


# endian-ness tester
# ──────────────────────────────────────────────────────────────────────────────
IS_LITTLE_ENDIAN_SRC 		:= $(call HELPER_SOURCE_PATH,network is_little_endian)
IS_LITTLE_ENDIAN_OBJ		:= $(call HELPER_OBJECT_PATH,network is_little_endian)
IS_LITTLE_ENDIAN_BIN		:= $(call BINARY_PATH,is_little_endian)
# ─────────────── target prequisites ───────────────────────────────────────────
IS_LITTLE_ENDIAN_BIN_PREQS	:= $(IS_LITTLE_ENDIAN_OBJ)
IS_LITTLE_ENDIAN_OBJ_PREQS	:= $(IS_LITTLE_ENDIAN_SRC)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS				+= $(IS_LITTLE_ENDIAN_OBJ)	\
				   $(IS_LITTLE_ENDIAN_BIN)


# timestamp_server
# ──────────────────────────────────────────────────────────────────────────────
TIMESTAMP_SERVER_SRC 		:= $(call HELPER_SOURCE_PATH,server timestamp_server)
TIMESTAMP_SERVER_OBJ		:= $(call HELPER_OBJECT_PATH,server timestamp_server)
TIMESTAMP_SERVER_BIN		:= $(call BINARY_PATH,timestamp_server)
# ─────────────── target prequisites ───────────────────────────────────────────
TIMESTAMP_SERVER_BIN_PREQS	:= $(TIMESTAMP_SERVER_OBJ)		\
				   $(TIME_TIME_STRING_OBJ_GROUP)	\
				   $(NETWORK_NETWORK_UTILS_OBJ_GROUP)
TIMESTAMP_SERVER_OBJ_PREQS	:= $(TIMESTAMP_SERVER_SRC)		\
				   $(TIME_TIME_STRING_HDR)		\
				   $(NETWORK_NETWORK_UTILS_HDR)		\
				   $(IS_LITTLE_ENDIAN_BIN)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS				+= $(TIMESTAMP_SERVER_OBJ)		\
				   $(TIMESTAMP_SERVER_BIN)

# timestamp_client
# ──────────────────────────────────────────────────────────────────────────────
TIMESTAMP_CLIENT_SRC 		:= $(call HELPER_SOURCE_PATH,client timestamp_client)
TIMESTAMP_CLIENT_OBJ		:= $(call HELPER_OBJECT_PATH,client timestamp_client)
TIMESTAMP_CLIENT_BIN		:= $(call BINARY_PATH,timestamp_client)
# ─────────────── target prequisites ───────────────────────────────────────────
TIMESTAMP_CLIENT_BIN_PREQS	:= $(TIMESTAMP_CLIENT_OBJ)		\
				   $(TIME_TIME_STRING_OBJ_GROUP)	\
				   $(NETWORK_NETWORK_UTILS_OBJ_GROUP)
TIMESTAMP_CLIENT_OBJ_PREQS	:= $(TIMESTAMP_CLIENT_SRC)		\
				   $(TIME_TIME_STRING_HDR)		\
				   $(NETWORK_NETWORK_UTILS_HDR)		\
				   $(IS_LITTLE_ENDIAN_BIN)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS				+= $(TIMESTAMP_CLIENT_OBJ)		\
				   $(TIMESTAMP_CLIENT_BIN)

# sendmail_server
# ──────────────────────────────────────────────────────────────────────────────
SENDMAIL_SERVER_SRC 		:= $(call HELPER_SOURCE_PATH,server sendmail_server)
SENDMAIL_SERVER_OBJ		:= $(call HELPER_OBJECT_PATH,server sendmail_server)
SENDMAIL_SERVER_BIN		:= $(call BINARY_PATH,sendmail_server)
# ─────────────── target prequisites ───────────────────────────────────────────
SENDMAIL_SERVER_BIN_PREQS	:= $(SENDMAIL_SERVER_OBJ)		\
				   $(NETWORK_NETWORK_UTILS_OBJ_GROUP)
SENDMAIL_SERVER_OBJ_PREQS	:= $(SENDMAIL_SERVER_SRC)		\
				   $(NETWORK_NETWORK_UTILS_HDR)		\
				   $(IS_LITTLE_ENDIAN_BIN)
# ─────────────── targets ──────────────────────────────────────────────────────
TARGETS				+= $(SENDMAIL_SERVER_OBJ)		\
				   $(SENDMAIL_SERVER_BIN)


# ══════════════════════════════════════════════════════════════════════════════
# ──────────────────────────────────────────────────────────────────────────────
# 				MAKE RULES
# ──────────────────────────────────────────────────────────────────────────────
# ══════════════════════════════════════════════════════════════════════════════
#
#
## COMPOSITE RULES
# ══════════════════════════════════════════════════════════════════════════════
# make all targets
# ──────────────────────────────────────────────────────────────────────────────
all: $(TARGETS)

# remove all targets
# ──────────────────────────────────────────────────────────────────────────────
clean:
	$(RM) $(RM_FLAGS) $(TARGETS)

# set endianess environment variable
# ──────────────────────────────────────────────────────────────────────────────
is_little_endian: $(IS_LITTLE_ENDIAN_BIN)
	$(eval IS_LITTLE_ENDIAN := $(shell $(IS_LITTLE_ENDIAN_BIN)))

# make client/timestamp_client binary
# ──────────────────────────────────────────────────────────────────────────────
$(TIMESTAMP_CLIENT_BIN): $(TIMESTAMP_CLIENT_BIN_PREQS)
	$(LD) $(LD_FLAGS) $^ $(LD_LIB_FLAGS) -o $@

# make client/timestamp_client object
# ──────────────────────────────────────────────────────────────────────────────
$(TIMESTAMP_CLIENT_OBJ): $(TIMESTAMP_CLIENT_OBJ_PREQS) is_little_endian
	$(CC) $(CC_FLAGS) -DIS_LITTLE_ENDIAN=$(IS_LITTLE_ENDIAN) -c $< -o $@

# make server/timestamp_server binary
# ──────────────────────────────────────────────────────────────────────────────
$(TIMESTAMP_SERVER_BIN): $(TIMESTAMP_SERVER_BIN_PREQS)
	$(LD) $(LD_FLAGS) $^ $(LD_LIB_FLAGS) -o $@

# make server/timestamp_server object
# ──────────────────────────────────────────────────────────────────────────────
$(TIMESTAMP_SERVER_OBJ): $(TIMESTAMP_SERVER_OBJ_PREQS) is_little_endian
	$(CC) $(CC_FLAGS) -DIS_LITTLE_ENDIAN=$(IS_LITTLE_ENDIAN) -c $< -o $@

# make server/sendmail_server binary
# ──────────────────────────────────────────────────────────────────────────────
$(SENDMAIL_SERVER_BIN): $(SENDMAIL_SERVER_BIN_PREQS)
	$(LD) $(LD_FLAGS) $^ $(LD_LIB_FLAGS) -o $@

# make server/sendmail_server object
# ──────────────────────────────────────────────────────────────────────────────
$(SENDMAIL_SERVER_OBJ): $(SENDMAIL_SERVER_OBJ_PREQS) is_little_endian
	$(CC) $(CC_FLAGS) -DIS_LITTLE_ENDIAN=$(IS_LITTLE_ENDIAN) -c $< -o $@

# make network/network_utils object
# ──────────────────────────────────────────────────────────────────────────────
$(NETWORK_NETWORK_UTILS_OBJ): $(NETWORK_NETWORK_UTILS_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -c $< -o $@

# make thread/thread_pool object
# ──────────────────────────────────────────────────────────────────────────────
$(THREAD_THREAD_POOL_OBJ): $(THREAD_THREAD_POOL_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -c $< -o $@

# make thread/thread_log object
# ──────────────────────────────────────────────────────────────────────────────
$(THREAD_THREAD_LOG_OBJ): $(THREAD_THREAD_LOG_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -c $< -o $@

# make thread/task_queue object
# ──────────────────────────────────────────────────────────────────────────────
$(THREAD_TASK_QUEUE_OBJ): $(THREAD_TASK_QUEUE_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -c $< -o $@

# make thread/thread_utils object
# ──────────────────────────────────────────────────────────────────────────────
$(THREAD_THREAD_UTILS_OBJ): $(THREAD_THREAD_UTILS_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -c $< -o $@

# make system/system_utils object
# ──────────────────────────────────────────────────────────────────────────────
$(SYSTEM_SYSTEM_UTILS_OBJ): $(SYSTEM_SYSTEM_UTILS_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -c $< -o $@

# make system/file_utils object
# ──────────────────────────────────────────────────────────────────────────────
$(SYSTEM_FILE_UTILS_OBJ): $(SYSTEM_FILE_UTILS_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -c $< -o $@

# make system/stdio_utils object
# ──────────────────────────────────────────────────────────────────────────────
$(SYSTEM_STDIO_UTILS_OBJ): $(SYSTEM_STDIO_UTILS_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -c $< -o $@

# make system/exit_utils object
# ──────────────────────────────────────────────────────────────────────────────
$(SYSTEM_EXIT_UTILS_OBJ): $(SYSTEM_EXIT_UTILS_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -c $< -o $@

# make uuid/uuid_utils object
# ──────────────────────────────────────────────────────────────────────────────
$(UUID_UUID_UTILS_OBJ): $(UUID_UUID_UTILS_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -c $< -o $@

# make time/time_string object
# ──────────────────────────────────────────────────────────────────────────────
$(TIME_TIME_STRING_OBJ): $(TIME_TIME_STRING_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -c $< -o $@

# make string/string_utils object
# ──────────────────────────────────────────────────────────────────────────────
$(STRING_STRING_UTILS_OBJ): $(STRING_STRING_UTILS_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -c $< -o $@

# make string/ascii_utils object
# ──────────────────────────────────────────────────────────────────────────────
$(STRING_ASCII_UTILS_OBJ): $(STRING_ASCII_UTILS_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -c $< -o $@

# make string/utf8_utils object
# ──────────────────────────────────────────────────────────────────────────────
$(STRING_UTF8_UTILS_OBJ): $(STRING_UTF8_UTILS_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -c $< -o $@

# make random/random_generator object
# ──────────────────────────────────────────────────────────────────────────────
$(RANDOM_RANDOM_GENERATOR_OBJ): $(RANDOM_RANDOM_GENERATOR_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -c $< -o $@

# make random/random object
# ──────────────────────────────────────────────────────────────────────────────
$(RANDOM_RANDOM_OBJ): $(RANDOM_RANDOM_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -c $< -o $@

# make random/pcg_random object
# ──────────────────────────────────────────────────────────────────────────────
$(RANDOM_PCG_RANDOM_OBJ): $(RANDOM_PCG_RANDOM_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -c $< -o $@

# make memory/memory_put_array object
# ──────────────────────────────────────────────────────────────────────────────
$(MEMORY_MEMORY_PUT_ARRAY_OBJ): $(MEMORY_MEMORY_PUT_ARRAY_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -c $< -o $@

# make memory/memory_copy_array object
# ──────────────────────────────────────────────────────────────────────────────
$(MEMORY_MEMORY_COPY_ARRAY_OBJ): $(MEMORY_MEMORY_COPY_ARRAY_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -c $< -o $@

# make memory/memory_swap object
# ──────────────────────────────────────────────────────────────────────────────
$(MEMORY_MEMORY_SWAP_OBJ): $(MEMORY_MEMORY_SWAP_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -c $< -o $@

# make memory/memory_zero object
# ──────────────────────────────────────────────────────────────────────────────
$(MEMORY_MEMORY_ZERO_OBJ): $(MEMORY_MEMORY_ZERO_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -c $< -o $@

# make memory/memory_utils object
# ──────────────────────────────────────────────────────────────────────────────
$(MEMORY_MEMORY_UTILS_OBJ): $(MEMORY_MEMORY_UTILS_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -c $< -o $@

# make bitwise/bitwise_utils object
# ──────────────────────────────────────────────────────────────────────────────
$(BITWISE_BITWISE_UTILS_OBJ): $(BITWISE_BITWISE_UTILS_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -c $< -o $@

# make time/time_utils object
# ──────────────────────────────────────────────────────────────────────────────
$(TIME_TIME_UTILS_OBJ): $(TIME_TIME_UTILS_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -c $< -o $@

# make utils/uint128_ops object
# ──────────────────────────────────────────────────────────────────────────────
$(UTILS_UINT128_OPS_OBJ): $(UTILS_UINT128_OPS_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -c $< -o $@

# make utils/closure object
# ──────────────────────────────────────────────────────────────────────────────
$(UTILS_CLOSURE_OBJ): $(UTILS_CLOSURE_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -c $< -o $@

# make utils/utils object
# ──────────────────────────────────────────────────────────────────────────────
$(UTILS_UTILS_OBJ): $(UTILS_UTILS_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -c $< -o $@

# make network/is_little_endian binary
# ──────────────────────────────────────────────────────────────────────────────
$(IS_LITTLE_ENDIAN_BIN): $(IS_LITTLE_ENDIAN_BIN_PREQS)
	$(LD) $(LD_FLAGS) $^ $(LD_LIB_FLAGS) -o $@

# make network/is_little_endian object
# ──────────────────────────────────────────────────────────────────────────────
$(IS_LITTLE_ENDIAN_OBJ): $(IS_LITTLE_ENDIAN_OBJ_PREQS)
	$(CC) $(CC_FLAGS) -c $< -o $@
