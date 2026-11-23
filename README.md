# ⚔️ DSA Quiz Arena: High-Performance Competitive Platform in C++

A comprehensive, interactive C++ console application built directly around the **UCS201 / UCS201P (Data Structures and Algorithms)** university syllabus.

---

## 🎯 Syllabus & Lab Mapping Table

Every feature in Quiz Arena was built using custom data structures implemented from first principles (without STL container shortcuts for core algorithms):

| Syllabus Unit / Lab Week | Data Structure / Algorithm | Role in Quiz Arena | Implementation File |
| :--- | :--- | :--- | :--- |
| **Lab Week 4 / Module 3** | **Doubly Linked List** | Sequential question stream (`prev` / `next` navigation without array copying). | `include/Question.hpp` |
| **Lab Week 3 / Module 3** | **Binary Search Tree (BST)** | Real-time Global Leaderboard. Uses **Reverse In-Order Traversal** to rank scores descending. Features an ASCII tree hierarchy renderer. | `include/Leaderboard.hpp` |
| **Lab Week 9 / Module 2** | **Circular Queue** | Multiplayer pass-and-play turn engine with overflow/underflow handling and circular rotation. | `include/MultiplayerQueue.hpp` |
| **Lab Week 7 / Module 2** | **Stack (LIFO)** | "Mark for Review" lifeline buffer: skipped questions are pushed onto the stack and popped in reverse order during the review phase. | `include/ReviewStack.hpp` |
| **Lab Week 8 / Module 2** | **Infix to Postfix & Evaluation** | Math & Logic Blitz challenge: dynamic expression evaluation using operator precedence and operand stack. | `include/ExpressionEvaluator.hpp` |
| **Lab Week 10 / Module 5** | **Graph (Adjacency Matrix)** | Campaign Skill Tree: directed graph of topic prerequisites. Implements **BFS**, **DFS**, and **Topological Sort**. | `include/SkillGraph.hpp` |
| **Lab Week 1 / Module 4** | **Sorting & Searching** | Binary Search on Question IDs; ranking sort for tournament brackets. | `include/QuizEngine.hpp` |

---

## 🚀 How to Build and Run

### Option 1: One-Click Build Script (Recommended)
Double-click `build.bat` or run in terminal:
```cmd
cd C:\Users\himan\Desktop\PRO\QuizArena
build.bat
QuizArena.exe
```

### Option 2: Using MSVC Command Line (`cl.exe`)
```cmd
cl.exe /EHsc /std:c++17 /O2 /I include src\main.cpp /Fe:QuizArena.exe
QuizArena.exe
```

### Option 3: Using GCC / MinGW (`g++`)
```bash
g++ -std=c++17 -O2 -I include src/main.cpp -o QuizArena.exe
./QuizArena.exe
```

---

## 🖥️ UI & Terminal Features
- **ANSI 24-bit TrueColor / VT100 Escape Codes:** High-contrast color hierarchy (cyan, emerald green, gold, coral red, magenta).
- **Unicode Box-Drawing Characters:** Clean cards, progress meters, and status bars.
- **ASCII Visual Tree & Matrix Rendering:** Live terminal visualization of the BST node hierarchy and Graph Adjacency Matrix.
- **Robust Input Handling:** Safe input validation against non-numeric inputs or buffer overflows.

---

## 🎓 Viva / Defense Preparation Guide

When presenting this project to your professor or examiner:
1. **Why use a BST for the Leaderboard instead of a flat array?**
   - *Answer:* Inserting a score into a sorted array takes $O(n)$ time due to shifting elements. In a balanced BST, insertion takes $O(\log n)$ average time. When generating the leaderboard, a **Reverse In-Order Traversal** (Right $\rightarrow$ Root $\rightarrow$ Left) extracts players in descending score order in $O(n)$ time.
2. **How does the Circular Queue prevent the "false overflow" of a linear queue?**
   - *Answer:* In a simple linear queue, when elements are dequeued, the space before `front` is wasted even if `rear` reaches the end. The Circular Queue wraps around using modulo arithmetic (`(rear + 1) % MAX`), allowing infinite turn cycles without moving memory.
3. **Why use a Directed Acyclic Graph (DAG) for the Skill Tree?**
   - *Answer:* Learning topics have strict prerequisites. A DAG prevents circular dependencies. We use **BFS** to find all downstream topics unlocked by a concept, and **Topological Sort** (Kahn's In-Degree algorithm) to compute the optimal curriculum path.
4. **How does the Infix to Postfix converter work?**
   - *Answer:* It uses a character Stack to manage operator precedence (`^` > `* / %` > `+ -`). Operands are output immediately, while operators wait on the stack until an operator of lower or equal precedence arrives, guaranteeing correct mathematical evaluation order without parentheses.
