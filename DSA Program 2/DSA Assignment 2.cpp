// THIS IS THE PROVIDED CODE FOR PROGRAM #2, DSA 1, FALL 2018

#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <cstring>
#include <cctype>
#include <cstdlib>

using namespace std;

// A simple class; each object holds four public fields
class Data {
public:
    unsigned int val1;
    unsigned int val2;
    char val3;
    string val4;
};

// Load the data from a specified input file
void loadDataList(list<Data *> &l, const string &filename) {

    ifstream input(filename.c_str());
    if (!input) {
        cerr << "Error: could not open " << filename << endl;
        exit(1);
    }

    // The first line indicates the size
    string line;
    getline(input, line);
    stringstream ss(line);
    int size;
    ss >> size;

    // Load the data
    for (int i = 0; i < size; i++) {
        getline(input, line);
        stringstream ss2(line);
        Data *pData = new Data();
        ss2 >> pData->val1 >> pData->val2 >> pData->val3 >> pData->val4;
        l.push_back(pData);
    }

    input.close();
}

// Output the data to a specified output file
void writeDataList(const list<Data *> &l, const string &filename) {

    ofstream output(filename.c_str());
    if (!output) {
        cerr << "Error: could not open " << filename << endl;
        exit(1);
    }

    // Write the size first
    int size = l.size();
    output << size << endl;

    // Write the data
    for (list<Data *>::const_iterator ipD = l.begin(); ipD != l.end(); ipD++) {
        output << (*ipD)->val1 << " "
               << (*ipD)->val2 << " "
               << (*ipD)->val3 << " "
               << (*ipD)->val4 << endl;
    }

    output.close();
}

// Sort the data according to a specified field
// (Implementation of this function will be later in this file)
void sortDataList(list<Data *> &, int field);

// The main function calls routines to get the data, sort the data,
// and output the data. The sort is timed according to CPU time.
int main() {
    string filename;
    cout << "Enter name of input file: ";
    cin >> filename;
    list<Data *> theList;
    loadDataList(theList, filename);

    cout << "Data loaded.  Executing sort..." << endl;

    int field = 0;
    cout << "Enter an integer from 1 - 4, representing field to sort: ";
    try {
        cin >> field;
        if (field < 1 || field > 4) {
            cerr << "Error: invalid field" << endl;
            exit(1);
        }
    }
    catch (...) {
        cerr << "Error: invalid field" << endl;
        exit(1);
    }

    clock_t t1 = clock();
    sortDataList(theList, field);
    clock_t t2 = clock();
    double timeDiff = ((double) (t2 - t1)) / CLOCKS_PER_SEC;

    cout << "Sort finished. CPU time was " << timeDiff << " seconds." << endl;

    cout << "Enter name of output file: ";
    cin >> filename;
    writeDataList(theList, filename);

    return 0;
}

// -------------------------------------------------
// YOU MAY NOT CHANGE OR ADD ANY CODE ABOVE HERE !!!
// -------------------------------------------------

int size;
Data *output[127][1000000];
Data *input[1100000];

void columnOneSort(list<Data *> &l);
void columnTwoSort(list<Data *> &l);
void columnThreeSort(list<Data *> &l);
void columnFourSort(list<Data *> &l);

void sortDataList(list<Data *> &l, int field){
    size = l.size();
    switch(field){
        case 1:
            columnOneSort(l);
            break;
        case 2:
            columnTwoSort(l);
            break;
        case 3:
            columnThreeSort(l);
            break;
        case 4:
            columnFourSort(l);
            break;
    }
}

void columnOneSort(list<Data *> &l){
    int j, k = 0;
    Data *key;
    list<Data *>::iterator it;
    it = l.begin();

    do{
        input[k] = *it;
        it++;
        k++;
    }
    while(it != l.end());

    for(int i = 1; i < size; i++){
        key = input[i];
        j = i - 1;
        while(j >= 0 && input[j]->val1 > key->val1){
            input[j+1] = input [j];
            j--;
        }
        input[j+1] = key;
    }

    it = l.begin();
    for(int i = 0; i < size; i++){
        *it = input[i];
        it++;
    }
}

void columnTwoSort(list<Data *> &l){
    int j;
    int k = 0;
    int index1 = 0;
    int count[127] = {0};
    Data *key;
    list<Data *>::iterator it;
    it = l.begin();

    do{
        input[k] = *it;
        it++;
        k++;
    }
    while(it != l.end());

    for(unsigned int e = 100000; e <= 1000000000 ; e *= 10){
        int count1[127] = {0};
        index1 = 0;
        for(int i = 0; i < size; i++){
            unsigned int p = ((input[i]->val2)/e)%10;
            output[p][count1[p]] = input[i];
            count1[p]++;
        }

        for(int i = 0; i < 127; i++){
            for(j = 0; j < count1[i]; j++){
                input[index1] = output[i][j];
                index1++;
            }
        }
    }

    for(int i = 1; i < size; i++){
        key = input[i];
        j = i-1;
        while(j >= 0 && input[j]->val2 > key->val2){
            input[j+1] = input[j];
            j--;
        }
        input[j+1] = key;
    }

    it = l.begin();
    for(int i = 0; i < size; i++){
        *it = input[i];
        it++;
    }
}


void columnThreeSort(list<Data *> &l){
    int count[127] = {0};
    list<Data *>::iterator it;
    it = l.begin();
    while(it != l.end()){
        int p = int((*it)->val3);
        output[p][count[p]] = *it;
        count[p]++, it++;
    }

    it = l.begin();
    for(int i = 0; i < 127; i++){
        for(int j = 0; j < count[i]; j++){
            *it = output[i][j];
            it++;
        }
    }
}

void columnFourSort(list<Data *> &l){
    int j;
    int k = 0;
    int count[127] = {0};
    Data *key;
    list<Data *>::iterator it;
    it = l.begin();
    do {
        input[k] = *it;
        it++, k++;
    }
    while(it != l.end());

    for(int positive = 2; positive >= 0; --positive){
        int count1[127] = {0};
        for(int i = 0; i < size; i++){
            int p = input[i]->val4[positive];
            output[p][count1[p]] = input[i];
            count1[p]++;
        }

        int index1 = 0;
        for(int i = 0; i < 127; i++){
            for(int i1 = 0; i1 < count1[i]; i1++){
                input[index1] = output[i][i1];
                index1++;
            }
        }
    }

    for(int i = 1; i < size; i++){
        key = input[i];
        j = i - 1;
        while(j >= 0 && input[j]->val4 > key->val4){
            input[j+1] = input[j];
            j--;
        }
        input[j+1] = key;
    }

    it = l.begin();
    for(int i = 0; i < size; i++){
        *it = input[i];
        it++;
    }
}