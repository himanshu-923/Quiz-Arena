#ifndef REVIEW_STACK_HPP
#define REVIEW_STACK_HPP

#include <iostream>
#include "Question.hpp"
#include "UI.hpp"

// Implements Stack of marked questions matching Lab Week 7 requirements
class ReviewStack {
private:
    static const int MAX_STACK = 30;
    QuestionNode* stackArray[MAX_STACK];
    int topIndex;

public:
    ReviewStack() : topIndex(-1) {}

    bool isFull() const {
        return topIndex >= MAX_STACK - 1;
    }

    bool isEmpty() const {
        return topIndex == -1;
    }

    int size() const {
        return topIndex + 1;
    }

    bool push(QuestionNode* question) {
        if (isFull()) {
            std::cout << UI::RED << "  [!] Stack Overflow: Review stack is completely full.\n" << UI::RESET;
            return false;
        }
        stackArray[++topIndex] = question;
        return true;
    }

    QuestionNode* pop() {
        if (isEmpty()) {
            std::cout << UI::RED << "  [!] Stack Underflow: Review stack is empty.\n" << UI::RESET;
            return nullptr;
        }
        return stackArray[topIndex--];
    }

    QuestionNode* peek() const {
        if (isEmpty()) return nullptr;
        return stackArray[topIndex];
    }

    void displayStatus() const {
        std::cout << "\n" << UI::MAGENTA << "  [Review Stack Status - Lab Week 7 LIFO ADT]" << UI::RESET << "\n";
        std::cout << "  Capacity: " << MAX_STACK << " | Items in Stack: " << (topIndex + 1) 
                  << " | Top Index: " << topIndex << "\n";
        
        if (isEmpty()) {
            std::cout << "  [Stack is currently empty]\n\n";
            return;
        }

        std::cout << "  Stack Elements (Top to Bottom):\n";
        for (int i = topIndex; i >= 0; --i) {
            std::cout << "    [" << i << "] Q#" << stackArray[i]->id << ": " 
                      << stackArray[i]->questionText.substr(0, 45) << "...\n";
        }
        std::cout << "\n";
    }
};

#endif // REVIEW_STACK_HPP
