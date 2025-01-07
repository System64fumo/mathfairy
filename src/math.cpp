#include <string>
#include <sstream>
#include <stack>
#include <map>
#include <cctype>
#include <cmath>
#include <stdexcept>

// Define operator precedence
std::map<char, int> precedence = {{'+', 1}, {'-', 1}, {'*', 2}, {'x', 2}, {'/', 2}, {'^', 3}};

// Function to perform the arithmetic operations
double apply_operator(double left, double right, char op) {
	switch (op) {
		case '+': return left + right;
		case '-': return left - right;
		case '*': return left * right;
		case 'x': return left * right;
		case '/':
			if (right == 0.0) {
				throw std::runtime_error("Error: Division by zero");
			}
			return left / right;
		case '^': return std::pow(left, right);
		default: throw std::invalid_argument("Error: Unknown operator");
	}
}

double evaluate_expression(const std::string& expression) {
	std::stack<double> values;
	std::stack<char> operators;

	size_t i = 0;
	bool expectOperand = true;

	while (i < expression.length()) {
		char c = expression[i];

		if (std::isspace(c)) {
			i++;
			continue;
		}

		if (isdigit(c) || c == '.') {
			std::stringstream ss;
			while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.')) {
				ss << expression[i];
				i++;
			}
			double value;
			ss >> value;
			values.push(value);
			expectOperand = false;
		} else if (c == '(') {
			operators.push(c);
			i++;
			expectOperand = true;
		} else if (c == ')') {
			while (!operators.empty() && operators.top() != '(') {
				char op = operators.top();
				operators.pop();

				if (values.size() < 2) {
					throw std::invalid_argument("Error: Invalid expression, insufficient operands");
				}

				double right = values.top();
				values.pop();
				double left = values.top();
				values.pop();

				values.push(apply_operator(left, right, op));
			}
			if (operators.empty() || operators.top() != '(') {
				throw std::invalid_argument("Error: Unbalanced parentheses");
			}
			operators.pop(); // Remove '('
			i++;
			expectOperand = false; // After ')', we expect an operator
		} else if (precedence.find(c) != precedence.end()) {
			// Operator found, handle precedence
			if (expectOperand && c == '-') {
				// Unary minus: next token should be a number
				i++;
				std::stringstream ss;
				while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.')) {
					ss << expression[i];
					i++;
				}
				double value;
				ss >> value;
				values.push(-value);
				expectOperand = false;
			} else {
				while (!operators.empty() && precedence[operators.top()] >= precedence[c]) {
					char op = operators.top();
					operators.pop();

					if (values.size() < 2) {
						throw std::invalid_argument("Error: Invalid expression, insufficient operands");
					}

					double right = values.top();
					values.pop();
					double left = values.top();
					values.pop();

					values.push(apply_operator(left, right, op));
				}
				operators.push(c);
				i++;
				expectOperand = true; // After an operator, we expect an operand
			}
		} else {
			throw std::invalid_argument("Error: Invalid character in expression");
		}
	}

	// Process any remaining operators
	while (!operators.empty()) {
		char op = operators.top();
		operators.pop();

		if (values.size() < 2) {
			throw std::invalid_argument("Error: Invalid expression, insufficient operands");
		}

		double right = values.top();
		values.pop();
		double left = values.top();
		values.pop();

		values.push(apply_operator(left, right, op));
	}

	if (values.size() != 1) {
		throw std::invalid_argument("Error: Invalid expression, too many values left");
	}

	return values.top();
}
