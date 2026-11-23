#ifndef EXPRESSION_EVALUATOR_HPP
#define EXPRESSION_EVALUATOR_HPP

#include <iostream>
#include <string>
#include <cctype>
#include <cmath>
#include <vector>
#include "UI.hpp"

// Implements Infix to Postfix & Evaluation matching Lab Week 8
class ExpressionEvaluator {
private:
    static int precedence(char op) {
        if (op == '^') return 3;
        if (op == '*' || op == '/' || op == '%') return 2;
        if (op == '+' || op == '-') return 1;
        return 0;
    }

    static bool isOperator(char c) {
        return (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^');
    }

    static double applyOp(double a, double b, char op) {
        switch (op) {
            case '+': return a + b;
            case '-': return a - b;
            case '*': return a * b;
            case '/': return b != 0 ? (a / b) : 0;
            case '%': return static_cast<long long>(b) != 0 ? static_cast<long long>(a) % static_cast<long long>(b) : 0;
            case '^': return std::pow(a, b);
            default: return 0;
        }
    }

public:
    // Lab Week 8: Infix to Postfix conversion with +, -, *, /, %, ^ and ()
    static std::string infixToPostfix(const std::string& infix) {
        std::string postfix = "";
        std::vector<char> opStack;

        for (size_t i = 0; i < infix.length(); ++i) {
            char c = infix[i];
            if (std::isspace(c)) continue;

            if (std::isalnum(c)) {
                postfix += c;
                postfix += ' ';
            } else if (c == '(') {
                opStack.push_back(c);
            } else if (c == ')') {
                while (!opStack.empty() && opStack.back() != '(') {
                    postfix += opStack.back();
                    postfix += ' ';
                    opStack.pop_back();
                }
                if (!opStack.empty() && opStack.back() == '(') {
                    opStack.pop_back(); // Pop '('
                }
            } else if (isOperator(c)) {
                while (!opStack.empty() && opStack.back() != '(' &&
                       (precedence(opStack.back()) > precedence(c) ||
                       (precedence(opStack.back()) == precedence(c) && c != '^'))) {
                    postfix += opStack.back();
                    postfix += ' ';
                    opStack.pop_back();
                }
                opStack.push_back(c);
            }
        }

        while (!opStack.empty()) {
            postfix += opStack.back();
            postfix += ' ';
            opStack.pop_back();
        }

        return postfix;
    }

    // Evaluates numeric postfix expressions
    static double evaluatePostfix(const std::string& postfix) {
        std::vector<double> valStack;
        for (size_t i = 0; i < postfix.length(); ++i) {
            char c = postfix[i];
            if (std::isspace(c)) continue;

            if (std::isdigit(c)) {
                double val = 0;
                while (i < postfix.length() && std::isdigit(postfix[i])) {
                    val = (val * 10) + (postfix[i] - '0');
                    i++;
                }
                i--;
                valStack.push_back(val);
            } else if (isOperator(c)) {
                if (valStack.size() < 2) return 0;
                double val2 = valStack.back(); valStack.pop_back();
                double val1 = valStack.back(); valStack.pop_back();
                valStack.push_back(applyOp(val1, val2, c));
            }
        }
        return valStack.empty() ? 0 : valStack.back();
    }
};

#endif // EXPRESSION_EVALUATOR_HPP
