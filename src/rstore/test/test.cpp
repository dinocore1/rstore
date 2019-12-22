
#include <rollsum.h>

#include <gtest/gtest.h>

TEST(rollsum, init) {
    Rollsum sum;
    RollsumInit(&sum);
    EXPECT_EQ(0, RollsumDigest(&sum));
}