#!/bin/sh
TARGET="X86";
ROOTFOLDER=$(pwd);
EXITCODE=1

if [ ! -z "$1" ]
then
	TARGET=$1;
fi

if [ "$TARGET" = "X86" ]
then
	make dynamiclib ARCHITECTURE="$ROOTFOLDER/x86.mk" MANAGER="$ROOTFOLDER/makefile.mk" BUILD="sqlite3.24"; > out.txt 2>&1
	EXITCODE=$?
	cat out.txt | grep --color -E "error:"
elif [ "$TARGET" = "clean" ]
then
	make cleanall MANAGER="$ROOTFOLDER/makefile.mk" BUILD="sqlite3.24" ;
	EXITCODE=$?
	rm test/*.so test/*.a test/*.o test/*.db test/*.out test/*.gcda test/*.gcno test/*.trace test/*.dsc test/*.xml
	rm -r test/cov-report
elif [ "$TARGET" = "test" ]
then
	cd ./test
	./build-tests.sh
	EXITCODE=$?
	cd ..
else
	make dynamiclib ARCHITECTURE="$ROOTFOLDER/$TARGET.mk" MANAGER="$ROOTFOLDER/makefile.mk" BUILD="sqlite3.24"; > out.txt 2>&1
	EXITCODE=$?
	cat out.txt | grep --color -E "error:"
fi

exit $EXITCODE;
