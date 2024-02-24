//
// Created by Julian To on 2/19/24.
//

#include <iostream>
class Process {
public:

    std::string name;
    int totalTime;

    Process(std::string name, int totalTime) {
        this->name = name;
        this->totalTime = totalTime;
    }

    void updateRunTime() {
        totalTime--;
    }

    void print() {
        using namespace std;
        cout << "Process " << name << ": " << totalTime <<
        " seconds" << endl;
    }

};

template <typename T> class Node {
public:
    T *data;
    Node<T> *next;
    Node<T> *prev;

    Node(T *data) {
        this->data = data;
        this->next = nullptr;
        this->prev = nullptr;


    }

    void print() {
        data->print();
    }
};

template <typename T> class CircularDLL { //circular doubly linked list
private:
    Node<T> *head;
    int length;
public:
    CircularDLL(T *data) {
        head = nullptr;
        length = 0;
        insert( data );


    }

    CircularDLL() {
        head = nullptr;
        length = 0;
    }

    ~CircularDLL() {
        Node<T> *next = head;
//        do {
//            next = curr->next;
//            remove( curr );
//            curr = next;
//
//        } while ( length > 0 );
        for ( int i = 0; i < length; i++ ) {
            Node<T> *curr = next;
            next = curr->next;
            remove( curr );
        }



    }
    
    Node<T> *getHead() {
        return head;
    }

    int getLength() {
        return length;
    }

    void print() {
        using namespace std;

        Node<T> *curr = head;
        int index = 1;

        if ( length == 0 ) { cout << "EMPTY LIST!!!"; }

        do {
            cout << "\t" << index << ".\t";
            curr->print();
            curr = curr->next;
            index++;
        } while( curr != head );

    }

    void insert(T *data) {
        Node<T> *newNode = new Node<T>( data );
        if ( head == nullptr ) {
            head = newNode;
            newNode->next = head;
            newNode->prev = head;
            length++;
            return;
        }

        Node<T> *curr = head;
        while ( curr->next != head ) { //while the next node is not equal to the head
            curr = curr->next;
        }
        //set the newNode's values
        newNode->prev = curr;
        newNode->next = head;

        //inserts newNode into circular doubly linked list.
        curr->next = newNode;
        head->prev = newNode;
        length += 1;


    }

    void remove( Node<T> *node ) {

        if (length == 0 ) return;
        if ( length == 1 ) head = nullptr;

        Node<T> *nodeBefore = node->prev;
        Node<T> *nodeAfter = node->next;

        nodeBefore->next = nodeAfter;
        nodeAfter->prev = nodeBefore;

        if ( node == head ) head = nodeAfter;
        free(node);

        length--;

    }


    bool isEmpty() {
        if ( length == 0 ) return true;
        return false;
    }

};

class RoundRobinScheduler {
private:
    int quantumTime;
    int cyclesElapsed;
    int timeElapsed;
    CircularDLL<Process> list;

public:
    RoundRobinScheduler() {
        this->quantumTime = 0;
        this->cyclesElapsed = 0;
        this->timeElapsed = 0;
        this->list = CircularDLL<Process>();
    }

    void run() {
        using namespace std;
        cout << "Enter Quantum Time: " ;
        cin >> quantumTime;

        cout << "\nPrepopulating processes...\n" << endl;
        list.insert( new Process("A", 10) );
        list.insert( new Process("B", 12) );
        list.insert( new Process("B", 12) );
        list.insert( new Process("C", 8) );
        list.insert( new Process("D", 5) );
        list.insert( new Process("E", 10) );

        list.print();

        while ( !list.isEmpty() ) {

            while ( newProcessPrompted() ) {
                insertFromUser();
            }

            runCycle();

            if ( list.isEmpty() ) break;

            list.print();
        }

        cout << "Processes ended" << endl;


    }

    void stepProcesses() {

        Node<Process> *curr = list.getHead();

        do {
            curr->data->updateRunTime();
            curr = curr->next;
        } while ( curr != list.getHead() );


    }

    void removeElapsedProcesses() {
        Node<Process> *next = list.getHead();
        int reps = list.getLength();

        for ( int i = 0; i < reps; i++ ) {
            Node<Process> *curr = next;
            next = curr->next; // if removes head, curr->next becomes the head
            if ( curr->data->totalTime <= 0 ) list.remove( curr );
        }
    }

    bool newProcessPrompted() {
        using namespace std;
        string confirmation;
        cout << "\nAdd new process? (Enter Y/N): ";
        cin >> confirmation;

        if ( confirmation == "N" ) return false;
        return true;
    }

    void insertFromUser() {
        using namespace std;

        string processName;
        int processTime;
        cout << "\nEnter new process name: ";
        cin >> processName;

        cout << "Enter total process time: ";
        cin >> processTime;

        list.insert( new Process( processName, processTime ) );

        cout << "\nProcess added." << endl;


    }

    void runCycle() {
        using namespace std;

        cyclesElapsed++;
        cout << "\nRunning cycle " << cyclesElapsed << ".\n" << endl;
        timeElapsed += quantumTime;
        for (int i = 0; i < quantumTime; i++ ) {
            stepProcesses();
        };

        removeElapsedProcesses();

        cout << "After cycle " << cyclesElapsed << " - " << timeElapsed
            << " seconds elapses - state of processes is as follows " << endl;

    }

};

int main() {
    RoundRobinScheduler rrs = RoundRobinScheduler();
    rrs.run();

    return 0;
}