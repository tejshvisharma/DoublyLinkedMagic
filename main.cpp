#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <unistd.h> // For sleep()
#include <vector>   // For undo/redo functionality
using namespace std;

// Color codes for better user interaction
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"
#define MAGENTA "\033[35m"
#define BOLD "\033[1m"

class Node {
public:
    int data;
    Node* prev;
    Node* next;
    Node(int val) {
        data = val;
        prev = NULL;
        next = NULL;
    }
};

class LinkedList {
    Node* head;
    Node* tail;
    vector<string> actions; // For undo/redo functionality
    int nodeCount;          // To keep track of the number of nodes

public:
    LinkedList() {
        head = NULL;
        tail = NULL;
        nodeCount = 0;
    }

    void insertAtHead(int val) {
        if (val < 0) {
            cout << RED << "\n⚠️  Negative values are not allowed!\n" << RESET;
            return;
        }

        Node* newNode = new Node(val);
        cout << BLUE << "\n🔵 Operation: Insert at Head" << RESET << endl;
        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        nodeCount++;
        actions.push_back("Inserted " + to_string(val) + " at Head");
        displayListWithPointers();
        printAlgorithm("Insert at Head", val);
        waitForUser();
    }

    void insertAtTail(int val) {
        if (val < 0) {
            cout << RED << "\n⚠️  Negative values are not allowed!\n" << RESET;
            return;
        }

        Node* newNode = new Node(val);
        cout << BLUE << "\n🔵 Operation: Insert at Tail" << RESET << endl;
        if (tail == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        nodeCount++;
        actions.push_back("Inserted " + to_string(val) + " at Tail");
        displayListWithPointers();
        printAlgorithm("Insert at Tail", val);
        waitForUser();
    }

    void insertAtPosition(int pos, int val) {
        if (val < 0) {
            cout << RED << "\n⚠️  Negative values are not allowed!\n" << RESET;
            return;
        }

        cout << BLUE << "\n🔵 Operation: Insert at Position " << pos << RESET << endl;
        if (pos < 1 || pos > nodeCount + 1) {
            cout << RED << "\n⚠️  Position out of bounds!\n" << RESET;
            return;
        }

        Node* newNode = new Node(val);
        if (pos == 1) {
            insertAtHead(val);
            return;
        }

        Node* temp = head;
        for (int i = 1; i < pos - 1 && temp != NULL; i++) {
            temp = temp->next;
        }

        newNode->next = temp->next;
        newNode->prev = temp;

        if (temp->next != NULL) {
            temp->next->prev = newNode;
        } else {
            tail = newNode;
        }

        temp->next = newNode;
        nodeCount++;
        actions.push_back("Inserted " + to_string(val) + " at Position " + to_string(pos));
        displayListWithPointers();
        printAlgorithm("Insert at Position", val, pos);
        waitForUser();
    }

    void deleteByValue(int val) {
        cout << BLUE << "\n🔵 Operation: Delete by Value" << RESET << endl;
        if (head == NULL) {
            cout << RED << "\n  List is empty!\n" << RESET;
            return;
        }

        Node* temp = head;
        if (head->data == val) {
            head = head->next;
            if (head != NULL) head->prev = NULL;
            else tail = NULL;
            delete temp;
            nodeCount--;
            actions.push_back("Deleted " + to_string(val) + " by Value");
            displayListWithPointers();
            printAlgorithm("Delete by Value", val);
            waitForUser();
            return;
        }

        while (temp != NULL && temp->data != val) {
            temp = temp->next;
        }

        if (temp == NULL) {
            cout << RED << "\n  Value not found in the list!\n" << RESET;
            return;
        }

        if (temp == tail) {
            tail = tail->prev;
            tail->next = NULL;
            delete temp;
            nodeCount--;
            actions.push_back("Deleted " + to_string(val) + " by Value");
            displayListWithPointers();
            printAlgorithm("Delete by Value", val);
            waitForUser();
            return;
        }

        temp->prev->next = temp->next;
        temp->next->prev = temp->prev;
        delete temp;
        nodeCount--;
        actions.push_back("Deleted " + to_string(val) + " by Value");
        displayListWithPointers();
        printAlgorithm("Delete by Value", val);
        waitForUser();
    }

    bool search(int val) {
        cout << BLUE << "\n Operation: Search for Value " << val << RESET << endl;
        Node* temp = head;
        while (temp != NULL) {
            if (temp->data == val) {
                cout << GREEN << "\n Value " << val << " found in the list!\n" << RESET;
                displayListWithPointers();
                printAlgorithm("Search for Value", val);
                waitForUser();
                return true;
            }
            temp = temp->next;
        }
        cout << RED << "\n  Value " << val << " not found in the list!\n" << RESET;
        displayListWithPointers();
        printAlgorithm("Search for Value", val);
        waitForUser();
        return false;
    }

    void displayListWithPointers() {
        if (head == NULL) {
            cout << RED << "\n  List is empty!\n" << RESET;
            return;
        }

        Node* temp = head;
        cout << CYAN << "\n Current List with Pointers:\n" << RESET;
        while (temp != NULL) {
            if (temp->prev) cout <<YELLOW<< "[" << temp->prev->data << "] <- ";
            else cout <<YELLOW<< "[NULL] <- ";

            cout <<YELLOW<< "[" << temp->data << "]";

            if (temp->next) cout <<YELLOW<< " -> [" << temp->next->data << "]";
            else cout <<YELLOW<< " -> [NULL]";

            cout << "\n"<<RESET;
            temp = temp->next;
        }
        cout << "Total Nodes: " << nodeCount << "\n";
    }

    void printAlgorithm(string operation, int val = 0, int pos = 0) {
        cout << MAGENTA << "\n====================================================" << endl;
        cout << "                Algorithm for " << operation << endl;
        cout << "====================================================" << RESET << endl;

        if (operation == "Insert at Head") {
            cout << "1. Create a new node with value " << val << ".\n";
            cout << "2. If the list is empty, set head and tail to the new node.\n";
            cout << "3. If the list is not empty, link the new node's next to the current head.\n";
            cout << "4. Update the current head's previous pointer to the new node.\n";
            cout << "5. Set the new node as the new head.\n";
        } else if (operation == "Insert at Tail") {
            cout << "1. Create a new node with value " << val << ".\n";
            cout << "2. If the list is empty, set head and tail to the new node.\n";
            cout << "3. Link the current tail's next pointer to the new node.\n";
            cout << "4. Update the new node's previous pointer to the current tail.\n";
            cout << "5. Set the new node as the new tail.\n";
        } else if (operation == "Insert at Position") {
            cout << "1. Create a new node with value " << val << ".\n";
            cout << "2. Traverse to the node at position " << pos - 1 << ".\n";
            cout << "3. Link the new node's next pointer to the current node's next pointer.\n";
            cout << "4. Update the next node's previous pointer to the new node.\n";
            cout << "5. Link the current node's next pointer to the new node.\n";
            cout << "6. Update the new node's previous pointer to the current node.\n";
        } else if (operation == "Delete by Value") {
            cout << "1. Start from the head and traverse the list.\n";
            cout << "2. If the node to delete is found, adjust the pointers of surrounding nodes.\n";
            cout << "3. If it's the head, move head to the next node.\n";
            cout << "4. If it's the tail, move tail to the previous node.\n";
            cout << "5. Delete the node and free memory.\n";
        } else if (operation == "Search for Value") {
            cout << "1. Start from the head and traverse the list.\n";
            cout << "2. Compare each node's value with the search value.\n";
            cout << "3. If found, return true, else return false.\n";
        }
        cout << MAGENTA << "====================================================\n" << RESET;
    }

    void waitForUser() {
        cout << "Press enter to continue...";
        cin.ignore();
        cin.get();
    }

    void showMenu() {
        cout <<BOLD<<MAGENTA<< "\n*__________________________________________________________________*";
        cout << "\n|---+---+---+---+---+---+---+--+---+---+---+---+---+---+---+-------|";
        cout << "\n|---+---+---+---+ LINKED LIST OPERATIONS MENU +---+---+---+--------|";
        cout << "\n|---+---+---+---+---+---+---+--+---+---+---+---+---+---+---+-------|";
        cout << "\n|__________________________________________________________________|\n\n"<<YELLOW;
        cout << "*-----------------------------------------------------------------*\n";
        cout << "|1. INSERT AT HEAD   | 2. INSERT AT TAIL  | 3. INSERT AT POSITION |\n";
        cout << "*-----------------------------------------------------------------*\n"<<BLUE;
        cout << "*-----------------------------------------------------------------*\n";
        cout << "|4. DELETE BY VALUE  | 5. SEARCH          | 6. DISPLAY LIST       |\n";
        cout << "*-----------------------------------------------------------------*\n"<<RED;
        
        cout << "*---------------------*\n";
        cout << "|7. EXIT              |\n";
        cout << "*---------------------*\n\n"<<CYAN;
        cout << "ENTER YOUR CHOICE : ";
    }

    void undoLastAction() {
        if (actions.empty()) {
            cout << RED << "\n  No actions to undo!\n" << RESET;
            return;
        }

        // Simple undo functionality; can be enhanced further
        actions.pop_back();
        cout << GREEN << "\n Last action undone!\n" << RESET;
    }

    int getNodeCount() {
        return nodeCount;
    }
};

int main() {
    LinkedList list;
    int choice, value, position;

    while (true) {
        list.showMenu();
        
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter value to insert at head: ";
                cin >> value;
                list.insertAtHead(value);
                break;
            case 2:
                cout << "Enter value to insert at tail: ";
                cin >> value;
                list.insertAtTail(value);
                break;
            case 3:
                cout <<CYAN "Enter position and value to insert: "<<RESET;
                cin >> position >> value;
                list.insertAtPosition(position, value);
                break;
            case 4:
                cout <<RED<< "Enter value to delete: "<<RESET;
                cin >> value;
                list.deleteByValue(value);
                break;
            case 5:
                cout <<MAGENTA "Enter value to search: ";
                cin >> value;
                list.search(value);
                break;
            case 6:
                list.displayListWithPointers();
                break;
            case 7:
                list.undoLastAction();
                break;
            case 8:
                cout << GREEN << "\n🔚 Exiting the program. Goodbye!\n" << RESET;
                return 0;
            default:
                cout << RED << "\n⚠️  Invalid option! Please try again.\n" << RESET;
        }
    }

    return 0;
}



