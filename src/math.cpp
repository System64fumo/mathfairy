#include <string>
#include <vector>
#include <unordered_map>
#include <cmath>

bool has_error = false;
std::unordered_map<char, int> precedence = {{'+', 1}, {'-', 1}, {'*', 2}, {'x', 2}, {'/', 2}, {'^', 3}};

double apply_operator(const double& left, const double& right, const char& op) {
	switch (op) {
		case '+': return left + right;
		case '-': return left - right;
		case '*': return left * right;
		case 'x': return left * right;
		case '/':
			if (right == 0.0) {
				has_error = true;
				return 0.0;
			}
			return left / right;
		case '^': return std::pow(left, right);
		default:
			has_error = true;
			return 0.0;
	}
}

double evaluate_expression(const std::string& expression) {
	std::vector<double> values;
	std::vector<char> operators;
	
	has_error = false;

	size_t i = 0;
	bool expectOperand = true;

	while (i < expression.length()) {
		char c = expression[i];

		if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
			i++;
			continue;
		}

		if (isdigit(c) || c == '.') {
			std::string num_str;
			while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.')) {
				num_str += expression[i];
				i++;
			}

			double value = 0.0;
			double fraction = 0.0;
			bool has_decimal = false;
			double decimal_place = 10.0;
			
			for (char ch : num_str) {
				if (ch == '.') {
					has_decimal = true;
				} else if (!has_decimal) {
					value = value * 10.0 + (ch - '0');
				} else {
					fraction = fraction + (ch - '0') / decimal_place;
					decimal_place *= 10.0;
				}
			}
			
			values.push_back(value + fraction);
			expectOperand = false;
		} else if (c == '(') {
			operators.push_back(c);
			i++;
			expectOperand = true;
		} else if (c == ')') {
			while (!operators.empty() && operators.back() != '(') {
				char op = operators.back();
				operators.pop_back();

				if (values.size() < 2) {
					has_error = true;
					return 0.0;
				}

				double right = values.back();
				values.pop_back();
				double left = values.back();
				values.pop_back();

				values.push_back(apply_operator(left, right, op));
			}
			if (operators.empty() || operators.back() != '(') {
				has_error = true;
				return 0.0;
			}
			operators.pop_back();
			i++;
			expectOperand = false;
		} else if (precedence.find(c) != precedence.end()) {
			if (expectOperand && c == '-') {
				i++;
				std::string num_str;
				while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.')) {
					num_str += expression[i];
					i++;
				}
				
				if (num_str.empty()) {
					values.push_back(-1.0);
					operators.push_back('*');
				} else {
					double value = 0.0;
					double fraction = 0.0;
					bool has_decimal = false;
					double decimal_place = 10.0;
					
					for (char ch : num_str) {
						if (ch == '.') {
							has_decimal = true;
						} else if (!has_decimal) {
							value = value * 10.0 + (ch - '0');
						} else {
							fraction = fraction + (ch - '0') / decimal_place;
							decimal_place *= 10.0;
						}
					}
					
					values.push_back(-(value + fraction));
				}
				expectOperand = false;
			} else {
				while (!operators.empty() && precedence[operators.back()] >= precedence[c]) {
					char op = operators.back();
					operators.pop_back();

					if (values.size() < 2) {
						has_error = true;
						return 0.0;
					}

					double right = values.back();
					values.pop_back();
					double left = values.back();
					values.pop_back();

					values.push_back(apply_operator(left, right, op));
				}
				operators.push_back(c);
				i++;
				expectOperand = true;
			}
		} else {
			has_error = true;
			return 0.0;
		}
	}

	while (!operators.empty()) {
		char op = operators.back();
		operators.pop_back();

		if (values.size() < 2) {
			has_error = true;
			return 0.0;
		}

		double right = values.back();
		values.pop_back();
		double left = values.back();
		values.pop_back();

		values.push_back(apply_operator(left, right, op));
	}

	if (values.size() != 1) {
		has_error = true;
		return 0.0;
	}

	return values.back();
}

std::string format_double(double value) {
	char buffer[50];
	
	if (std::abs(value) < 1e-10) {
		return "0";
	}
	
	if (std::abs(value - std::round(value)) < 1e-10) {
		long long int_val = static_cast<long long>(std::round(value));
		std::snprintf(buffer, sizeof(buffer), "%lld", int_val);
	} else {
		std::snprintf(buffer, sizeof(buffer), "%.10f", value);

		std::string result = buffer;
		size_t pos = result.find_last_not_of('0');
		if (pos != std::string::npos && pos >= result.find('.')) {
			result.erase(pos + 1);
			if (result.back() == '.') {
				result.pop_back();
			}
		}
		return result;
	}
	
	return std::string(buffer);
}