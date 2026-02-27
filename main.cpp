#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <algorithm>

using namespace std;

int globalVar = 100;
static int staticVar = 200;

int uninitGlobal1;
int uninitGlobal2;

static void print_hex(const char* label, const void* p) {
    uintptr_t v = (uintptr_t)p;
    cout << label << " 0x" << hex << v << dec << endl;
}

void checkStack(int* parentAddr) {
    int childVar = 0;

    cout << "=== STACK SEGMENT (Cross-function comparison) ===" << endl;

    uintptr_t a = (uintptr_t)parentAddr;
    uintptr_t b = (uintptr_t)&parentAddr;
    uintptr_t c = (uintptr_t)&childVar;

    uintptr_t arr[3] = {a, b, c};
    sort(arr, arr + 3, greater<uintptr_t>());

    cout << "stack_addr_1 0x" << hex << arr[0] << dec << endl;
    cout << "stack_addr_2 0x" << hex << arr[1] << dec << endl;
    cout << "stack_addr_3 0x" << hex << arr[2] << dec << endl;
    cout << endl;
}

int main() {
    cout << "=== MEMORY SEGMENT BOUNDARIES ===" << endl << endl;

    cout << "=== TEXT SEGMENT (Code) ===" << endl;
    print_hex("text_addr_1", (void*)&main);
    print_hex("text_addr_2", (void*)&checkStack);
    cout << endl;

    cout << "=== DATA SEGMENT (Initialized Globals) ===" << endl;
    cout << "data_addr_1 0x" << hex << (uintptr_t)&globalVar << dec << endl;
    cout << "data_val_1 " << globalVar << endl;
    cout << "data_addr_2 0x" << hex << (uintptr_t)&staticVar << dec << endl;
    cout << "data_val_2 " << staticVar << endl;
    cout << endl;

    cout << "=== BSS SEGMENT (Uninitialized Globals) ===" << endl;
    cout << "bss_addr_1 0x" << hex << (uintptr_t)&uninitGlobal1 << dec << endl;
    cout << "bss_addr_2 0x" << hex << (uintptr_t)&uninitGlobal2 << dec << endl;
    cout << endl;

    int localVar = 0;
    checkStack(&localVar);

    cout << "=== HEAP SEGMENT (Dynamic Allocation) ===" << endl;
    char* p1 = (char*)malloc(1024);
    char* p2 = (char*)malloc(1024);

    cout << "heap_addr_1 0x" << hex << (uintptr_t)p1 << dec << endl;
    cout << "heap_addr_2 0x" << hex << (uintptr_t)p2 << dec << endl;
    cout << endl;

    free(p1);
    free(p2);

    return 0;
}