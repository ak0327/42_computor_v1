#include "TestParser.hpp"
#include <cmath>
#include <limits>
#include "gtest/gtest.h"

TEST(TestLib, TestSqrt) {
    double num;

    for (int n = 0; n <= 100000; ++n) {
        EXPECT_DOUBLE_EQ(std::sqrt(n), Computor::sqrt(n));
    }

    num = 0.1;
    EXPECT_DOUBLE_EQ(std::sqrt(num), Computor::sqrt(num));

    num = 0.01;
    EXPECT_DOUBLE_EQ(std::sqrt(num), Computor::sqrt(num));

    num = 0.001;
    EXPECT_DOUBLE_EQ(std::sqrt(num), Computor::sqrt(num));

    num = std::numeric_limits<double>::min();
    EXPECT_DOUBLE_EQ(std::sqrt(num), Computor::sqrt(num));

    num = std::numeric_limits<int32_t>::max();
    EXPECT_DOUBLE_EQ(std::sqrt(num), Computor::sqrt(num));

    num = std::numeric_limits<double>::max();
    EXPECT_DOUBLE_EQ(std::sqrt(num), Computor::sqrt(num));

    ASSERT_THROW(Computor::sqrt(-1), std::domain_error);
    ASSERT_THROW(Computor::sqrt(-0.0001), std::domain_error);
    ASSERT_THROW(Computor::sqrt(-std::numeric_limits<double>::min()), std::domain_error);
    ASSERT_THROW(Computor::sqrt(std::numeric_limits<int32_t>::lowest()), std::domain_error);
    ASSERT_THROW(Computor::sqrt(std::numeric_limits<double>::quiet_NaN()), std::domain_error);
    ASSERT_THROW(Computor::sqrt(std::numeric_limits<double>::infinity()), std::domain_error);
    ASSERT_THROW(Computor::sqrt(std::numeric_limits<double>::lowest()), std::domain_error);
}

TEST(TestLib, TestIsNan) {
    double num;

    num = 0;
    EXPECT_EQ(std::isnan(num), Computor::isnan(num));

    num = 0.1;
    EXPECT_EQ(std::isnan(num), Computor::isnan(num));

    num = 1;
    EXPECT_EQ(std::isnan(num), Computor::isnan(num));

    num = 100;
    EXPECT_EQ(std::isnan(num), Computor::isnan(num));

    num = std::numeric_limits<double>::max();
    EXPECT_EQ(std::isnan(num), Computor::isnan(num));

    num = std::numeric_limits<double>::infinity();
    EXPECT_EQ(std::isnan(num), Computor::isnan(num));

    num = -std::numeric_limits<double>::infinity();
    EXPECT_EQ(std::isnan(num), Computor::isnan(num));

    num = std::numeric_limits<double>::quiet_NaN();
    EXPECT_EQ(std::isnan(num), Computor::isnan(num));
}

TEST(TestLib, TestIsInf) {
    double num;

    num = 0;
    EXPECT_EQ(std::isnan(num), Computor::isnan(num));

    num = 0.1;
    EXPECT_EQ(std::isnan(num), Computor::isnan(num));

    num = 1;
    EXPECT_EQ(std::isnan(num), Computor::isnan(num));

    num = 100;
    EXPECT_EQ(std::isnan(num), Computor::isnan(num));

    num = std::numeric_limits<double>::max();
    EXPECT_EQ(std::isnan(num), Computor::isnan(num));

    num = std::numeric_limits<double>::infinity();
    EXPECT_EQ(std::isnan(num), Computor::isnan(num));

    num = -std::numeric_limits<double>::infinity();
    EXPECT_EQ(std::isnan(num), Computor::isnan(num));

    num = std::numeric_limits<double>::quiet_NaN();
    EXPECT_EQ(std::isnan(num), Computor::isnan(num));
}
