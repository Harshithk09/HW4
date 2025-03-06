#include <gtest/gtest.h>

#include "multiset.h"

TEST(Multiset, Empty) {
    Multiset<int> mset;

    /* Should be fully empty */
    EXPECT_EQ(mset.Empty(), true);
    EXPECT_EQ(mset.Size(), 0);
    EXPECT_THROW(mset.Count(42), std::exception);
}

TEST(Multiset, OneKey) {
  Multiset<int> mset;

  /* Test some insertion */
  mset.Insert(23);
  mset.Insert(42);
  mset.Insert(42);
  EXPECT_EQ(mset.Empty(), false);
  EXPECT_EQ(mset.Size(), 3);
  EXPECT_EQ(mset.Min(), 23);
  EXPECT_EQ(mset.Max(), 42);
  EXPECT_EQ(mset.Count(42), 2);
}

TEST(MultisetTest, InvalidKey) {
    Multiset ms;
    ms.insert(5);
    ms.insert(3);
    ms.insert(7);

    EXPECT_THROW(ms.floor(2), std::invalid_argument);
    EXPECT_THROW(ms.ceil(8), std::invalid_argument);
}

TEST(MultisetTest, Floor) {
    Multiset ms;
    ms.insert(5);
    ms.insert(3);
    ms.insert(7);
    ms.insert(3);  // Duplicate key

    EXPECT_EQ(ms.floor(4), 3);
    EXPECT_EQ(ms.floor(5), 5);
    EXPECT_EQ(ms.floor(6), 5);
    EXPECT_EQ(ms.floor(7), 7);
    EXPECT_THROW(ms.floor(2), std::invalid_argument);  // No key <= 2
}

TEST(MultisetTest, Ceil) {
    Multiset ms;
    ms.insert(5);
    ms.insert(3);
    ms.insert(7);
    ms.insert(3);  // Duplicate key

    EXPECT_EQ(ms.ceil(4), 5);
    EXPECT_EQ(ms.ceil(5), 5);
    EXPECT_EQ(ms.ceil(6), 7);
    EXPECT_EQ(ms.ceil(7), 7);
    EXPECT_THROW(ms.ceil(8), std::invalid_argument);  // No key >= 8
}


int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

