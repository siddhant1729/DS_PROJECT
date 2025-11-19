📌 Task Manager — Data Structures Project

A clean, efficient, and fully-featured Task Management System implemented in C++ using core Data Structures such as:

Linked List

Hash Map

Stack

Priority Queue (Min-Heap)

Custom Date Structure

This project demonstrates DSA concepts in real-world application design, with a clean terminal UI and multiple functionalities such as add, edit, delete, search, undo, and statistics.

🌟 Features
✅ Task Operations

Add new tasks

Edit existing tasks

Delete tasks

Mark tasks as Completed / Pending

Search tasks by ID

Search tasks by Title keyword

Sort tasks by Priority or Deadline

🔁 Undo System

Undo last delete

Undo last update/edit

Implemented using Stack

⚙️ Data Structures Used
Feature	Data Structure
Store all tasks	Singly Linked List
Fast Search	Custom Hash Map
Undo operations	Stack
Sorted by deadline/priority	Min Heap (Priority Queue)
Date validation & comparison	Custom Date class
📂 Project Structure
.
├── main.cpp
├── README.md  ← (this file)
└── assets/    ← (screenshots here)

📸 Screenshots

(Add screenshots of your terminal app here)

Example:


🧠 Concepts Covered
✔ Linked List

Used to maintain the entire task list dynamically.

✔ Hash Map

Provides O(1) search for tasks using task ID.

✔ Stack

Stores deleted/edited tasks for Undo operations.

✔ Priority Queue

Sorts tasks by urgency (deadline + priority).

✔ Custom Date Class

Handles:

Validation

Comparison

Leap year adjustment

🚀 How to Run
1. Clone the repository
git clone https://github.com/your-username/DS_PROJECT.git
cd DS_PROJECT

2. Compile

Using g++:

g++ main.cpp -o task_manager

3. Run
./task_manager


Windows:

task_manager.exe

🎮 Usage Guide

When you run the program, you’ll see:

========================================
              TASK MANAGER
========================================
1. Add New Task
2. Display All Tasks
3. Display by Priority/Deadline
4. Search Task
5. Edit Task
6. Delete Task
7. Change Status
8. Undo Last Operation
9. Statistics
10. Exit
========================================


Follow the menu to interact with your task list.

🛠️ Technologies Used

C++ (Core logic)

Standard Template Library concepts

Object-Oriented Programming

Data Structures + Algorithms

🧪 Future Enhancements

Persistent file storage (Save/Load tasks)

GUI using Qt or SFML

Export data to CSV/JSON

Add colored terminal UI

⭐ If You Like This Project

Don’t forget to ⭐ star the repo on GitHub — it motivates me to improve it further!

👨‍💻 Author

Siddhant
📧 your-email@example.com

🌐 GitHub: https://github.com/siddhant1729