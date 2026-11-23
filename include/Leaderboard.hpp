#ifndef LEADERBOARD_HPP
#define LEADERBOARD_HPP

#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include "UI.hpp"

struct PlayerNode {
    std::string name;
    int score;
    int streak;
    float accuracy;
    PlayerNode* left;
    PlayerNode* right;

    PlayerNode(const std::string& n, int s, int st, float acc)
        : name(n), score(s), streak(st), accuracy(acc), left(nullptr), right(nullptr) {}
};

class LeaderboardBST {
private:
    PlayerNode* root;

    PlayerNode* insertRec(PlayerNode* node, const std::string& name, int score, int streak, float acc) {
        if (!node) {
            return new PlayerNode(name, score, streak, acc);
        }
        if (score <= node->score) {
            node->left = insertRec(node->left, name, score, streak, acc);
        } else {
            node->right = insertRec(node->right, name, score, streak, acc);
        }
        return node;
    }

    void reverseInOrder(PlayerNode* node, int& rank, int limit) const {
        if (!node || rank > limit) return;

        // Traverse right subtree first (higher scores)
        reverseInOrder(node->right, rank, limit);

        if (rank <= limit) {
            std::string rankBadge;
            if (rank == 1) rankBadge = UI::YELLOW + " 🥇 #1 " + UI::RESET;
            else if (rank == 2) rankBadge = UI::WHITE + " 🥈 #2 " + UI::RESET;
            else if (rank == 3) rankBadge = UI::CYAN + " 🥉 #3 " + UI::RESET;
            else rankBadge = "    #" + std::to_string(rank) + " ";

            std::cout << "  " << std::left << std::setw(14) << rankBadge
                      << std::left << std::setw(20) << (UI::BOLD + node->name + UI::RESET)
                      << UI::GREEN << std::right << std::setw(8) << node->score << " pts" << UI::RESET
                      << "   " << UI::CYAN << std::right << std::setw(6) << node->streak << " streak" << UI::RESET
                      << "   " << UI::YELLOW << std::right << std::fixed << std::setprecision(1) << node->accuracy << "%" << UI::RESET
                      << "\n";
            rank++;
        }

        // Traverse left subtree
        reverseInOrder(node->left, rank, limit);
    }

    void printTreeVisual(PlayerNode* node, const std::string& prefix, bool isLeft) const {
        if (!node) return;

        std::cout << prefix;
        std::cout << (isLeft ? "├── " : "└── ");
        std::cout << UI::CYAN << node->name << UI::RESET << " (" << UI::GREEN << node->score << " pts" << UI::RESET << ")\n";

        std::string nextPrefix = prefix + (isLeft ? "│   " : "    ");
        if (node->right) printTreeVisual(node->right, nextPrefix, true);
        if (node->left)  printTreeVisual(node->left, nextPrefix, false);
    }

    void destroyTree(PlayerNode* node) {
        if (!node) return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }

public:
    LeaderboardBST() : root(nullptr) {
        // Pre-populate with realistic bot benchmark scores
        insert("CodeMaster_Neo", 140, 5, 93.3f);
        insert("AlgorithmNinja", 110, 4, 85.0f);
        insert("ByteHero", 80, 2, 72.5f);
        insert("SyntaxSamurai", 60, 2, 66.0f);
    }

    ~LeaderboardBST() {
        destroyTree(root);
    }

    void insert(const std::string& name, int score, int streak, float acc) {
        root = insertRec(root, name, score, streak, acc);
    }

    void display(int limit = 10) const {
        std::cout << "\n" << UI::CYAN << UI::BOLD 
                  << "  ═══════════════════════════════════════════════════════════════════════\n"
                  << "                     🏆 ARENA GLOBAL LEADERBOARD (BST)                  \n"
                  << "  ═══════════════════════════════════════════════════════════════════════" 
                  << UI::RESET << "\n";
        std::cout << UI::GRAY << "   RANK         PLAYER              SCORE        MAX STREAK    ACCURACY\n"
                  << "  ───────────────────────────────────────────────────────────────────────\n" << UI::RESET;
        
        int rank = 1;
        reverseInOrder(root, rank, limit);
        std::cout << UI::GRAY << "  ───────────────────────────────────────────────────────────────────────\n" << UI::RESET;
        std::cout << UI::DIM << "  [Data Structure: Binary Search Tree • Sorted via Reverse In-Order Traversal]\n\n" << UI::RESET;
    }

    void renderTreeHierarchy() const {
        std::cout << "\n" << UI::YELLOW << UI::BOLD 
                  << "  [BST Internal Node Hierarchy Representation]\n" << UI::RESET;
        if (!root) {
            std::cout << "  (Empty BST)\n";
            return;
        }
        std::cout << UI::BOLD << "  ROOT: " << root->name << " (" << root->score << " pts)\n" << UI::RESET;
        if (root->right) printTreeVisual(root->right, "  ", true);
        if (root->left)  printTreeVisual(root->left, "  ", false);
        std::cout << "\n";
    }
};

#endif // LEADERBOARD_HPP
