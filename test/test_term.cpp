#include "gtest.h"
#include "term.h"

TEST(Term, can_create_term)
{
	ASSERT_NO_THROW(term a(types::operand_));
}
TEST(Term, can_get_type_term)
{
	term a(types::operand_);
	EXPECT_EQ(types::operand_, a.getType());
}
TEST(Term, can_create_operand)
{
	ASSERT_NO_THROW(operand a(10));
}
TEST(Term, can_create_operation)
{
	ASSERT_NO_THROW(operation a('+'));
}
TEST(Term, can_get_operation)
{
	operation a('+');
	EXPECT_EQ('+', a.getOperation());
}
TEST(Term, can_get_operation_priority)
{
	operation a('+');
	EXPECT_EQ(1, a.getPriority());
}
TEST(Term, can_create_open_bracket)
{
	ASSERT_NO_THROW(open_bracket a);
}
TEST(Term, can_create_close_bracket)
{
	ASSERT_NO_THROW(close_bracket a);
}