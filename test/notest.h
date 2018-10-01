#include <iostream>

#define ASSERT_EQ(x,y) std::cout << __LINE__ << "\t" << toString(x) << " == " << toString(y) << std::endl
#define ASSERT_NE(x,y) std::cout << __LINE__ << "\t" << toString(x) << " != " << toString(y) <<  std::endl

#define EXPECT_EQ(x,y) std::cout << __LINE__ << "\t" << toString(x) << " == " << toString(y) <<  std::endl
#define EXPECT_NE(x,y) std::cout << __LINE__ << "\t" << toString(x) << " != " << toString(y) <<  std::endl

#define ASSERT_TRUE(x) x
#define ASSERT_FALSE(x) x

#define EXPECT_TRUE(x) x
#define EXPECT_FALSE(x) x

// x::y()
#define TEST_F(x,y) void y()   

#define RUN_ALL_TESTS() 
