#include "linal.h"
#include "gtest/gtest.h"

using linal::Vector;


TEST(VectorTest, InitializeTest) {
  Vector<int> first(1, 2, 3);
  EXPECT_EQ(first.x(), 1);
  EXPECT_EQ(first.y(), 2);
  EXPECT_EQ(first.z(), 3);
}

TEST(VectorTest, EqualityTest) {
  Vector<int> first(1, 0, 0);
  Vector<int> first_equal(1, 0, 0);
  Vector<int> second(0, 1, 0);

  EXPECT_EQ(first, first_equal);
  // Tests != operator rather than == operator in test above.
  EXPECT_FALSE(first != first_equal);
  EXPECT_NE(first, second);
  EXPECT_TRUE(first != second);

}
