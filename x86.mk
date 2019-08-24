
ARCH=x86
CROSS_COMPILE=

TOOLCHAIN		:= /usr/bin
ARCHPREFIX      := $(TOOLCHAIN)/
SDKPATH			:= 

# Linker flags, also tell the linker to ignore functions that aren't used.
LIBPATH			:= 
LIBS			:= -lpthread -ldl
INCPATH 		:= -isystem /usr/include/libdrm

# unix fs parms
CP				:= cp
MV              := mv
RM				:= rm
TESTDIR			:= test -d
MKDIR           := mkdir

CC              := "$(ARCHPREFIX)gcc"
CXX             := "$(ARCHPREFIX)g++"
LINK            := "$(ARCHPREFIX)g++"
LINK_SHLIB      := "$(ARCHPREFIX)g++"
AR              := "$(ARCHPREFIX)ar"
OBJCOPY         := "$(ARCHPREFIX)objcopy"
STRIP           := "$(ARCHPREFIX)strip"
AS				:= "$(ARCHPREFIX)as"
LD				:= "$(ARCHPREFIX)g++"

# Using the Thumb Instruction Set, The CPU Variant, Which floating point ABI to use, The type of FPU we are using
ARCHFLAGS 		:= 
# -g -O0 -Wall
# -Os -DNDEBUG
OPTIMISATION 	:= -g -O0 -Wall


# Compiler flags
# -DPOS_DEBUG -DHWX86
CFLAG			:= -MMD $(OPTIMISATION) $(ARCHFLAGS)
CXXFLAG			:= -MMD $(OPTIMISATION) $(ARCHFLAGS)
LDFLAG			:= $(OPTIMISATION) $(ARCHFLAGS)
LDLIBFLAG		:= -Wl,-rpath,.:./lib:..:../lib

