GTEST_DIR="../googletest/googletest";
GMOCK_DIR="../googletest/googlemock";

# googletest 1.8.x is the last release supporting pre-C++11 compilers.
# cd googletest; git checkout 2fe3bd9 
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

#g++ -isystem /usr/include/libdrm -I. -I.. -I${GTEST_DIR}/include -g -O0 -Wall  -fPIC -c gtest1.cpp -o gtest1.o
#g++ -g -O0 -Wall  -o test.d gtest1.o  -L. -L.. -Wl,-rpath,../ -Wl,--start-group -lorm -lutils -lsqlite3.24 -lgtest -lpthread -ldl  -Wl,--end-group

# ../script/GendbInterface.pl < person.sql > person.h
# g++ -I. -I.. -I${GTEST_DIR}/include -g -Wall -fPIC -std=c++98 gtest1.cpp -o gtest1.out -L. -L.. -Wl,-rpath,.:.. -Wl,--start-group -lutils -lsqlite3.24 -lplusorm -lgtest -lpthread -ldl  -Wl,--end-group

#for Cygwin use this line
#cp ../*.so ./
#g++ -I. -I.. -I${GTEST_DIR}/include -g -O0 -Wall -fPIC gtest1.cpp -o gtest1 -L/cygdrive/d/POS/orm -L. -L.. -Wl,-rpath,../ -Wl,--start-group libplusorm.so libutils.so libsqlite3.24.so libgtest.a -lpthread -ldl  -Wl,--end-group

# ./gtest1.out
#valgrind -v --leak-check=full --show-leak-kinds=all test/gtest1
#test/gtest1


g++ -I. -I.. -I../sqlite3.24/inc -I${GTEST_DIR}/include -g -O0 -Wall -fPIC -std=c++98 test2.cpp -o test2.out -L. -L.. -Wl,-rpath,.:.. -Wl,--start-group -lutils -lsqlite3.24 -lgtest -lpthread -ldl  -Wl,--end-group

g++ -I. -I.. -I${GTEST_DIR}/include -g -Wall -fPIC -std=c++98 test1.cpp -o test1.out -L. -L.. -Wl,-rpath,.:.. -Wl,--start-group -lplusorm -lutils -lsqlite3.24 -lgtest -lpthread -ldl  -Wl,--end-group

