#ifndef translator_h
#define translator_h

#include "term.h"
#include <string>
#include "mStack.h"
#include <vector>
#include <exception>
#include <iostream>

class arithmetic_expression
{
private:
	std::string expression;
	std::vector<term*> terms, polish_entry;
	double solution;
public:
	arithmetic_expression(std::string s) : expression(s) {};
	~arithmetic_expression();

	friend int isDigit(char c);
	friend int isPlusorMinus(char c);
	friend int isOperation(char c);
	friend int isDot(char c);
	friend int isLetter(char c);

	void expression_to_terms();
	void print_terms();
	void terms_to_polish_entry();
	void print_polish_entry();
	void polish_entry_to_solution();

	int syntax_analysis();
	std::vector<term*> getTerms() noexcept { return terms; };
	std::vector<term*> getPolishEntry() noexcept { return polish_entry; };
	double getSolution() noexcept { return solution; };

};

#endif