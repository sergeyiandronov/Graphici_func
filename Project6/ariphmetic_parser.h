#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <cmath>
const std::map<char, double> priorities = { {'+',0},{'-',0},{'*',0.5},{'/',0.5},{'^',0.6} };
class ariphmetic_node {
	std::string	expression="";
    double number=0;
	char var=0; 
	char operation=0;
	ariphmetic_node* rightChild=nullptr;
	ariphmetic_node* leftChild=nullptr;
	bool parser_braket(std::string& s);
    void made_var_number();
	
public:
	
	ariphmetic_node(std::string expression_string);			
	double calc(double x);
	void check_except();
	std::string calc_str(double x);
	
};