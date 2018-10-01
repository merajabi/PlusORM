#!/bin/sh
TARGET="X86";
ROOTFOLDER=$(pwd);
if [ ! -z "$1" ]
then
	TARGET=$1;
fi

if [ "$TARGET" = "X86" ]
then
	make dynamiclib ARCHITECTURE="$ROOTFOLDER/x86.mk" MANAGER="$ROOTFOLDER/makefile.mk" BUILD="sqlite3.24"; > out.txt 2>&1
	cat out.txt | grep --color -E "error:"
elif [ "$TARGET" = "clean" ]
then
	make cleanall MANAGER="$ROOTFOLDER/makefile.mk" BUILD="sqlite3.24" ;
	rm test/*.so test/*.a test/*.o test/*.out
elif [ "$TARGET" = "test" ]
then
	cd ./test
	./build-tests.sh
	cd ..
else
	make dynamiclib ARCHITECTURE="$ROOTFOLDER/$TARGET.mk" MANAGER="$ROOTFOLDER/makefile.mk" BUILD="sqlite3.24"; > out.txt 2>&1
	cat out.txt | grep --color -E "error:"
fi
