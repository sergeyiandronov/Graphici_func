
#include "ariphmetic_node.h"

bool ariphmetic_node::parser_braket(std::string& s) {
	if (s[0] == '(' && s[s.length() - 1] == ')') {
		s = s.substr(1, s.length() - 2);
		return true;
	}
	return false;
}
void ariphmetic_node::made_var_number() {
	if (expression.length() == 1 && expression[0] > 96 && expression[0] < 123) {
		var = expression[0];

	}
	else { var = NULL; }
	int delim_number = 0;
	for (int i = 0; i < expression.length(); i++) {
		if (expression[i] == '.' || expression[i] == ',') {
			delim_number++;
		}
		if (!((expression[i] > 47 && expression[i] < 58 || (expression[i] == '.' || expression[i] == ',')) && delim_number<2)) {
			number = -1;
			return;
		}

	}
	std::istringstream s_number_str(expression);
	if (!(s_number_str >> number)) {
		number = -1;
	}

}
ariphmetic_node::ariphmetic_node(std::string expression_string) {
	for (int i = 0; i < expression_string.length(); i++) {
		if (expression_string[i] != ' ') {
			expression += expression_string[i];
		}
	}
	while (parser_braket(expression));

	double min_priority = -1;
	double priority = 0;
	int operationpos = -1;
	int braket_number = 0;
	char f = 0;
	std::string rightChilds;
	std::string	leftChilds;
	int i = 0;
	int foffset = 0;
	bool no_op_yet = true;
	if (expression.substr(0, 3) == "cos") {
		f = 'c';
		i = 3;
		foffset = 3;
	}
	if (expression.substr(0, 3) == "tan") {
		f = 't';
		i = 3;
		foffset = 3;
	}
	if (expression.substr(0, 3) == "sin") {
		f = 's';
		i = 3;
		foffset = 3;
	}

	if (expression.substr(0, 3) == "exp") {
		f = 'e';
		i = 3;
		foffset = 3;
	}
	if (expression.substr(0, 3) == "log") {
		f = 'l';
		i = 3;
		foffset = 3;
	}
	if (expression.substr(0, 1) == "-") {
		f = 'm';
		i = 1;
		foffset = 1;
	}

	for (; i <expression.length(); i++) {
		if (expression[i] == '+' || (expression[i] == '-'&&i != foffset && (expression[i - 1] != '+' && expression[i - 1] != '*' && expression[i - 1] != '/' && expression[i - 1] != '^')) || expression[i] == '*' || expression[i] == '/' || expression[i] == '^') {
			no_op_yet = false;
			priority = braket_number + priorities.at(expression[i]);
			if (min_priority == -1) {
				min_priority = priority;
				operationpos = i;
			}
		}

		if (expression[i] == '(') {
			braket_number++;
		}
		if (expression[i] == ')') {
			braket_number--;
		}
		if (braket_number == 0 && f && i == expression.length() - 1) {
			operation = f;
			leftChild = nullptr;

			rightChild = new ariphmetic_node(expression.substr(foffset, i - foffset + 1));





			var = NULL;
			number = -1;
			return;

		}
		if (braket_number == 0 && f && !(no_op_yet)) {
			f = 0;
			foffset = 0;
		}
		if (braket_number < 0) {
			throw "недопустимое выражение";
		}
		if ((expression[i] == '^' || expression[i] == '+' || (expression[i] == '-'&&i != 0 && (expression[i - 1] != '+' && expression[i - 1] != '*' && expression[i - 1] != '/' && expression[i - 1] != '^')) || expression[i] == '*' || expression[i] == '/') && priority <= min_priority) {
			min_priority = priority;
			operationpos = i;
		}

	}
	if (operationpos != -1) {
		leftChilds = expression.substr(0, operationpos);
		rightChilds = expression.substr(operationpos + 1);
		rightChild = new ariphmetic_node(rightChilds);
		leftChild = new ariphmetic_node(leftChilds);
		operation = expression[operationpos];
		var = NULL;
		number = -1;
		return;

	}

	operation = NULL;
	made_var_number();

}


double ariphmetic_node::calc(double x) {
	switch (operation) {
	case '+':
		return leftChild->calc(x) + rightChild->calc(x);
		break;
	case '-':
		return leftChild->calc(x) - rightChild->calc(x);
		break;

	case '*':
		return leftChild->calc(x) * rightChild->calc(x);
		break;
	case '/':
		return leftChild->calc(x) / rightChild->calc(x);
		break;
	case '^':
		return pow(leftChild->calc(x), rightChild->calc(x));
		break;
	case 'c':
		return cos(rightChild->calc(x));
		break;
	case 's':
		return sin(rightChild->calc(x));
		break;
	case 'm':
		return -rightChild->calc(x);
		break;
	case 't':
		return tan(rightChild->calc(x));
		break;
	case 'e':
		return exp(rightChild->calc(x));
		break;
	case 'l':
		return log(rightChild->calc(x));
		break;
	case NULL:
		if (var == 'x') {
			return x;
		}
		if (number != -1) {
			return number;
		}
		break;
	}
	return 0;
}
void ariphmetic_node::check_except() {
	switch (operation) {
	case '+':
		leftChild->check_except();
		rightChild->check_except();
		return;
		break;
	case '-':
		leftChild->check_except();
		rightChild->check_except();
		return;
		break;

	case '*':
		leftChild->check_except();
		rightChild->check_except();
		return;
		break;
	case '/':
		leftChild->check_except();
		rightChild->check_except();
		return;
		break;
	case '^':
		leftChild->check_except();
		rightChild->check_except();
		return;
		break;
	case 'c':

		rightChild->check_except();
		return;
		break;
	case 's':

		rightChild->check_except();
		return;
		break;
	case 'm':

		rightChild->check_except();
		return;
		break;
	case 't':

		rightChild->check_except();
		return;
		break;
	case 'e':

		rightChild->check_except();
		return;
		break;
	case 'l':

		rightChild->check_except();
		return;
		break;
	case NULL:
		if (var == 'x') {
			return;
		}
		if (number != -1) {
			if (number > 100) {
				throw "слишком большие числа в выражении";
			}
			if (number < 0.001) {
				throw "слишком маленькие числа в выражении";
			}
			return;
		}
		break;
	}
	throw "недопустимое выражение";
	return;
}
std::string ariphmetic_node::calc_str(double x) {
	std::ostringstream cs;
	cs << calc(x);
	return cs.str();

}
ariphmetic_node::~ariphmetic_node() {
	delete rightChild;
	delete leftChild;
}