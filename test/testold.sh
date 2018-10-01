rm *.o test/gtest1 test/gtest0 test/test0

GTEST_DIR="googletest/googletest";
GMOCK_DIR="googletest/googlemock";

#${GMOCK_DIR}/scripts/generator/gmock_gen.py turtle.h

if [ ! -f ./libgtest.a ] 
then
	g++ -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o
fi

if [ ! -f ./libgmock.a ] 
then
	g++ -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -isystem ${GMOCK_DIR}/include -I${GMOCK_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
	g++ -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -isystem ${GMOCK_DIR}/include -I${GMOCK_DIR} -pthread -c ${GMOCK_DIR}/src/gmock-all.cc
	ar -rv libgmock.a gtest-all.o gmock-all.o
fi

gcc -I./sqlite3.24 -pthread -c sqlite3.24/sqlite3.c -o obj/sqlite3.o
gcc -I./sqlite3.24 -pthread -c sqlite3.24/shell.c -o obj/shell.o
gcc -pthread obj/sqlite3.o obj/shell.o -ldl -o sqlite3

g++ -g -I. -Iinc -I../inc -I./sqlite3.24 -c src/database.cpp -o obj/database.o
g++ -g -I. -Iinc -I../inc -I./sqlite3.24 -c src/orm.cpp		-o obj/orm.o
g++ -g -I. -Iinc -I../inc -I./sqlite3.24 -c src/configuration.cpp -o obj/configuration.o

# Simple test
#g++ -g -I. -I./sqlite3.24 -pthread test/test0.cpp obj/database.o obj/configuration.o obj/orm.o obj/sqlite3.o -ldl -o test/test0
#valgrind -v --leak-check=full --show-leak-kinds=all test/test0
#test/test0

# google test
g++ -g -std=gnu++11 -I. -Iinc -I../inc -I./sqlite3.24 -isystem ${GTEST_DIR}/include -pthread test/gtest1.cpp obj/database.o obj/configuration.o obj/orm.o obj/sqlite3.o -ldl libgtest.a -o test/gtest1
#g++ -g -std=c++11 -I. -isystem ${GTEST_DIR}/include -isystem ${GMOCK_DIR}/include -pthread test/gmock0.cpp resource.o libgmock.a -o test/gmock0

valgrind -v --leak-check=full --show-leak-kinds=all test/gtest1
test/gtest1


#gcc -g -I. -isystem ${GTEST_DIR}/include -pthread test/gtest1.c resource.o libgtest.a -o test/gtest1
#valgrind -v --leak-check=full --show-leak-kinds=all test/gtest1
#test/gtest1

