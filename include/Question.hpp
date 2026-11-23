#ifndef QUESTION_HPP
#define QUESTION_HPP

#include <string>
#include <vector>
#include <iostream>

struct QuestionNode {
    int id;
    std::string topic;
    std::string difficulty; // Easy, Medium, Hard
    std::string questionText;
    std::vector<std::string> options; // 4 choices
    int correctOption; // 1-based index (1 to 4)
    std::string explanation;
    int points;

    QuestionNode* prev;
    QuestionNode* next;

    QuestionNode(int qId, const std::string& top, const std::string& diff,
                 const std::string& text, const std::vector<std::string>& opts,
                 int correct, const std::string& exp, int pts)
        : id(qId), topic(top), difficulty(diff), questionText(text),
          options(opts), correctOption(correct), explanation(exp), points(pts),
          prev(nullptr), next(nullptr) {}
};

class QuestionDoublyLinkedList {
private:
    QuestionNode* head;
    QuestionNode* tail;
    int nodeCount;

public:
    QuestionDoublyLinkedList() : head(nullptr), tail(nullptr), nodeCount(0) {}

    ~QuestionDoublyLinkedList() {
        clear();
    }

    void addQuestion(int id, const std::string& topic, const std::string& diff,
                     const std::string& text, const std::vector<std::string>& opts,
                     int correct, const std::string& exp, int pts = 10) {
        QuestionNode* newNode = new QuestionNode(id, topic, diff, text, opts, correct, exp, pts);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        nodeCount++;
    }

    void clear() {
        QuestionNode* curr = head;
        while (curr) {
            QuestionNode* nxt = curr->next;
            delete curr;
            curr = nxt;
        }
        head = tail = nullptr;
        nodeCount = 0;
    }

    QuestionNode* getHead() const { return head; }
    QuestionNode* getTail() const { return tail; }
    int size() const { return nodeCount; }
    bool isEmpty() const { return nodeCount == 0; }
};

#endif // QUESTION_HPP
