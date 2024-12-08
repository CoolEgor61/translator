#include "translator.h"
#include <math.h>

arithmetic_expression::~arithmetic_expression()
{
	if (!terms.size())
	{
		for (std::size_t i = 0; i < terms.size(); i++) delete terms[i];
	}
	if (!polish_entry.size())
	{
		for (std::size_t i = 0; i < polish_entry.size(); i++) delete polish_entry[i];
	}
}

int isDigit(char c)
{
	if (c >= '0' && c <= '9') return 1;
	else return 0;
}

int isPlusorMinus(char c)
{
	if (c == '+' || c == '-') return 1;
	else return 0;
}

int isOperation(char c)
{
	if (c == '+' || c == '-' || c == '*' || c == '/') return 1;
	else return 0;
}

int isDot(char c)
{
	if (c == '.') return 1;
	else return 0;
}

int isLetter(char c)
{
	if (c >= 97 && c <= 122) return 1;
	else return 0;
}

void arithmetic_expression::expression_to_terms()
{
	int status = 0, gotDot = 0;
	std::string number, function, ar_expr;
	std::stack<char> t;
	for (std::size_t i = 0; i < expression.size(); i++)
	{
		if (status == 0)
		{
			if (isDigit(expression[i]) || expression[i] == '.')
			{
				status = 1;
				if (expression[i] == '.') gotDot = 1;
				number += expression[i];
				if (i == expression.size() - 1) { status = 0; gotDot = 0; terms.push_back(new operand(stod(number))); number.clear(); }
			} else if (isOperation(expression[i]))
			{
				if (isPlusorMinus(expression[i]) && terms.back()->getType() == open_bracket_)
				{
					status = 1;
					number += expression[i];
				} else terms.push_back(new operation(expression[i]));
			} else if (expression[i] == '(')
			{
				terms.push_back(new open_bracket);
			} else if (expression[i] == ')')
			{
				terms.push_back(new close_bracket);
			}
			else if (isLetter(expression[i]))
			{
				status = 2;
				function.push_back(expression[i]);
			}
		}
		else if (status==1)
		{
			if (isDigit(expression[i]) || expression[i] == '.')
			{
					if (expression[i] == '.' && gotDot == 1) throw std::logic_error("Wrong input");
					else {
						if (expression[i] == '.') gotDot = 1;
						number += expression[i];
						if (i == expression.size() - 1) { status = 0; gotDot = 0; terms.push_back(new operand(stod(number))); number.clear(); }
					}
			}
			else if (isOperation(expression[i]))
			{
					status = 0;
					gotDot = 0;
					terms.push_back(new operand(stod(number)));
					terms.push_back(new operation(expression[i]));
					number.clear();
			}
			else if (expression[i] == '(')
			{
					status = 0;
					gotDot = 0;
					terms.push_back(new operand(stod(number)));
					terms.push_back(new open_bracket);
					number.clear();
			}
			else if (expression[i] == ')')
			{
					status = 0;
					gotDot = 0;
					terms.push_back(new operand(stod(number)));
					terms.push_back(new close_bracket);
					number.clear();
			}
			else if (isLetter(expression[i]))
			{
				throw std::logic_error("Wrong input");
			}
		}
		else if (status == 2)
		{
			if (isLetter(expression[i]))
			{
				function.push_back(expression[i]);
			}
			else if (expression[i] == '(')
			{
				ar_expr.push_back(expression[i]);
				t.push('(');
			}
			else if (expression[i] == ')')
			{
				t.pop();
				ar_expr.push_back(expression[i]);
				if (t.empty())
				{
					status = 0;
					arithmetic_expression foperand(ar_expr);
					foperand.expression_to_terms();
					foperand.terms_to_polish_entry();
					foperand.polish_entry_to_solution();
					if (function == "sin") terms.push_back(new operand(sin(foperand.getSolution())));
					if (function == "cos") terms.push_back(new operand(cos(foperand.getSolution())));
					if (function == "tan") terms.push_back(new operand(tan(foperand.getSolution())));
					function.clear(); ar_expr.clear();
				}
			}
			else if (isDigit(expression[i]))
			{
				ar_expr.push_back(expression[i]);
			}
			else if (isOperation(expression[i]))
			{
				if (ar_expr.back()==types::operation_) throw std::logic_error("Wrong input"); else ar_expr.push_back(expression[i]);
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
	std::stack<char> t;
	if (terms[0]->getType() == types::operand_) state = 0;
	if (terms[0]->getType() == types::operation_) throw std::logic_error("Wrong input");
	if (terms[0]->getType() == types::open_bracket_) { state = 2; t.push('('); }
	if (terms[0]->getType() == types::close_bracket_) throw std::logic_error("Wrong input");
	for (std::size_t i = 1; i < terms.size(); i++)
	{
		switch (state)
		{
		case 0:
			if (terms[i]->getType() == types::open_bracket_ || terms[i]->getType() == types::operand_) {
				throw std::logic_error("Wrong input");
			}
			else if (terms[i]->getType() == types::close_bracket_) { state = 3; if (!t.empty()) t.pop(); else throw std::logic_error("Wrong input"); } else state = 1;
			break;
		case 1:
			if (terms[i]->getType() == types::close_bracket_ || terms[i]->getType() == types::operation_) {
				throw std::logic_error("Wrong input");
			}
			else if (terms[i]->getType() == types::open_bracket_) { state = 2; t.push('('); } else state = 0;
			break;
		case 2:
			if (terms[i]->getType() == types::operation_ || terms[i]->getType() == types::close_bracket_) {
				throw std::logic_error("Wrong input");
			}
			else if (terms[i]->getType() == types::operand_) state = 0; else { state = 2; t.push('('); }
			break;
		case 3:
			if (terms[i]->getType() == types::operand_ || terms[i]->getType() == types::open_bracket_) {
				throw std::logic_error("Wrong input");
			}
			else if (terms[i]->getType() == types::operation_) state = 1; else { state = 3; if (!t.empty()) t.pop(); else throw std::logic_error("Wrong input"); }
			break;
		default:
			break;
		}
	}
	if (!t.empty()) throw std::logic_error("Wrong input");
	if (terms[terms.size()-1]->getType() == types::operation_ || terms[terms.size() - 1]->getType() == types::open_bracket_) throw std::logic_error("Wrong input");
	return 0;
}

void arithmetic_expression::terms_to_polish_entry()
{
	std::stack<term*> st;
	for (std::size_t i = 0; i < arithmetic_expression::terms.size(); i++)
	{
		if (terms[i]->getType() == types::operand_) arithmetic_expression::polish_entry.push_back(new operand(((operand*)(terms[i]))->getValue()));
		else if (terms[i]->getType() == types::operation_)
		{
			if (st.empty()) st.push(new operation(((operation*)(terms[i]))->getOperation()));
			else {
				while (!st.empty() && st.top()->getType()==types::operation_ && ((operation*)(terms[i]))->getPriority() <= ((operation*)(st.top()))->getPriority())
				{
					arithmetic_expression::polish_entry.push_back(st.top());
					st.pop();
				}
				st.push(new operation(((operation*)(terms[i]))->getOperation()));
			}
		}
		else if (terms[i]->getType() == types::open_bracket_) st.push(new open_bracket);
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
			delete st.top();
			st.pop();
			double t2 = ((operand*)(st.top()))->getValue();
			delete st.top();
			st.pop();
			switch (((operation*)(arithmetic_expression::polish_entry[i]))->getOperation())
			{
			case '+': st.push(new operand(t2 + t1));
				break;
			case '-': st.push(new operand(t2 - t1));
				break;
			case '*': st.push(new operand(t2 * t1));
				break;
			case '/': 
				if (t1 == 0) throw std::logic_error("division by zero");
				else st.push(new operand(t2 / t1));
				break;
			default:
				break;
			}
		}
	}
	arithmetic_expression::solution = ((operand*)(st.top()))->getValue();
	delete st.top();
	st.pop();
}

