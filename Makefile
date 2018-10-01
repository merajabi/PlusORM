SUBDIRS:= sqlite3.24 utils
INC    := inc ./ $(SUBDIRS)
SRC    := src
TARGET	:= plusorm
LIBNAMES   := 
$(warning COMPILER: $(ARCHITECTURE))
-include $(ARCHITECTURE)
include $(MANAGER)

