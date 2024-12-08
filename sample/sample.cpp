#include <iostream>
#include "term.h"
#include "translator.h"
#include <vector>

int main()
{
	//std::string s = "21+(3+5*7-16/2)*3+";
	//std::string s = "(-.1)";
	std::string s = "2*(3-cos(2-2))";
	//std::string s = "sin(3.14)";
	arithmetic_expression s1(s);
	s1.expression_to_terms();
	s1.print_terms();
	std::cout << std::endl;
	s1.terms_to_polish_entry();
	s1.print_polish_entry();
	std::cout << std::endl;
	s1.polish_entry_to_solution();
	double a = s1.getSolution();
	std::cout << a;

}