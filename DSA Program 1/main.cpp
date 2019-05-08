/*
Kevin Kerliu
11/20/2018

Data Structures and Algorithms Assignment #1: Stacks and Queues

Program Description
This program implements stacks and queues via singly linked lists.
It interprets commands from an input file and writes the corresponding output to an output file.
*/

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <iterator>
#include <typeinfo>

std::ifstream inputFile;
std::ofstream outputFile;

void openInputandOutputStream(std::ifstream &input, std::ofstream &output);
void parseLineDetermineType(const std::string &commandLine);

int main(){
    openInputandOutputStream(inputFile, outputFile);
    std::string commandLine;
    //The default delimiter for the getline function is the new line character, \n
    while(getline(inputFile, commandLine)){
        parseLineDetermineType(commandLine);
    }
    inputFile.close();
    outputFile.close();
    return 0;
}





/*
Template class declaration for SimpleList, a singly linked list.
SimpleList is an abstract base class.
It serves as the base class for the stack class and queue class.
*/
template <typename T>
class SimpleList{
private:
    class Node{
    public:
        Node(T val, Node* pointer){
            data = val;
            next = pointer;
        }
        Node *next;
        T data;
    };
    Node *head;
    Node *tail;
    std::string listName;
protected:
    void insertAtHead(T val);
    void insertAtTail(T val);
    T removeFromHead();
public:
    explicit SimpleList(std::string name);
    virtual void push(T val) = 0;
    virtual T pop() = 0;
    std::string getListName();
    bool isEmpty() const;
};

//SimpleList constructor.
template <typename T>
SimpleList<T>::SimpleList(std::string name){
    listName = name;
    head = nullptr;
    tail = nullptr;
}

//Checks if the SimpleList is empty.
template <typename T>
bool SimpleList<T>::isEmpty() const{
    return (head == nullptr && tail == nullptr);
}

//Returns the name of the SimpleList.
template <typename T>
std::string SimpleList<T>::getListName(){
    return listName;
}

//Inserts a node at the front of the SimpleList. This is used for stacks.
template <typename T>
void SimpleList<T>::insertAtHead(T val){
    auto *newTempNode = new Node(val, head);
    head = newTempNode;
}

//Inserts a node at the end of the SimpleList. This is used for queues.
template <typename T>
void SimpleList<T>::insertAtTail(T val){
    auto *newTempNode = new Node(val, nullptr);
    ((tail == nullptr) ? head : tail->next) = newTempNode;
    tail = newTempNode;
}

//Removes a node from the end of the SimpleList. This is used for both stacks and queues.
template <typename T>
T SimpleList<T>::removeFromHead(){
    Node *pointer = head;
    T val= pointer->data;
    if (pointer-> next == nullptr){
        tail = nullptr;
    }
    head = pointer->next;
    delete pointer;
    return val;
}





//Class declaration for Stack
template <typename T>
class Stack: public SimpleList<T>
{
public:
    void push(T val) override;
    T pop() override;
    explicit Stack(std::string stackName);
};

//Push onto the stack. This means inserting at the start of the SimpleList.
template <typename T>
void Stack<T>::push(T val){
    SimpleList<T>::insertAtHead(val);
}

//Pop from the stack. This means removing from the start of the SimpleList.
template <typename T>
T Stack<T>::pop(){
    return SimpleList<T>::removeFromHead();
}

//Stack constructor.
template <typename T>
Stack<T>::Stack(std::string stackName):SimpleList<T>(stackName){
}





// Class declaration for Queue
template <typename T>
class Queue: public SimpleList<T>
{
public:
    void push(T val) override;
    T pop() override;
    explicit Queue(std::string queueName);
};

//Push onto the queue. This means inserting at the end of the SimpleList.
template <typename T>
void Queue<T>::push(T val){
    SimpleList<T>::insertAtTail(val);
}

//Pop from the queue. This means removing from the front of the SimpleList.
template <typename T>
T Queue<T>::pop(){
    return SimpleList<T>::removeFromHead();
}

//Queue constructor.
template <typename T>
Queue<T>::Queue(std::string queueName):SimpleList<T>(queueName){
}





//Prompts the user for an input file and opens the file so it is ready to read from.
//Prompts the user for an output file and opens the file so it is ready to write to.
void openInputandOutputStream(std::ifstream &input, std::ofstream &output){
    std::string inputFileName;
    std::cout << "Enter name of input file: ";
    std::cin >> inputFileName;
    input.open(inputFileName.c_str());

    std::string outputFileName;
    std::cout << "Enter name of output file: ";
    std::cin >> outputFileName;
    output.open(outputFileName.c_str());
}

//Searches for the specified list. This helps determine whether or not a given list already exists.
template <typename T>
SimpleList<T>* SimpleListSearch(std::list<SimpleList<T> *> &searchList, std::string listName){
    for(typename std::list<SimpleList<T> *>::const_iterator it = searchList.begin(); it != searchList.end(); ++it){
        if((*it)->getListName() == listName){
            return *it;
        }
    }
    return nullptr;
}

//Processes commands from the input file once it is parsed.
template <typename T>
void processLine(std::list<SimpleList<T> *> &listSLType, std::string commands[], T val){
    /*
    If the command is to create a stack or queue, check if the stack or queue already exists.
    If the stack or queue already exists, then this command is not valid.
    On the other hand, if the command is valid, check if the command is to create a stack or to create a queue.
    If the command is to create a stack, then proceed accordingly (call the stack constructor).
    If the command is to create a queue, then proceed accordingly (call the queue constructor).
    */
    if(commands[0] == "create"){
        if (SimpleListSearch(listSLType, commands[1]) != nullptr){
            outputFile << "ERROR: This name already exists!" << std::endl;
        }
        else{
            if(commands[2] == "stack"){
                auto * pointerSL = new Stack<T>(commands[1]);
                listSLType.push_front(pointerSL);
            }
            else if(commands[2] == "queue"){
                auto * pointerSL = new Queue<T>(commands[1]);
                listSLType.push_front(pointerSL);
            }
        }
    }
    /*
    If the command is to push, check if the stack or queue already exists.
    If the stack or queue does not already exist, then this command is not valid.
    On the other hand, if the command is valid, then push it onto the stack or queue.
    */
    else if(commands[0] == "push"){
        SimpleList<T> *pointer = SimpleListSearch(listSLType, commands[1]);
        if(pointer == nullptr){
            outputFile << "ERROR: This name does not exist!" << std::endl;
        }
        else{
            pointer->push(val);
        }
    }
    /*
    If the command is to pop, check if the stack or queue already exists.
    If the stack or queue does not already exist, then this command is not valid.
    On the other hand, the stack or queue may exist but be empty.
    If the stack or queue is emtpy, then this command is not valid.
    Otherwise, if the command is valid, then pop from the stack or queue.
    */
    else if(commands[0] == "pop"){
        SimpleList<T> *pointer = SimpleListSearch(listSLType, commands[1]);
        if(pointer == nullptr){
            outputFile << "ERROR: This name does not exist!" << std::endl;
        }
        else if(pointer->isEmpty()){
            outputFile << "ERROR: This list is empty!" << std::endl;
        }
        else{
            outputFile << "Value popped: " << pointer->pop() << std::endl;
        }
    }
}

std::list<SimpleList<int> *> listSLi; //All stacks and queues of integers
std::list<SimpleList<double> *> listSLd; //All stacks and queues of doubles
std::list<SimpleList<std::string> *> listSLs; //All stacks and queues of strings

//Parses the input file to prepare it for processing.
//Together with the process function a check is performed on the read line and the appropriate functions are called.
void parseLineDetermineType(const std::string &commandLine){
    outputFile << "PROCESSING COMMAND: " << commandLine << std::endl;

    std::string swap;
    std::stringstream ss1(commandLine);
    auto * commands = new std::string[3];

    int i=0;
    while(ss1 >> swap)
        commands[i++] = swap;


    //Call processLine with the appropriate type SimpleList as a parameter.
    if(commands[1].substr(0,1) == "i"){
        int val = 0;
        if(!commands[2].empty()){
            val = atoi(commands[2].c_str());
        }
        processLine(listSLi, commands, val);
    }
    else if(commands[1].substr(0,1) == "d"){
        double val = 0.0;
        if(!commands[2].empty()){
            val = atof(commands[2].c_str());
        }
        processLine(listSLd, commands, val);
    }
    else if(commands[1].substr(0,1) == "s"){
        processLine(listSLs, commands, commands[2]);
    }
}
