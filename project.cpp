#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <limits>

using namespace std;

// ==================== UTILITY FUNCTIONS ====================

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause()
{
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// ==================== DATE STRUCTURE ====================

struct Date
{
    int day, month, year;

    Date() : day(1), month(1), year(2024) {}
    Date(int d, int m, int y) : day(d), month(m), year(y) {}

    string toString() const
    {
        stringstream ss;
        ss << setfill('0') << setw(2) << day << "/"
           << setw(2) << month << "/" << year;
        return ss.str();
    }

    bool isValid() const
    {
        if (year < 2024 || year > 2100)
            return false;
        if (month < 1 || month > 12)
            return false;
        if (day < 1 || day > 31)
            return false;

        int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))
            daysInMonth[1] = 29;

        return day <= daysInMonth[month - 1];
    }

    int toInt() const { return year * 10000 + month * 100 + day; }

    bool operator<(const Date &other) const { return toInt() < other.toInt(); }
    bool operator>(const Date &other) const { return toInt() > other.toInt(); }
    bool operator==(const Date &other) const { return toInt() == other.toInt(); }
};

// ==================== TASK STRUCTURE ====================

struct Task
{
    int id;
    string title;
    string description;
    string category;
    int priority;
    Date deadline;
    string status;

    Task() : id(0), priority(2), status("Pending") {}

    Task(int i, string t, string d, string c, int p, Date dl)
        : id(i), title(t), description(d), category(c),
          priority(p), deadline(dl), status("Pending") {}

    string getPriorityString() const
    {
        switch (priority)
        {
        case 1:
            return "High";
        case 2:
            return "Medium";
        case 3:
            return "Low";
        }
        return "Unknown";
    }

    void display() const
    {
        cout << "\n----------------------------------------\n";
        cout << "ID: " << id << "\n";
        cout << "Title: " << title << "\n";
        cout << "Description: " << description << "\n";
        cout << "Category: " << category << "\n";
        cout << "Priority: " << getPriorityString() << "\n";
        cout << "Deadline: " << deadline.toString() << "\n";
        cout << "Status: " << status << "\n";
        cout << "----------------------------------------\n";
    }
};

// ==================== LINKED LIST ====================

struct Node
{
    Task data;
    Node *next;
    Node(Task t) : data(t), next(nullptr) {}
};

class LinkedList
{
private:
    Node *head;
    int size;

public:
    LinkedList() : head(nullptr), size(0) {}

    void insert(Task task)
    {
        Node *newNode = new Node(task);
        if (!head)
            head = newNode;
        else
        {
            Node *temp = head;
            while (temp->next)
                temp = temp->next;
            temp->next = newNode;
        }
        size++;
    }

    bool remove(int id)
    {
        if (!head)
            return false;

        if (head->data.id == id)
        {
            Node *temp = head;
            head = head->next;
            delete temp;
            size--;
            return true;
        }

        Node *temp = head;
        while (temp->next && temp->next->data.id != id)
            temp = temp->next;

        if (temp->next)
        {
            Node *toDelete = temp->next;
            temp->next = toDelete->next;
            delete toDelete;
            size--;
            return true;
        }

        return false;
    }

    Task *search(int id)
    {
        Node *temp = head;
        while (temp)
        {
            if (temp->data.id == id)
                return &(temp->data);
            temp = temp->next;
        }
        return nullptr;
    }

    void display() const
    {
        if (!head)
        {
            cout << "\nNo tasks to display.\n";
            return;
        }
        Node *temp = head;
        while (temp)
        {
            temp->data.display();
            temp = temp->next;
        }
    }

    int getSize() const { return size; }
    Node *getHead() const { return head; }
};

// ==================== STACK (Undo) ====================

struct StackNode
{
    Task data;
    string operation;
    StackNode *next;
    StackNode(Task t, string op) : data(t), operation(op), next(nullptr) {}
};

class Stack
{
private:
    StackNode *top;
    int size;

public:
    Stack() : top(nullptr), size(0) {}

    void push(Task task, string operation)
    {
        StackNode *newNode = new StackNode(task, operation);
        newNode->next = top;
        top = newNode;
        size++;
    }

    bool pop(Task &task, string &op)
    {
        if (!top)
            return false;
        task = top->data;
        op = top->operation;

        StackNode *temp = top;
        top = top->next;
        delete temp;
        size--;

        return true;
    }

    bool isEmpty() const { return top == nullptr; }
};

// ==================== MIN HEAP (Priority Queue) ====================

class PriorityQueue
{
private:
    Task *heap;
    int size;

    int left(int i) { return 2 * i + 1; }
    int right(int i) { return 2 * i + 2; }
    int parent(int i) { return (i - 1) / 2; }

    void heapifyUp(int i)
    {
        while (i > 0 && heap[i].deadline < heap[parent(i)].deadline)
        {
            swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }

    void heapifyDown(int i)
    {
        int smallest = i;
        int l = left(i);
        int r = right(i);

        if (l < size && heap[l].deadline < heap[smallest].deadline)
            smallest = l;
        if (r < size && heap[r].deadline < heap[smallest].deadline)
            smallest = r;

        if (smallest != i)
        {
            swap(heap[i], heap[smallest]);
            heapifyDown(smallest);
        }
    }

public:
    PriorityQueue(int cap)
    {
        heap = new Task[cap];
        size = 0;
    }

    void insert(Task t)
    {
        heap[size] = t;
        heapifyUp(size);
        size++;
    }

    bool extractMin(Task &t)
    {
        if (size == 0)
            return false;
        t = heap[0];
        heap[0] = heap[size - 1];
        size--;
        heapifyDown(0);
        return true;
    }
};

// ==================== HASH MAP ====================

struct HashNode
{
    int key;
    Task *value;
    HashNode *next;
    HashNode(int k, Task *v) : key(k), value(v), next(nullptr) {}
};

class HashMap
{
private:
    static const int SIZE = 50;
    HashNode *table[SIZE];

    int hash(int k) { return k % SIZE; }

public:
    HashMap()
    {
        for (int i = 0; i < SIZE; i++)
            table[i] = nullptr;
    }

    void insert(int key, Task *value)
    {
        int idx = hash(key);
        HashNode *node = new HashNode(key, value);

        node->next = table[idx];
        table[idx] = node;
    }

    Task *search(int key)
    {
        int idx = hash(key);
        HashNode *temp = table[idx];

        while (temp)
        {
            if (temp->key == key)
                return temp->value;
            temp = temp->next;
        }
        return nullptr;
    }

    void remove(int key)
    {
        int idx = hash(key);
        HashNode *temp = table[idx];
        HashNode *prev = nullptr;

        while (temp)
        {
            if (temp->key == key)
            {
                if (prev)
                    prev->next = temp->next;
                else
                    table[idx] = temp->next;
                delete temp;
                return;
            }
            prev = temp;
            temp = temp->next;
        }
    }

    void clear()
    {
        for (int i = 0; i < SIZE; i++)
        {
            HashNode *temp = table[i];
            while (temp)
            {
                HashNode *next = temp->next;
                delete temp;
                temp = next;
            }
            table[i] = nullptr;
        }
    }
};

// ==================== TASK MANAGER ====================

class TaskManager
{
private:
    LinkedList tasks;
    HashMap map;
    Stack undo;
    int nextId = 1;

    void rebuildMap()
    {
        map.clear();
        Node *temp = tasks.getHead();
        while (temp)
        {
            map.insert(temp->data.id, &(temp->data));
            temp = temp->next;
        }
    }

public:
    void addTask()
    {
        clearScreen();

        Task t;
        t.id = nextId++;

        cin.ignore();
        cout << "Enter Title: ";
        getline(cin, t.title);

        cout << "Enter Description: ";
        getline(cin, t.description);

        cout << "Enter Category: ";
        getline(cin, t.category);

        cout << "Enter Priority (1-High, 2-Medium, 3-Low): ";
        cin >> t.priority;

        int d, m, y;
        do
        {
            cout << "Enter Deadline (DD MM YYYY): ";
            cin >> d >> m >> y;
            t.deadline = Date(d, m, y);
        } while (!t.deadline.isValid());

        tasks.insert(t);
        rebuildMap();

        cout << "\nTask Added Successfully.";
        pause();
    }

    void displayAll()
    {
        clearScreen();
        tasks.display();
        pause();
    }

    void displayByPriority()
    {
        clearScreen();

        if (tasks.getSize() == 0)
        {
            cout << "\nNo tasks available.\n";
            pause();
            return;
        }

        PriorityQueue pq(tasks.getSize());
        Node *temp = tasks.getHead();

        while (temp)
        {
            pq.insert(temp->data);
            temp = temp->next;
        }

        Task t;
        while (pq.extractMin(t))
            t.display();

        pause();
    }

    void searchTask()
    {
        clearScreen();

        int id;
        cout << "Enter Task ID: ";
        cin >> id;

        Task *t = map.search(id);

        if (t)
            t->display();
        else
            cout << "\nTask Not Found.\n";

        pause();
    }

    void editTask()
    {
        clearScreen();
        int id;
        cout << "Enter Task ID to Edit: ";
        cin >> id;

        Task *t = map.search(id);
        if (!t)
        {
            cout << "\nTask Not Found.\n";
            pause();
            return;
        }

        Task old = *t;

        cin.ignore();
        cout << "New Title: ";
        getline(cin, t->title);

        cout << "New Description: ";
        getline(cin, t->description);

        cout << "New Category: ";
        getline(cin, t->category);

        cout << "New Priority (1-3): ";
        cin >> t->priority;

        int d, m, y;
        do
        {
            cout << "New Deadline (DD MM YYYY): ";
            cin >> d >> m >> y;
            t->deadline = Date(d, m, y);
        } while (!t->deadline.isValid());

        undo.push(old, "UPDATE");

        cout << "\nTask Updated.";
        pause();
    }

    void deleteTask()
    {
        clearScreen();
        int id;
        cout << "Enter Task ID to Delete: ";
        cin >> id;

        Task *t = map.search(id);
        if (!t)
        {
            cout << "\nTask Not Found.\n";
            pause();
            return;
        }

        Task backup = *t;

        tasks.remove(id);
        map.remove(id);

        undo.push(backup, "DELETE");

        cout << "\nTask Deleted.";
        pause();
    }

    void changeStatus()
    {
        clearScreen();

        int id;
        cout << "Enter Task ID: ";
        cin >> id;

        Task *t = map.search(id);
        if (!t)
        {
            cout << "\nTask Not Found.\n";
            pause();
            return;
        }

        Task old = *t;

        cout << "1. Pending\n2. Completed\nEnter New Status: ";
        int s;
        cin >> s;

        t->status = (s == 2 ? "Completed" : "Pending");
        undo.push(old, "UPDATE");

        cout << "\nStatus Updated.";
        pause();
    }

    void undoLast()
    {
        clearScreen();

        if (undo.isEmpty())
        {
            cout << "\nNothing to Undo.";
            pause();
            return;
        }

        Task t;
        string op;

        undo.pop(t, op);

        if (op == "DELETE")
        {
            tasks.insert(t);
            rebuildMap();
            cout << "\nRestored Deleted Task.";
        }
        else if (op == "UPDATE")
        {
            Task *cur = map.search(t.id);
            if (cur)
                *cur = t;
            cout << "\nRestored Previous State.";
        }

        pause();
    }

    void statistics()
    {
        clearScreen();
        int total = 0, pending = 0, completed = 0;

        Node *temp = tasks.getHead();
        while (temp)
        {
            total++;
            if (temp->data.status == "Completed")
                completed++;
            else
                pending++;
            temp = temp->next;
        }

        cout << "\nTotal Tasks: " << total;
        cout << "\nPending: " << pending;
        cout << "\nCompleted: " << completed;

        pause();
    }
};

// ==================== MENU ====================

void displayMenu()
{
    cout << "\n========================================\n";
    cout << "              TASK MANAGER              \n";
    cout << "========================================\n";
    cout << "1. Add New Task\n";
    cout << "2. Display All Tasks\n";
    cout << "3. Display by Priority/Deadline\n";
    cout << "4. Search Task\n";
    cout << "5. Edit Task\n";
    cout << "6. Delete Task\n";
    cout << "7. Change Status\n";
    cout << "8. Undo Last Operation\n";
    cout << "9. Statistics\n";
    cout << "10. Exit\n";
    cout << "========================================\n";
    cout << "Enter your choice: ";
}

// ==================== MAIN ====================

int main()
{
#ifdef _WIN32
    system("chcp 65001 > nul"); // UTF-8 Fix on Windows
#endif

    TaskManager manager;
    int choice;

    while (true)
    {
        clearScreen();
        displayMenu();
        cin >> choice;

        clearScreen();

        switch (choice)
        {
        case 1:
            manager.addTask();
            break;
        case 2:
            manager.displayAll();
            break;
        case 3:
            manager.displayByPriority();
            break;
        case 4:
            manager.searchTask();
            break;
        case 5:
            manager.editTask();
            break;
        case 6:
            manager.deleteTask();
            break;
        case 7:
            manager.changeStatus();
            break;
        case 8:
            manager.undoLast();
            break;
        case 9:
            manager.statistics();
            break;
        case 10:
            return 0;
        }
    }
}
