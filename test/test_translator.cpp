#include "gtest.h"
#include "translator.h"

TEST(Translator, can_create_arithmetic_expression)
{
	std::string s = "21+(3+5*7-16/2)*3";
	ASSERT_NO_THROW(arithmetic_expression s1(s));
}
TEST(Translator, can_unarniy_minus)
{
	std::string s = "(-7)+5";
	arithmetic_expression s1(s);
	s1.expression_to_terms();
	s1.terms_to_polish_entry();
	s1.polish_entry_to_solution();
	double a = s1.getSolution();
	EXPECT_EQ(-2, a);
}
TEST(Translator, can_count_sin)
{
	std::string s = "1+2*(3-sin(0))-4";
	arithmetic_expression s1(s);
	s1.expression_to_terms();
	s1.terms_to_polish_entry();
	s1.polish_entry_to_solution();
	double a = s1.getSolution();
	std::cout << a;
	EXPECT_EQ(3, a);
}
TEST(Translator, can_count_cos)
{
	std::string s = "1+2*(3-cos(0))-4";
	arithmetic_expression s1(s);
	s1.expression_to_terms();
	s1.terms_to_polish_entry();
	s1.polish_entry_to_solution();
	double a = s1.getSolution();
	std::cout << a;
	EXPECT_EQ(1, a);
}
TEST(Translator, can_count_tan)
{
	std::string s = "1+2*(3-tan(0))-4";
	arithmetic_expression s1(s);
	s1.expression_to_terms();
	s1.terms_to_polish_entry();
	s1.polish_entry_to_solution();
	double a = s1.getSolution();
	std::cout << a;
	EXPECT_EQ(3, a);
}
TEST(Translator, can_create_vector_of_terms)
{
	std::string s = "21+(3*5)/3";
	arithmetic_expression s1(s);
	s1.expression_to_terms();
	std::vector<term*> ans_term;
	ans_term.push_back(new operand(21));
	ans_term.push_back(new operation('+'));
	ans_term.push_back(new open_bracket);
	ans_term.push_back(new operand(3));
	ans_term.push_back(new operation('*'));
	ans_term.push_back(new operand(5));
	ans_term.push_back(new close_bracket);
	ans_term.push_back(new operation('/'));
	ans_term.push_back(new operand(3));
	std::vector<term*> terms = s1.getTerms();
	int f = 0;
	if (ans_term.size() != terms.size()) f = 1; else {
		for (std::size_t i = 0; i < ans_term.size(); i++)
		{
			if (ans_term[i]->getType() == operand_)
			{
				ASSERT_NO_THROW(((operand*)(terms[i])));
				if (((operand*)(ans_term[i]))->getValue() != ((operand*)(terms[i]))->getValue()) {
					f = 1;
					break;
				}
			}
			if (ans_term[i]->getType() == operation_)
			{
				ASSERT_NO_THROW(((operation*)(terms[i])));
				if (((operation*)(ans_term[i]))->getOperation() != ((operation*)(terms[i]))->getOperation()) {
					f = 1;
					break;
				}
			}
			if (ans_term[i]->getType() == open_bracket_)
			{
				ASSERT_NO_THROW(((open_bracket*)(terms[i])));
				if (((open_bracket*)(ans_term[i]))->getBracket() != ((open_bracket*)(terms[i]))->getBracket()) {
					f = 1;
					break;
				}
			}
			if (ans_term[i]->getType() == close_bracket_)
			{
				ASSERT_NO_THROW(((close_bracket*)(terms[i])));
				if (((close_bracket*)(ans_term[i]))->getBracket() != ((close_bracket*)(terms[i]))->getBracket()) {
					f = 1;
					break;
				}
			}
		}
	}
	EXPECT_EQ(0, f);
}
TEST(Translator, cant_create_vector_of_terms_with_wrong_input1)
{
	std::string s = "21.54.1+(3+5*7-16/2)*3";
	arithmetic_expression s1(s);
	ASSERT_ANY_THROW(s1.expression_to_terms());
}
TEST(Translator, can_create_polish_entry)
{
	std::string s = "21+(3*5)/3";
	arithmetic_expression s1(s);
	s1.expression_to_terms();
	s1.terms_to_polish_entry();
	std::vector<term*> polish_entry = s1.getPolishEntry();
	std::vector<term*> polish_entry_ans;
	polish_entry_ans.push_back(new operand(21));
	polish_entry_ans.push_back(new operand(3));
	polish_entry_ans.push_back(new operand(5));
	polish_entry_ans.push_back(new operation('*'));
	polish_entry_ans.push_back(new operand(3));
	polish_entry_ans.push_back(new operation('/'));
	polish_entry_ans.push_back(new operation('+'));
	int f = 0;
	if (polish_entry.size() != polish_entry_ans.size()) f = 1; else {
		for (std::size_t i = 0; i < polish_entry_ans.size(); i++)
		{
			if (polish_entry_ans[i]->getType() == operand_)
			{
				ASSERT_NO_THROW(((operand*)(polish_entry[i])));
				if (((operand*)(polish_entry_ans[i]))->getValue() != ((operand*)(polish_entry[i]))->getValue()) {
					f = 1;
					break;
				}
			}
			if (polish_entry_ans[i]->getType() == operation_)
			{
				ASSERT_NO_THROW(((operation*)(polish_entry[i])));
				if (((operation*)(polish_entry_ans[i]))->getOperation() != ((operation*)(polish_entry[i]))->getOperation()) {
					f = 1;
					break;
				}
			}
		}
	}
	EXPECT_EQ(0, f);
}
TEST(Translator, can_get_solution_from_polish_entry)
{
	std::string s = "21+(3+5*7-16/2)*3";
	arithmetic_expression s1(s);
	s1.expression_to_terms();
	s1.terms_to_polish_entry();
	s1.polish_entry_to_solution();
	double ans_s1 = s1.getSolution();
	EXPECT_EQ(111, ans_s1);
}
/*
TEST(Translator, can_check_isDigit)
{
	EXPECT_EQ(1,isDigit('3'));
}
TEST(Translator, cant_check_isDigit_with_not_digit)
{
	EXPECT_EQ(0, isDigit('a'));
}
TEST(Translator, can_check_isPlusorMinus)
{
	EXPECT_EQ(1, isPlusorMinus('+'));
}
TEST(Translator, cant_check_isPlusorMinus_with_not_PlusorMinus)
{
	EXPECT_EQ(0, isPlusorMinus('*'));
}
TEST(Translator, can_check_isOperation)
{
	EXPECT_EQ(1, isOperation('+'));
}
TEST(Translator, cant_check_isOperation_with_not_Operation)
{
	EXPECT_EQ(0, isOperation(':'));
}
TEST(Translator, can_check_isDot)
{
	EXPECT_EQ(1, isDot('.'));
}
TEST(Translator, cant_check_isDot_with_not_Dot)
{
	EXPECT_EQ(0, isDot(','));
}
*/