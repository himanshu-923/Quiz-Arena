#ifndef MULTIPLAYER_QUEUE_HPP
#define MULTIPLAYER_QUEUE_HPP

#include <iostream>
#include <string>
#include "UI.hpp"

struct ArenaPlayer {
    int id;
    std::string name;
    int score;
    int streak;
    int correctAnswers;
    int totalAnswered;

    ArenaPlayer(int pId = 0, const std::string& pName = "")
        : id(pId), name(pName), score(0), streak(0), correctAnswers(0), totalAnswered(0) {}
};

// Implements Circular Queue matching Lab Week 9 requirements
class CircularTurnQueue {
private:
    static const int MAX_SIZE = 4;
    ArenaPlayer queue[MAX_SIZE];
    int front;
    int rear;
    int count;

public:
    CircularTurnQueue() : front(0), rear(-1), count(0) {}

    bool isFull() const {
        return count == MAX_SIZE;
    }

    bool isEmpty() const {
        return count == 0;
    }

    int size() const {
        return count;
    }

    bool enqueue(const ArenaPlayer& player) {
        if (isFull()) {
            std::cout << UI::RED << "  [!] Circular Queue Overflow: Maximum player capacity (" << MAX_SIZE << ") reached.\n" << UI::RESET;
            return false;
        }
        rear = (rear + 1) % MAX_SIZE;
        queue[rear] = player;
        count++;
        return true;
    }

    bool dequeue(ArenaPlayer& removedPlayer) {
        if (isEmpty()) {
            std::cout << UI::RED << "  [!] Circular Queue Underflow: No players in queue.\n" << UI::RESET;
            return false;
        }
        removedPlayer = queue[front];
        front = (front + 1) % MAX_SIZE;
        count--;
        return true;
    }

    ArenaPlayer* getCurrentPlayer() {
        if (isEmpty()) return nullptr;
        return &queue[front];
    }

    void passTurn() {
        if (count <= 1) return;
        // Rotate current player from front to rear
        ArenaPlayer current = queue[front];
        front = (front + 1) % MAX_SIZE;
        rear = (rear + 1) % MAX_SIZE;
        queue[rear] = current;
    }

    void displayQueueStatus() const {
        std::cout << "\n" << UI::CYAN << "  [Circular Turn Queue Status - Lab Week 9 ADT]" << UI::RESET << "\n";
        std::cout << "  Capacity: " << MAX_SIZE << " | Active Players: " << count 
                  << " | Front Index: " << front << " | Rear Index: " << rear << "\n";
        std::cout << "  Turn Order: ";
        for (int i = 0; i < count; ++i) {
            int idx = (front + i) % MAX_SIZE;
            if (i == 0) {
                std::cout << UI::GREEN << UI::BOLD << "[" << queue[idx].name << " (UP NOW)]" << UI::RESET << " ➔ ";
            } else {
                std::cout << UI::WHITE << queue[idx].name << UI::RESET << (i < count - 1 ? " ➔ " : "");
            }
        }
        std::cout << " (Loops to Front)\n\n";
    }

    ArenaPlayer* getPlayerAtSlot(int i) {
        if (i < 0 || i >= count) return nullptr;
        int idx = (front + i) % MAX_SIZE;
        return &queue[idx];
    }
};

#endif // MULTIPLAYER_QUEUE_HPP
