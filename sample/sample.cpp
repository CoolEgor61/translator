#include <iostream>
#include "term.h"
#include "translator.h"
#include <vector>

int main()
{
	std::string s;
	std::cout << "Input arithmetic expresson:\n";
	std::cin >> s;
	arithmetic_expression s1(s);
	s1.expression_to_terms();
	//s1.print_terms();
	//std::cout << std::endl;
	s1.terms_to_polish_entry();
	//s1.print_polish_entry();
	//std::cout << std::endl;
	s1.polish_entry_to_solution();
	double a = s1.getSolution();
	std::cout << "Solution: " << a;

}