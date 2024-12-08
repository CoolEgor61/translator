
#ifndef term_h
#define term_h

enum types
{operand_, operation_, open_bracket_, close_bracket_};

class term
{
private:
	types type;
public:
	term(types T) : type(T) {};
	types getType() { return type; };
};

class operand : public term
{
private:
	double value;
public:
	operand(double val = 0) : value(val), term(types::operand_) {};
	double getValue() noexcept { return value; };
};

class operation : public term
{
private:
	char op;
	int priority;
public:
	int pr(char c) {
		if (c == '+' || c == '-') return 1; else
			if (c == '*' || c == '/') return 2; else
				return 0;
	}
	operation(char _op) : op(_op), priority(pr(_op)), term(types::operation_) {};
	char getOperation() noexcept { return op; };
	int getPriority() noexcept { return priority; };
};

class open_bracket : public term
{
private:
	char bracket;
public:
	open_bracket() : bracket('('), term(types::open_bracket_) {};
	char getBracket() { return bracket; };
};

class close_bracket : public term
{
private:
	char bracket;
public:
	close_bracket() : bracket(')'), term(types::close_bracket_) {};
	char getBracket() { return bracket; };
};

#endif
