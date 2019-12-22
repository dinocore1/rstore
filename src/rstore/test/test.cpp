
#include <rollsum.h>

#include <gtest/gtest.h>

TEST(rollsum, init) {
    Rollsum sum;
    RollsumInit(&sum);
    EXPECT_EQ(0, RollsumDigest(&sum));
    EXPECT_EQ(0, sum.count);
}

TEST(rollsum, rollin) {
    Rollsum sum;
    RollsumInit(&sum);

    RollsumRollin(&sum, 0);
}