#include <iostream>
#include <cstdlib>  // for malloc
using namespace std;

// TEXT segment - code lives here (functions)
// DATA segment - global/static initialized variables
int globalVar = 100;           // initialized data (DATA)
static int staticVar = 200;    // static initialized (DATA)

// BSS segment (uninitialized data, part of DATA)
int uninitGlobal;              // uninitialized global (BSS)
int uninitGlobal2;             // second uninitialized global (BSS)

// Stack check: compare address from main (parent) vs child function
void checkStack(int* parentAddr) {
    int childVar = 99;
    cout << "--- STACK SEGMENT (Cross-function comparison) ---" << endl;
    cout << "main local addr (parent frame) : " << (void*)parentAddr << endl;
    cout << "param addr (child frame)       : " << (void*)&parentAddr << endl;
    cout << "child local addr (child frame) : " << (void*)&childVar << endl;
    cout << "Stack grows: "
         << (parentAddr > &childVar ? "DOWN (parent frame > child frame)" : "UP")
         << endl;
    cout << endl;
}

int main() {

    // STACK segment - local variable in main
    int mainVar = 10;

    // HEAP segment - dynamic allocation (malloc, larger sizes)
    // int* heapVar1 = new int(42);   // new may not allocate sequentially
    // int* heapVar2 = new int(99);
    char* heapVar1 = (char*)malloc(1024);
    char* heapVar2 = (char*)malloc(1024);

    cout << "=== MEMORY SEGMENT BOUNDARIES ===" << endl;
    cout << endl;

    // TEXT segment (code/instructions)
    cout << "--- TEXT SEGMENT (Code) ---" << endl;
    cout << "Address of main()       : " << (void*)&main << endl;
    cout << "Address of checkStack() : " << (void*)&checkStack << endl;
    cout << endl;

    // DATA segment (initialized globals)
    cout << "--- DATA SEGMENT (Initialized Globals) ---" << endl;
    cout << "globalVar  addr : " << (void*)&globalVar
         << " value: " << globalVar << endl;
    cout << "staticVar  addr : " << (void*)&staticVar
         << " value: " << staticVar << endl;
    cout << endl;

    // BSS segment (uninitialized globals)
    cout << "--- BSS SEGMENT (Uninitialized Globals) ---" << endl;
    cout << "uninitGlobal  addr: " << (void*)&uninitGlobal
         << " value: " << uninitGlobal << endl;
    cout << "uninitGlobal2 addr: " << (void*)&uninitGlobal2
         << " value: " << uninitGlobal2 << endl;
    cout << endl;

    // STACK segment - cross-function comparison (guaranteed)
    checkStack(&mainVar);

    // HEAP segment (dynamic - grows UP)
    cout << "--- HEAP SEGMENT (Dynamic Allocation) ---" << endl;
    cout << "heapVar1 addr  : " << (void*)heapVar1 << endl;
    cout << "heapVar2 addr  : " << (void*)heapVar2 << endl;
    cout << "Heap grows: "
         << (heapVar2 > heapVar1 ? "UP (lower to higher)" : "DOWN")
         << endl;
    cout << endl;

    // Summary - relative positions
    cout << "=== RELATIVE POSITION SUMMARY ===" << endl;
    cout << "TEXT  (lowest) : " << (void*)&main      << endl;
    cout << "DATA           : " << (void*)&globalVar  << endl;
    cout << "BSS            : " << (void*)&uninitGlobal << endl;
    cout << "HEAP           : " << (void*)heapVar1   << endl;
    cout << "STACK (highest): " << (void*)&mainVar   << endl;

    // cleanup
    free(heapVar1);
    free(heapVar2);

    return 0;
}