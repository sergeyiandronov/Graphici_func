#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <cmath>
const std::map<char, double> priorities = { {'+',0},{'-',0},{'*',0.5},{'/',0.5},{'^',0.6} };
class ariphmetic_node {
	std::string	expression="";
    double number;
	char var;
	char operation;
	ariphmetic_node* rightChild=nullptr;
	ariphmetic_node* leftChild=nullptr;
	bool parser_braket(std::string& s) {
		if (s[0]=='(' && s[s.length()-1]==')') {
			s = s.substr(1, s.length() - 2);
			return true;
		}
		return false;
	}
	void made_var_number() {
		if (expression.length() == 1 && expression[0] > 96 && expression[0] < 123) {
			var = expression[0];
			
		}
		else { var = NULL;}
		int delim_number = 0;
		for (int i = 0; i < expression.length(); i++) {
			if (expression[i] == '.' || expression[i] == ',') {
				delim_number++;
			}
			if (!((expression[i] > 47 && expression[i] < 58||(expression[i] == '.'|| expression[i] == ','))&&delim_number<2)) {
				number = -1;
				return;
			}
			
		}
		std::istringstream s_number_str(expression);
		if (!(s_number_str >> number)) {
			number = -1;
		}

	}
	
public:
	
	ariphmetic_node(std::string expression_string) {
		for (int i = 0; i < expression_string.length();i++) {
			if (expression_string[i]!=' ') {
				expression += expression_string[i];
			}
		}
		while (parser_braket(expression));
		
		double min_priority=-1;
		double priority=0;
		int operationpos=-1;
		int braket_number = 0;
		char f=0;
		std::string rightChilds;
		std::string	leftChilds;
		int i = 0;
		if (expression.substr(0, 3) == "cos") {
			f = 'c';
			i = 3;
		}
		if (expression.substr(0, 3) == "tan") {
			f = 't';
			i = 3;
		}
		if (expression.substr(0, 3) == "sin") {
			f = 's';
			i = 3;
		}
		
		if (expression.substr(0, 1) == "-") {
			f = 'm';
			i = 1;
		}
		for (; i <expression.length();i++) {
			if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/'||expression[i]=='^') {
				
				priority = braket_number + priorities.at(expression[i]);
				if (min_priority == -1) {
					min_priority = priority;
					operationpos = i;
				}
			}
			
			if (expression[i ]=='(') {
				braket_number++;
			}
			if (expression[i] == ')') {		
				braket_number--;
			}
			if (braket_number == 0 && f && i==expression.length()-1) {
				operation = f;
				leftChild = nullptr;
				rightChild = new ariphmetic_node(expression.substr((f == 'm' ? 1:3),(f!='m' ? i-2 : i )));
				var = NULL;
				number = -1;
				return;

			}
			if (braket_number == 0 && f) {
				f = 0;
			}
			if (braket_number < 0) {
				throw "invalid syntax";
			}
			if ((expression[i] == '^'|| expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/') && priority <= min_priority) {
				min_priority = priority;
				operationpos = i ;
			}
			
		}
		if (operationpos != -1) {
			leftChilds = expression.substr(0, operationpos);
			rightChilds = expression.substr(operationpos+1 );
			rightChild = new ariphmetic_node(rightChilds);
			leftChild = new ariphmetic_node(leftChilds);
			operation = expression[operationpos];
			var = NULL;
			number = -1;
			return;

		}
		/*if (expression.substr(0, 4) == "cos("&&expression[expression.length() - 1] == ')') {
			operation = 'c';
			leftChild = nullptr;
			rightChild = new ariphmetic_node(expression.substr(4, expression.length() - 5));
			var = NULL;
			number = -1;
			return;
		}
		if (expression.substr(0, 4) == "sin("&&expression[expression.length() - 1] == ')') {
			operation = 's';
			leftChild = nullptr;
			rightChild = new ariphmetic_node(expression.substr(4, expression.length() - 5));
			var = NULL;
			number = -1;
			return;
		}
		if (expression[0] == '-') {
			operation = 'm';
			leftChild = nullptr;
			rightChild = new ariphmetic_node(expression.substr(1));
			var = NULL;
			number = -1;
			return;
		}*/
		operation = NULL;
		made_var_number();
	}
	std::string printleft() {
		if (leftChild != nullptr) {
			return leftChild->printleft();
		}
		return expression;
	}
	
	double calc(double x,double y) {
		switch (operation) {
		case '+':
			return leftChild->calc(x,y) + rightChild->calc(x,y);
			break;
		case '-':
			return leftChild->calc(x,y) - rightChild->calc(x,y);
			break;

		case '*':
			return leftChild->calc(x,y) * rightChild->calc(x,y);
			break;
		case '/':
			return leftChild->calc(x,y) / rightChild->calc(x,y);
			break;
		case '^':
			return pow(leftChild->calc(x, y), rightChild->calc(x, y));
			break;
		case 'c':
			return cos( rightChild->calc(x, y));
			break;
		case 's':
			return sin(rightChild->calc(x, y));
			break;
		case 'm':
			return -rightChild->calc(x, y);
			break;
		case 't':
			return tan(rightChild->calc(x,y));
			break;
		case NULL:
			if (var == 'x') {
				return x;
			}
			if (var == 'y') {
				return y;
			}
			if (number != -1) {
				return number;
			}
			break;
		}
		throw "invalid syntax";
	    return 0;
	}
	std::string calc_str(double x, double y){
		std::ostringstream cs;
		cs << calc(x, y); 
		return cs.str();

	}

};