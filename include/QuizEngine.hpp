#ifndef QUIZ_ENGINE_HPP
#define QUIZ_ENGINE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <chrono>

#include "UI.hpp"
#include "Question.hpp"
#include "Leaderboard.hpp"
#include "MultiplayerQueue.hpp"
#include "ReviewStack.hpp"
#include "ExpressionEvaluator.hpp"
#include "SkillGraph.hpp"

class QuizEngine {
private:
    QuestionDoublyLinkedList questionBank;
    LeaderboardBST leaderboard;
    SkillGraph skillGraph;
    ReviewStack reviewStack;

    void loadQuestions() {
        // Module 1: Arrays & Recursion
        questionBank.addQuestion(101, "Arrays & Strings", "Easy",
            "What is the time complexity of accessing an element in an array by its index?",
            {"O(1)", "O(n)", "O(log n)", "O(n^2)"}, 1,
            "Direct index access is calculated as Base_Address + (Index * Element_Size), which executes in constant time O(1).", 10);

        questionBank.addQuestion(102, "Arrays & Strings", "Medium",
            "In linear search vs binary search, what is the prerequisite condition for binary search?",
            {"Array must have even size", "Array must be sorted", "Array must contain only positive integers", "Array must be a dynamic pointer"}, 2,
            "Binary search requires the elements to be sorted in monotonic order to eliminate half the search space in each iteration.", 15);

        // Module 2: Stacks & Queues
        questionBank.addQuestion(201, "Stacks & Queues", "Easy",
            "Which of the following data structures operates on the Last-In-First-Out (LIFO) principle?",
            {"Queue", "Stack", "Priority Queue", "Circular Array"}, 2,
            "A Stack operates strictly on LIFO order: elements pushed last are popped first.", 10);

        questionBank.addQuestion(202, "Stacks & Queues", "Medium",
            "In a Circular Queue of size N, what is the standard mathematical condition for Queue Full?",
            {"(rear + 1) % N == front", "rear == front", "rear == N - 1", "front == (rear + 1)"}, 1,
            "In circular array representation, the queue is full when advancing rear by 1 position (modulo N) lands on the front index.", 15);

        // Module 3: Linked Lists & Trees
        questionBank.addQuestion(301, "Linked Lists", "Easy",
            "What is the primary advantage of a Singly Linked List over a contiguous static array?",
            {"Instant O(1) random access by index", "Dynamic size and efficient O(1) insertion at head without shifting", "Zero pointer memory overhead", "Automatic contiguous caching"}, 2,
            "Linked lists allocate memory dynamically on the heap; nodes can be inserted without shifting existing elements.", 10);

        questionBank.addQuestion(401, "Trees & BST", "Medium",
            "What type of tree traversal on a Binary Search Tree (BST) visits keys in strictly ascending sorted order?",
            {"Pre-order Traversal", "Post-order Traversal", "In-order Traversal", "Level-order Traversal"}, 3,
            "In-order traversal visits Left Subtree -> Root -> Right Subtree, which yields keys in ascending order for any valid BST.", 15);

        questionBank.addQuestion(402, "Trees & BST", "Hard",
            "What is the worst-case time complexity of searching in a standard un-balanced Binary Search Tree (BST)?",
            {"O(log n)", "O(1)", "O(n)", "O(n log n)"}, 3,
            "If keys are inserted in sorted order, the BST degenerates into a linear linked list (skewed tree), causing worst-case search of O(n).", 20);

        // Module 5: Graphs
        questionBank.addQuestion(501, "Graphs & Shortest Path", "Medium",
            "Which graph traversal algorithm uses a FIFO Queue and is guaranteed to find the shortest path in an unweighted graph?",
            {"Depth First Search (DFS)", "Breadth First Search (BFS)", "Topological Sort", "Kruskal's Algorithm"}, 2,
            "BFS explores nodes layer by layer using a Queue, guaranteeing shortest path in unweighted networks.", 15);

        questionBank.addQuestion(502, "Graphs & Shortest Path", "Hard",
            "What is the primary prerequisite graph property required to compute a valid Topological Sort?",
            {"Graph must be undirected and connected", "Graph must be a Directed Acyclic Graph (DAG)", "Graph must have non-negative edge weights only", "Graph must be bipartite"}, 2,
            "Topological sorting is only valid for Directed Acyclic Graphs (DAGs) because directed cycles create circular dependencies.", 20);
    }

    void showHeader(const std::string& title) {
        UI::clearScreen();
        UI::printBanner();
        std::cout << UI::BG_CYAN << UI::BOLD << "  >>> " << title << " <<<  " << UI::RESET << "\n\n";
    }

public:
    QuizEngine() {
        UI::initTerminal();
        loadQuestions();
    }

    void runSoloCampaign() {
        showHeader("SOLO CAMPAIGN: SKILL TREE MASTERY");
        skillGraph.displayCampaignMap();

        std::cout << "  Enter Topic Number (1 to 6) to battle (or 0 to return): ";
        int choice = 0;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            return;
        }
        std::cin.ignore(1000, '\n');

        if (choice < 1 || choice > 6) return;
        int topicIdx = choice - 1;

        if (!skillGraph.isTopicUnlocked(topicIdx)) {
            std::cout << "\n" << UI::RED << UI::BOLD << "  [!] ACCESS DENIED: Topic is LOCKED 🔒." << UI::RESET << "\n";
            std::cout << "  You must complete its prerequisite topics in the graph first!\n";
            skillGraph.printReachableDFS(0);
            UI::pressAnyKey();
            return;
        }

        std::string selectedTopic = skillGraph.getTopicName(topicIdx);
        std::cout << "\n" << UI::GREEN << "  Topic Unlocked! Preparing question stream from Doubly Linked List...\n" << UI::RESET;
        UI::sleepMs(700);

        std::string playerName;
        std::cout << "  Enter your Gladiator Handle: ";
        std::getline(std::cin, playerName);
        if (playerName.empty()) playerName = "SoloWarrior";

        int score = 0;
        int streak = 0;
        int maxStreak = 0;
        int answeredCount = 0;
        int correctCount = 0;

        QuestionNode* curr = questionBank.getHead();
        while (curr) {
            if (curr->topic == selectedTopic) {
                answeredCount++;
                showHeader("BATTLE TOPIC: " + selectedTopic);
                UI::printProgressBar(answeredCount, 3);

                std::cout << "\n  " << UI::BOLD << "Q" << curr->id << " [" << curr->difficulty << " | " << curr->points << " pts]" << UI::RESET << "\n";
                std::cout << "  " << curr->questionText << "\n\n";

                for (size_t i = 0; i < curr->options.size(); ++i) {
                    std::cout << "    " << UI::CYAN << "[" << (i + 1) << "]" << UI::RESET << " " << curr->options[i] << "\n";
                }

                std::cout << "\n    " << UI::YELLOW << "[5] Mark for Review (Push onto Review Stack)" << UI::RESET << "\n";
                std::cout << "    " << UI::GRAY << "[0] Abandon Battle" << UI::RESET << "\n\n";
                std::cout << "  Your Move (1-5): ";

                int ans = 0;
                if (!(std::cin >> ans)) {
                    std::cin.clear();
                    std::cin.ignore(1000, '\n');
                    continue;
                }
                std::cin.ignore(1000, '\n');

                if (ans == 0) break;

                if (ans == 5) {
                    reviewStack.push(curr);
                    std::cout << UI::MAGENTA << "  ➔ Pushed Q#" << curr->id << " to Review Stack (LIFO)!\n" << UI::RESET;
                    UI::sleepMs(800);
                } else if (ans == curr->correctOption) {
                    streak++;
                    if (streak > maxStreak) maxStreak = streak;
                    int earned = curr->points + (streak > 1 ? (streak * 2) : 0);
                    score += earned;
                    correctCount++;
                    std::cout << "\n  " << UI::BG_GREEN << UI::BOLD << " CORRECT! ✔ " << UI::RESET 
                              << " Earned " << earned << " pts";
                    if (streak > 1) std::cout << " (" << streak << "x Streak Multiplier 🔥)";
                    std::cout << "\n";
                    std::cout << UI::DIM << "  Explanation: " << curr->explanation << UI::RESET << "\n";
                    UI::sleepMs(1200);
                } else {
                    streak = 0;
                    std::cout << "\n  " << UI::BG_RED << UI::BOLD << " INCORRECT! ✖ " << UI::RESET 
                              << " Correct answer was option [" << curr->correctOption << "]\n";
                    std::cout << UI::DIM << "  Explanation: " << curr->explanation << UI::RESET << "\n";
                    UI::sleepMs(1400);
                }
            }
            curr = curr->next;
        }

        // Handle Review Stack if items were marked
        if (!reviewStack.isEmpty()) {
            showHeader("REVIEW STACK RESOLUTION (LIFO POPPING)");
            reviewStack.displayStatus();
            std::cout << "  Resolving marked questions popped from Stack:\n\n";

            while (!reviewStack.isEmpty()) {
                QuestionNode* revQ = reviewStack.pop();
                std::cout << "  Popped Q#" << revQ->id << ": " << revQ->questionText << "\n";
                for (size_t i = 0; i < revQ->options.size(); ++i) {
                    std::cout << "    [" << (i + 1) << "] " << revQ->options[i] << "\n";
                }
                std::cout << "  Second Chance Answer (1-4): ";
                int ans = 0;
                std::cin >> ans;
                std::cin.ignore(1000, '\n');

                if (ans == revQ->correctOption) {
                    score += (revQ->points / 2);
                    std::cout << UI::GREEN << "  Correct! Recovered partial points: +" << (revQ->points / 2) << "\n" << UI::RESET;
                } else {
                    std::cout << UI::RED << "  Incorrect. No recovery.\n" << UI::RESET;
                }
                std::cout << "\n";
            }
        }

        float accuracy = answeredCount > 0 ? ((float)correctCount / answeredCount) * 100.0f : 0.0f;
        if (accuracy >= 50.0f) {
            skillGraph.markCompleted(topicIdx);
            std::cout << "\n" << UI::GREEN << UI::BOLD << "  CONGRATULATIONS! Topic '" << selectedTopic << "' MASTERED and Marked in Graph! ✔" << UI::RESET << "\n";
        }

        // Insert score into BST Leaderboard
        leaderboard.insert(playerName, score, maxStreak, accuracy);
        std::cout << UI::CYAN << "  Performance registered into Global Leaderboard (BST)!\n" << UI::RESET;

        UI::pressAnyKey();
    }

    void runMultiplayerArena() {
        showHeader("PASS-AND-PLAY MULTIPLAYER ARENA (CIRCULAR QUEUE)");
        CircularTurnQueue turnQueue;

        std::cout << "  Enter number of players (2 to 4): ";
        int n = 2;
        if (!(std::cin >> n)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            return;
        }
        std::cin.ignore(1000, '\n');
        if (n < 2) n = 2;
        if (n > 4) n = 4;

        for (int i = 1; i <= n; ++i) {
            std::string name;
            std::cout << "  Enter Player " << i << " name: ";
            std::getline(std::cin, name);
            if (name.empty()) name = "Player_" + std::to_string(i);
            turnQueue.enqueue(ArenaPlayer(i, name));
        }

        std::cout << "\n" << UI::GREEN << "  All players enqueued into Circular Buffer!\n" << UI::RESET;
        turnQueue.displayQueueStatus();
        UI::sleepMs(1000);

        QuestionNode* curr = questionBank.getHead();
        int round = 1;

        while (curr && round <= (n * 2)) {
            ArenaPlayer* activePlayer = turnQueue.getCurrentPlayer();
            if (!activePlayer) break;

            showHeader("ROUND " + std::to_string(round) + " - MULTIPLAYER ARENA");
            turnQueue.displayQueueStatus();

            std::cout << UI::BG_MAGENTA << UI::BOLD << "  >>> TURN: " << activePlayer->name << " <<<  " << UI::RESET << "\n\n";
            std::cout << "  Q#" << curr->id << " [" << curr->topic << " - " << curr->points << " pts]\n";
            std::cout << "  " << curr->questionText << "\n\n";

            for (size_t i = 0; i < curr->options.size(); ++i) {
                std::cout << "    " << UI::CYAN << "[" << (i + 1) << "]" << UI::RESET << " " << curr->options[i] << "\n";
            }

            std::cout << "\n  " << activePlayer->name << ", choose your answer (1-4): ";
            int ans = 0;
            std::cin >> ans;
            std::cin.ignore(1000, '\n');

            activePlayer->totalAnswered++;
            if (ans == curr->correctOption) {
                activePlayer->streak++;
                activePlayer->score += curr->points;
                activePlayer->correctAnswers++;
                std::cout << UI::GREEN << UI::BOLD << "\n  CORRECT! " << activePlayer->name << " gets +" << curr->points << " points!\n" << UI::RESET;
            } else {
                activePlayer->streak = 0;
                std::cout << UI::RED << UI::BOLD << "\n  WRONG ANSWER! Better luck next turn.\n" << UI::RESET;
            }

            // Circular Queue turn pass
            turnQueue.passTurn();
            round++;
            curr = curr->next;
            UI::sleepMs(1200);
        }

        showHeader("MULTIPLAYER MATCH RESULTS");
        std::cout << UI::YELLOW << UI::BOLD << "  Final Scores:\n" << UI::RESET;
        for (int i = 0; i < turnQueue.size(); ++i) {
            ArenaPlayer* p = turnQueue.getPlayerAtSlot(i);
            if (p) {
                float acc = p->totalAnswered > 0 ? ((float)p->correctAnswers / p->totalAnswered) * 100.0f : 0.0f;
                std::cout << "  • " << std::left << std::setw(18) << p->name 
                          << ": " << UI::GREEN << p->score << " pts" << UI::RESET 
                          << " | Accuracy: " << std::fixed << std::setprecision(1) << acc << "%\n";
                leaderboard.insert(p->name, p->score, p->streak, acc);
            }
        }

        std::cout << "\n" << UI::CYAN << "  [All results committed to Global BST Leaderboard]\n" << UI::RESET;
        UI::pressAnyKey();
    }

    void runMathBlitzChallenge() {
        showHeader("MATH & LOGIC BLITZ: INFIX TO POSTFIX EVALUATOR (LAB WEEK 8)");
        std::cout << "  Demonstrates Stack-based Infix conversion and expression calculation.\n\n";

        std::vector<std::pair<std::string, std::string>> challenges = {
            {"(3 + 5) * 2 ^ 2", "Expression with brackets and exponentiation"},
            {"10 + 2 * 6", "Precedence of multiplication over addition"},
            {"(100 - 20) / 4 + 3 ^ 2", "Complex parenthesized multi-operator expression"}
        };

        int score = 0;
        for (size_t i = 0; i < challenges.size(); ++i) {
            std::cout << UI::CYAN << "  Challenge " << (i + 1) << ": " << UI::RESET << challenges[i].first << "\n";
            std::cout << "  Note: " << challenges[i].second << "\n";

            std::string postfix = ExpressionEvaluator::infixToPostfix(challenges[i].first);
            double expectedVal = ExpressionEvaluator::evaluatePostfix(postfix);

            std::cout << UI::GRAY << "  [System Stack converting to Postfix: " << postfix << "]\n" << UI::RESET;
            std::cout << "  Calculate the result: ";

            double userVal = 0.0;
            if (std::cin >> userVal) {
                if (std::abs(userVal - expectedVal) < 0.001) {
                    std::cout << UI::GREEN << UI::BOLD << "  CORRECT! Calculation matches Stack Evaluator. (+25 pts)\n\n" << UI::RESET;
                    score += 25;
                } else {
                    std::cout << UI::RED << "  INCORRECT! Correct Evaluated Value = " << expectedVal << "\n\n" << UI::RESET;
                }
            }
            std::cin.ignore(1000, '\n');
        }

        std::cout << "  Total Math Blitz Score: " << UI::GREEN << score << " pts\n" << UI::RESET;
        leaderboard.insert("MathGladiator", score, 3, 100.0f);
        UI::pressAnyKey();
    }

    void viewLeaderboard() {
        showHeader("HALL OF FAME (BST LEADERBOARD)");
        leaderboard.display();
        leaderboard.renderTreeHierarchy();
        UI::pressAnyKey();
    }

    void viewSkillGraph() {
        showHeader("GRAPH TOPOLOGY & REACHABILITY INSPECTION (LAB WEEK 10)");
        skillGraph.displayAdjacencyMatrix();
        skillGraph.printReachableBFS(0);
        skillGraph.printReachableDFS(0);
        skillGraph.printTopologicalRoadmap();
        UI::pressAnyKey();
    }

    void start() {
        while (true) {
            showHeader("MAIN ARENA DASHBOARD");
            std::cout << "  " << UI::BOLD << "SELECT GAME MODE:" << UI::RESET << "\n\n";
            std::cout << "  " << UI::CYAN << "[1]" << UI::RESET << "  ⚔️  Solo Campaign (Graph Skill Tree & Doubly Linked List)\n";
            std::cout << "  " << UI::CYAN << "[2]" << UI::RESET << "  👥  Multiplayer Pass-and-Play (Circular Queue Turn Engine)\n";
            std::cout << "  " << UI::CYAN << "[3]" << UI::RESET << "  ⚡  Math & Logic Blitz (Infix/Postfix Stack Evaluator)\n";
            std::cout << "  " << UI::CYAN << "[4]" << UI::RESET << "  🏆  Leaderboard & BST Visualizer (Reverse In-Order Traversal)\n";
            std::cout << "  " << UI::CYAN << "[5]" << UI::RESET << "  🗺️   Inspect Graph Adjacency Matrix & BFS/DFS Traversal\n";
            std::cout << "  " << UI::CYAN << "[6]" << UI::RESET << "  🚪  Exit Arena\n\n";
            std::cout << "  Enter Choice (1-6): ";

            int choice = 0;
            if (!(std::cin >> choice)) {
                std::cin.clear();
                std::cin.ignore(1000, '\n');
                continue;
            }
            std::cin.ignore(1000, '\n');

            switch (choice) {
                case 1: runSoloCampaign(); break;
                case 2: runMultiplayerArena(); break;
                case 3: runMathBlitzChallenge(); break;
                case 4: viewLeaderboard(); break;
                case 5: viewSkillGraph(); break;
                case 6:
                    std::cout << "\n" << UI::GREEN << "  Thank you for playing DSA Quiz Arena! Keep coding.\n\n" << UI::RESET;
                    return;
                default:
                    std::cout << UI::RED << "  Invalid choice. Try again.\n" << UI::RESET;
                    UI::sleepMs(700);
            }
        }
    }
};

#endif // QUIZ_ENGINE_HPP
