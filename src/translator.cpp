#include "translator.h"

int isDigit(char c)
{
	if (c >= 48 && c <= 57) return 1;
	else return 0;
}

int isPlusorMinus(char c)
{
	if (c == 43 || c == 45) return 1;
	else return 0;
}

int isOperation(char c)
{
	if (c == '+' || c == '-' || c == '*' || c == '/') return 1;
	else return 0;
}

int isDot(char c)
{
	if (c == 46) return 1;
	else return 0;
}

void arithmetic_expression::expression_to_terms()
{
	int number_status = 0, gotDot = 0;
	std::string number;
	for (std::size_t i = 0; i < expression.size(); i++)
	{
		if (number_status == 0)
		{
			if (isDigit(expression[i]) || expression[i] == '.')
			{
				number_status = 1;
				if (expression[i] == '.') gotDot = 1;
				number += expression[i];
				if (i == expression.size() - 1) { number_status = 0; gotDot = 0; terms.push_back(new operand(stod(number))); number.clear(); }
			}
			if (isOperation(expression[i]))
			{
				terms.push_back(new operation(expression[i]));
			}
			if (expression[i] == '(')
			{
				terms.push_back(new open_bracket);
			}
			if (expression[i] == ')')
			{
				terms.push_back(new close_bracket);
			}
		}
		else
		{
			if (isDigit(expression[i]) || expression[i] == '.')
			{
				if (expression[i] == '.' && gotDot==1) throw std::logic_error("Wrong input");
				else {
					if (expression[i] == '.') gotDot = 1;
					number += expression[i];
					if (i == expression.size() - 1) { number_status = 0; gotDot = 0; terms.push_back(new operand(stod(number))); number.clear(); }
				}
			}
			if (isOperation(expression[i]))
			{
				number_status = 0;
				gotDot = 0;
				terms.push_back(new operand(stod(number)));
				terms.push_back(new operation(expression[i]));
				number.clear();
			}
			if (expression[i] == '(')
			{
				number_status = 0;
				gotDot = 0;
				terms.push_back(new operand(stod(number)));
				terms.push_back(new open_bracket);
				number.clear();
			}
			if (expression[i] == ')')
			{
				number_status = 0;
				gotDot = 0;
				terms.push_back(new operand(stod(number)));
				terms.push_back(new close_bracket);
				number.clear();
			}
		}
	}
	this->syntax_analysis();
}

void arithmetic_expression::print_terms()
{
	for (std::size_t i = 0; i < arithmetic_expression::terms.size(); i++)
	{
		if (arithmetic_expression::terms[i]->getType() == operand_) std::cout << ((operand*)(arithmetic_expression::terms[i]))->getValue() << " ";
		else if (arithmetic_expression::terms[i]->getType() == operation_) std::cout << ((operation*)(arithmetic_expression::terms[i]))->getOperation() << " ";
		else if (arithmetic_expression::terms[i]->getType() == open_bracket_) std::cout << "(" << " ";
		else std::cout << ")" << " ";
	}
}

int arithmetic_expression::syntax_analysis()
{
	std::size_t state;
	if (terms[0]->getType() == types::operand_) state = 0;
	if (terms[0]->getType() == types::operation_) state = 1;
	if (terms[0]->getType() == types::open_bracket_) state = 2;
	if (terms[0]->getType() == types::close_bracket_) state = 3;
	for (std::size_t i = 1; i < terms.size(); i++)
	{
		switch (state)
		{
		case 0:
			if (terms[i]->getType() == types::open_bracket_ || terms[i]->getType() == types::operand_) {
				throw std::logic_error("Wrong input"); return 1;
			}
			else if (terms[i]->getType() == types::close_bracket_) state = 3; else state = 1;
			break;
		case 1:
			if (terms[i]->getType() == types::close_bracket_ || terms[i]->getType() == types::operation_) {
				throw std::logic_error("Wrong input"); return 1;
			}
			else if (terms[i]->getType() == types::open_bracket_) state = 2; else state = 0;
			break;
		case 2:
			if (terms[i]->getType() == types::operation_) {
				throw std::logic_error("Wrong input"); return 1;
			}
			else if (terms[i]->getType() == types::operand_) state = 0; else if (terms[i]->getType() == types::open_bracket_) state = 2; else state = 3;
			break;
		case 3:
			if (terms[i]->getType() == types::operand_) {
				throw std::logic_error("Wrong input"); return 1;
			}
			else if (terms[i]->getType() == types::operation_) state = 1; else if (terms[i]->getType() == types::open_bracket_) state = 2; else state = 3;
			break;
		default:
			break;
		}
	}
	return 0;
}

void arithmetic_expression::terms_to_polish_entry()
{
	std::stack<term*> st;
	for (std::size_t i = 0; i < arithmetic_expression::terms.size(); i++)
	{
		if (terms[i]->getType() == types::operand_) arithmetic_expression::polish_entry.push_back(terms[i]);
		else if (terms[i]->getType() == types::operation_)
		{
			if (st.empty()) st.push(terms[i]);
			else {
				while (((operation*)(terms[i]))->getPriority() <= ((operation*)(st.top()))->getPriority())
				{
					arithmetic_expression::polish_entry.push_back(st.top());
					st.pop();
				}
				st.push(terms[i]);
			}
		}
		else if (terms[i]->getType() == types::open_bracket_) st.push(terms[i]);
		else if (terms[i]->getType() == types::close_bracket_)
		{
			while (st.top()->getType() != types::open_bracket_)
			{
				arithmetic_expression::polish_entry.push_back(st.top());
				st.pop();
			}
			st.pop();
		}
	}
	while (!st.empty())
	{
		arithmetic_expression::polish_entry.push_back(st.top());
		st.pop();
	}
}

void arithmetic_expression::print_polish_entry()
{
	for (std::size_t i = 0; i < arithmetic_expression::polish_entry.size(); i++)
	{
		if (arithmetic_expression::polish_entry[i]->getType() == operand_) std::cout << ((operand*)(arithmetic_expression::polish_entry[i]))->getValue() << " ";
		else if (arithmetic_expression::polish_entry[i]->getType() == operation_) std::cout << ((operation*)(arithmetic_expression::polish_entry[i]))->getOperation() << " ";
		else if (arithmetic_expression::polish_entry[i]->getType() == open_bracket_) std::cout << "(" << " ";
		else std::cout << ")" << " ";
	}
}

void arithmetic_expression::polish_entry_to_solution()
{
	std::stack<term*> st;
	for (std::size_t i = 0; i < arithmetic_expression::polish_entry.size(); i++)
	{
		types current_type = arithmetic_expression::polish_entry[i]->getType();
		if (current_type == types::operand_) st.push(arithmetic_expression::polish_entry[i]);
		else if (current_type == types::operation_)
		{
			double t1 = ((operand*)(st.top()))->getValue();
			st.pop();
			double t2 = ((operand*)(st.top()))->getValue();
			st.pop();
			switch (((operation*)(arithmetic_expression::polish_entry[i]))->getOperation())
			{
			case '+': st.push(new operand(t2 + t1));
				break;
			case '-': st.push(new operand(t2 - t1));
				break;
			case '*': st.push(new operand(t2 * t1));
				break;
			case '/': st.push(new operand(t2 / t1));
				break;
			default:
				break;
			}
		}
	}
	arithmetic_expression::solution = ((operand*)(st.top()))->getValue();
}

