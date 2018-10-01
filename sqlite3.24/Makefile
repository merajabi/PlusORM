SUBDIRS:= 
INC    := inc $(SUBDIRS)
SRC    := src
TARGET	:= sqlite3.24
LIBNAMES   := 

$(warning COMPILER: $(ARCHITECTURE))
-include $(ARCHITECTURE)

CFLAG   += -DSQLITE_THREADSAFE=1
CXXFLAG += -DSQLITE_THREADSAFE=1

include $(MANAGER)

