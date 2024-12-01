#ifndef translator_h
#define translator_h

#include "term.h"
#include <string>
#include <stack>
#include <vector>
#include <exception>
#include <iostream>

class arithmetic_expression
{
public:
	std::string expression;
	std::vector<term*> terms, polish_entry;
	double solution;
public:
	arithmetic_expression(std::string s) : expression(s) {};
	arithmetic_expression(std::vector<term*> s) : terms(s) {};

	friend int isDigit(char c);

	friend int isPlusorMinus(char c);

	friend int isOperation(char c);

	friend int isDot(char c);

	void expression_to_terms();
	void print_terms();
	int syntax_analysis();
	void terms_to_polish_entry();
	void print_polish_entry();
	void polish_entry_to_solution();
	double getSolution() { return solution; };

};

#endif