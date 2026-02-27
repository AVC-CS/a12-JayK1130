#include <iostream>
#include <cstdlib>
#include <cstdint>

using namespace std;

int globalVar = 100;
static int staticVar = 200;

int uninitGlobal1;
int uninitGlobal2;

void checkStack(int* parentAddr) {
    int childVar = 0;

    cout << "=== STACK SEGMENT (Cross-function comparison) ===" << endl;
    cout << "parentAddr value (main local): " << (void*)parentAddr << endl;
    cout << "&parentAddr (param addr):      " << (void*)&parentAddr << endl;
    cout << "&childVar (child local):       " << (void*)&childVar << endl;

    if ((uintptr_t)parentAddr > (uintptr_t)&childVar) {
        cout << "Stack grows DOWN" << endl;
    } else {
        cout << "Stack grows UP" << endl;
    }

    cout << endl;
}

int main() {
    cout << "=== MEMORY SEGMENT BOUNDARIES ===" << endl << endl;

    cout << "=== TEXT SEGMENT (Code) ===" << endl;
    cout << "Address of main():        " << (void*)&main << endl;
    cout << "Address of checkStack(): " << (void*)&checkStack << endl;
    cout << endl;

    cout << "=== DATA SEGMENT (Initialized Globals) ===" << endl;
    cout << "&globalVar = " << (void*)&globalVar << ", value = " << globalVar << endl;
    cout << "&staticVar = " << (void*)&staticVar << ", value = " << staticVar << endl;
    cout << endl;

    cout << "=== BSS SEGMENT (Uninitialized Globals) ===" << endl;
    cout << "&uninitGlobal1 = " << (void*)&uninitGlobal1 << endl;
    cout << "&uninitGlobal2 = " << (void*)&uninitGlobal2 << endl;
    cout << endl;

    int localVar = 42;
    checkStack(&localVar);

    cout << "=== HEAP SEGMENT (Dynamic Allocation) ===" << endl;
    char* p1 = (char*)malloc(1024);
    char* p2 = (char*)malloc(1024);

    cout << "p1 = " << (void*)p1 << endl;
    cout << "p2 = " << (void*)p2 << endl;

    if ((uintptr_t)p1 < (uintptr_t)p2) {
        cout << "Heap grows UP" << endl;
    } else {
        cout << "Heap grows DOWN" << endl;
    }

    free(p1);
    free(p2);

    return 0;
}