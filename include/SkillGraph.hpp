#ifndef SKILL_GRAPH_HPP
#define SKILL_GRAPH_HPP

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <stack>
#include <iomanip>
#include "UI.hpp"

// Implements Graph of Topics with Adjacency Matrix, BFS/DFS & Topological Sort
// Directly satisfies Lab Week 10 and Module 5
class SkillGraph {
private:
    static const int NUM_TOPICS = 6;
    std::string topicNames[NUM_TOPICS];
    int adjMatrix[NUM_TOPICS][NUM_TOPICS];
    bool completed[NUM_TOPICS];

public:
    SkillGraph() {
        topicNames[0] = "Arrays & Strings";
        topicNames[1] = "Stacks & Queues";
        topicNames[2] = "Linked Lists";
        topicNames[3] = "Recursion & Backtracking";
        topicNames[4] = "Trees & BST";
        topicNames[5] = "Graphs & Shortest Path";

        // Initialize Adjacency Matrix with 0
        for (int i = 0; i < NUM_TOPICS; ++i) {
            completed[i] = false;
            for (int j = 0; j < NUM_TOPICS; ++j) {
                adjMatrix[i][j] = 0;
            }
        }

        // Define prerequisite dependencies (Directed Edges i -> j means i is prerequisite for j)
        // Arrays (0) -> Stacks (1), Linked Lists (2), Recursion (3)
        adjMatrix[0][1] = 1;
        adjMatrix[0][2] = 1;
        adjMatrix[0][3] = 1;

        // Stacks (1) & Linked Lists (2) -> Trees (4)
        adjMatrix[1][4] = 1;
        adjMatrix[2][4] = 1;

        // Recursion (3) & Trees (4) -> Graphs (5)
        adjMatrix[3][5] = 1;
        adjMatrix[4][5] = 1;

        // By default, Foundation topic 0 is unlocked
        completed[0] = true;
    }

    void markCompleted(int topicIndex) {
        if (topicIndex >= 0 && topicIndex < NUM_TOPICS) {
            completed[topicIndex] = true;
        }
    }

    bool isTopicUnlocked(int topicIndex) const {
        if (topicIndex < 0 || topicIndex >= NUM_TOPICS) return false;
        if (topicIndex == 0) return true; // Base node always unlocked

        // Check if all prerequisites pointing into topicIndex are completed
        for (int i = 0; i < NUM_TOPICS; ++i) {
            if (adjMatrix[i][topicIndex] == 1 && !completed[i]) {
                return false;
            }
        }
        return true;
    }

    std::string getTopicName(int idx) const {
        if (idx >= 0 && idx < NUM_TOPICS) return topicNames[idx];
        return "Unknown";
    }

    // Lab Week 10: Print all reachable nodes using BFS
    void printReachableBFS(int startNode) const {
        if (startNode < 0 || startNode >= NUM_TOPICS) return;

        bool visited[NUM_TOPICS] = {false};
        std::queue<int> q;

        visited[startNode] = true;
        q.push(startNode);

        std::cout << UI::CYAN << "  Reachable topics from [" << topicNames[startNode] << "] via BFS:\n  " << UI::RESET;
        bool first = true;
        while (!q.empty()) {
            int curr = q.front();
            q.pop();

            if (!first) std::cout << " ➔ ";
            std::cout << UI::BOLD << topicNames[curr] << UI::RESET;
            first = false;

            for (int j = 0; j < NUM_TOPICS; ++j) {
                if (adjMatrix[curr][j] == 1 && !visited[j]) {
                    visited[j] = true;
                    q.push(j);
                }
            }
        }
        std::cout << "\n";
    }

    // Lab Week 10: DFS Helper
    void dfsUtil(int node, bool visited[]) const {
        visited[node] = true;
        std::cout << UI::BOLD << topicNames[node] << UI::RESET << " ";

        for (int j = 0; j < NUM_TOPICS; ++j) {
            if (adjMatrix[node][j] == 1 && !visited[j]) {
                std::cout << "➔ ";
                dfsUtil(j, visited);
            }
        }
    }

    // Lab Week 10: Print reachable nodes using DFS
    void printReachableDFS(int startNode) const {
        if (startNode < 0 || startNode >= NUM_TOPICS) return;
        bool visited[NUM_TOPICS] = {false};
        std::cout << UI::MAGENTA << "  Reachable topics from [" << topicNames[startNode] << "] via DFS:\n  " << UI::RESET;
        dfsUtil(startNode, visited);
        std::cout << "\n";
    }

    // Module 5 Theory: Topological Sort (Kahn's Algorithm using In-Degrees)
    void printTopologicalRoadmap() const {
        int inDegree[NUM_TOPICS] = {0};
        for (int i = 0; i < NUM_TOPICS; ++i) {
            for (int j = 0; j < NUM_TOPICS; ++j) {
                if (adjMatrix[i][j] == 1) inDegree[j]++;
            }
        }

        std::queue<int> q;
        for (int i = 0; i < NUM_TOPICS; ++i) {
            if (inDegree[i] == 0) q.push(i);
        }

        std::cout << UI::YELLOW << UI::BOLD << "  [Recommended Learning Sequence (Topological Sort)]:\n  " << UI::RESET;
        int count = 0;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            count++;
            std::cout << UI::GREEN << count << ". " << topicNames[u] << UI::RESET;
            if (count < NUM_TOPICS) std::cout << "  ➔  \n  ";

            for (int v = 0; v < NUM_TOPICS; ++v) {
                if (adjMatrix[u][v] == 1) {
                    if (--inDegree[v] == 0) {
                        q.push(v);
                    }
                }
            }
        }
        std::cout << "\n\n";
    }

    // Display Adjacency Matrix
    void displayAdjacencyMatrix() const {
        std::cout << UI::CYAN << "  Adjacency Matrix Representation (Lab Week 10):\n" << UI::RESET;
        std::cout << "      ";
        for (int i = 0; i < NUM_TOPICS; ++i) std::cout << " [" << i << "]";
        std::cout << "\n";

        for (int i = 0; i < NUM_TOPICS; ++i) {
            std::cout << "  [" << i << "] ";
            for (int j = 0; j < NUM_TOPICS; ++j) {
                if (adjMatrix[i][j] == 1) {
                    std::cout << UI::GREEN << "  1 " << UI::RESET;
                } else {
                    std::cout << UI::GRAY << "  0 " << UI::RESET;
                }
            }
            std::cout << "  " << topicNames[i] << "\n";
        }
        std::cout << "\n";
    }

    void displayCampaignMap() const {
        std::cout << "\n" << UI::CYAN << UI::BOLD 
                  << "  ═══════════════════════════════════════════════════════════════════════\n"
                  << "                  🗺️  DSA MASTERY CAMPAIGN MAP (GRAPH)                  \n"
                  << "  ═══════════════════════════════════════════════════════════════════════" 
                  << UI::RESET << "\n\n";

        for (int i = 0; i < NUM_TOPICS; ++i) {
            bool unlocked = isTopicUnlocked(i);
            std::cout << "  " << (i + 1) << ". ";
            if (completed[i]) {
                std::cout << UI::GREEN << UI::BOLD << "[COMPLETED ✔] " << UI::RESET << std::left << std::setw(28) << topicNames[i];
            } else if (unlocked) {
                std::cout << UI::YELLOW << UI::BOLD << "[UNLOCKED  ⭐] " << UI::RESET << std::left << std::setw(28) << topicNames[i];
            } else {
                std::cout << UI::GRAY << "[LOCKED    🔒] " << UI::RESET << std::left << std::setw(28) << topicNames[i];
            }

            // Show prerequisites
            std::cout << UI::GRAY << "Prerequisites: ";
            bool hasPrereq = false;
            for (int p = 0; p < NUM_TOPICS; ++p) {
                if (adjMatrix[p][i] == 1) {
                    std::cout << topicNames[p] << ", ";
                    hasPrereq = true;
                }
            }
            if (!hasPrereq) std::cout << "None (Root Entry)";
            std::cout << UI::RESET << "\n";
        }
        std::cout << "\n";
    }
};

#endif // SKILL_GRAPH_HPP
