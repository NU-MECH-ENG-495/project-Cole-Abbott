#include <gtest/gtest.h>
#include "first_project/header1.h"

using namespace first_project;

TEST(StringTest, BasicTests) {
    EXPECT_TRUE(are_strings_equal("Hello, World!", "Hello, World!"));
    EXPECT_TRUE(are_strings_equal("", ""));  // Empty strings

    // Different strings should return false
    EXPECT_FALSE(are_strings_equal("Hello, World!", "Hello, world!"));  // Different case
    EXPECT_FALSE(are_strings_equal("Hello", "Hello!"));  // Different length
    EXPECT_FALSE(are_strings_equal("", "Hello"));  // Empty and non-empty
}

TEST(StringTest, SpecialCharacters) {
    EXPECT_TRUE(are_strings_equal("!@#$%^&*()", "!@#$%^&*()"));
    EXPECT_FALSE(are_strings_equal("!@#$%^&*()", "!@#$%^&*"));
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
