TGS    :=$(TARGET).s
TGD    :=$(TARGET).d
STATIC := liba
DYNAMIC:= libso
TARGETLIB := lib$(TARGET)
TGSL := $(TARGETLIB).a
TGDL := $(TARGETLIB).so
LIBTARGETSA = $(addprefix lib,$(addsuffix .a,$(SUBDIRS)))
LIBTARGETSA += $(addprefix lib,$(addsuffix .a,$(LIBNAMES)))
LIBTARGETSO = $(addprefix lib,$(addsuffix .so,$(SUBDIRS)))
LIBTARGETSO += $(addprefix lib,$(addsuffix .so,$(LIBNAMES)))
STATICTARGETS:= $(addsuffix .smake,$(SUBDIRS))
DYNAMICTARGETS:= $(addsuffix .dmake,$(SUBDIRS))
CLEANTARGETS:= $(addsuffix .clean,$(SUBDIRS))
INCPATH += -I. $(addprefix -I,$(INC)) 
LIBPATH += -L. $(addprefix -L,$(SUBDIRS))
LIBS += $(addprefix -l,$(LIBNAMES))
LIBS += -Wl,--start-group $(addprefix -l,$(SUBDIRS)) -Wl,--end-group
# $(wildcard **/*.cpp)
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
unique = $(if $1,$(firstword $1) $(call unique,$(filter-out $(firstword $1),$1)))
SRCC := $(call rwildcard,$(SRC)/,*.c)
SRCX := $(call rwildcard,$(SRC)/,*.cpp)
OBJCS = $(patsubst $(SRC)/%.c,$(STATIC)/%.o,$(SRCC))
OBJXS = $(patsubst $(SRC)/%.cpp,$(STATIC)/%.o,$(SRCX))
OBJCD = $(patsubst $(SRC)/%.c,$(DYNAMIC)/%.o,$(SRCC))
OBJXD = $(patsubst $(SRC)/%.cpp,$(DYNAMIC)/%.o,$(SRCX))
DEPCS = $(patsubst %.o,%.d,$(OBJCS))
DEPXS = $(patsubst %.o,%.d,$(OBJXS))
DEPCD = $(patsubst %.o,%.d,$(OBJCD))
DEPXD = $(patsubst %.o,%.d,$(OBJXD))
OBJSS  = $(OBJCS) 
OBJSS += $(OBJXS) 
DEPSS  = $(DEPCS) 
DEPSS += $(DEPXS)
OBJDS  = $(OBJCD) 
OBJDS += $(OBJXD) 
DEPDS  = $(DEPCD) 
DEPDS += $(DEPXD)
#SRCTREE:=$(shell find $(SRC) -type d )
STREE:=$(call unique,$(dir $(OBJSS)))
DTREE:=$(call unique,$(dir $(OBJDS)))

.PHONY: clean cleanall prepare all dynamic static alllib dynamiclib staticlib
default: $(DYNAMICTARGETS) prepare $(TGD)
all: dynamic static
dynamic: $(DYNAMICTARGETS) prepare $(TGD)
static: $(STATICTARGETS) prepare $(TGS)
alllib: dynamiclib staticlib
dynamiclib: $(DYNAMICTARGETS) prepare $(TGDL)
staticlib: $(STATICTARGETS) prepare $(TGSL)
cleanall: $(CLEANTARGETS) clean

#$(TARGET): $(TGS) $(TGD)

$(TGS): $(OBJSS)
	$(LD) $(LDFLAG) -o $@ $^ $(LIBPATH) $(LIBTARGETSA)

$(TGD): $(OBJDS)
	$(LD) $(LDFLAG) -o $@ $^ $(LIBPATH) $(LDLIBFLAG) $(LIBS)  

#$(TARGETLIB): $(STATICTARGET) $(DYNAMICTARGET)

$(TGSL): $(OBJSS)
	$(AR) rcs $@ $^
	$(CP) -rf *.a  ../

$(TGDL): $(OBJDS)
	$(LD) -shared -o $@ $^ $(LIBPATH) $(LDLIBFLAG) $(LIBS)
	$(CP) -f *.so ../

$(STATIC)/%.o: $(SRC)/%.c
	$(CC)  $(INCPATH) $(CFLAG)    -c $< -o $@

$(STATIC)/%.o: $(SRC)/%.cpp
	$(CXX) $(INCPATH) $(CXXFLAG)  -c $< -o $@

$(DYNAMIC)/%.o: $(SRC)/%.c
	$(CC)  $(INCPATH) $(CFLAG)  -fPIC  -c $< -o $@

$(DYNAMIC)/%.o: $(SRC)/%.cpp
	$(CXX) $(INCPATH) $(CXXFLAG) -fPIC -c $< -o $@

%.dmake: %
	$(MAKE) dynamiclib ARCHITECTURE=$(ARCHITECTURE) MANAGER=$(MANAGER) -C $<

%.smake: %
	$(MAKE) staticlib ARCHITECTURE=$(ARCHITECTURE) MANAGER=$(MANAGER) -C $<

%.clean: %
	$(MAKE) cleanall ARCHITECTURE=$(ARCHITECTURE) MANAGER=$(MANAGER) -C $<

clean: 
	rm -f $(OBJSS) $(DEPSS) $(OBJDS) $(DEPDS) $(TGS) $(TGD) $(TGSL) $(TGDL) *.a *.so
	rm -rf $(STATIC) $(DYNAMIC)
#	find ./ -iname "*.a" -delete
#	find ./ -iname "*.so" -delete

prepare:
	-mkdir -p $(STATIC) $(DYNAMIC) $(STREE) $(DTREE)

# include all dep files in the makefile
#-include $(OBJS:.o=.d)  
-include $(DEPS)  

