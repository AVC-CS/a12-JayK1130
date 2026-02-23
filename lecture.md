# A12. Understanding Unix Memory Structures

## 1. Program vs Process

A **program** is a passive file on disk — your compiled executable (`a.out`).

A **process** is a running instance of that program in memory.

When you type `./a.out`, the OS loader:
- Reads the ELF binary from disk
- Allocates memory and creates 5 segments (TEXT, DATA, BSS, HEAP, STACK)
- Copies machine code into TEXT and initialized data into DATA
- Zeros out BSS, sets up STACK and HEAP pointers
- Jumps to `main()` and execution begins

One program can spawn multiple processes. Each process gets its own isolated memory space.

## 2. The 5 Unix Memory Segments

```
High Address  0x7fffffff

|            |
|   STACK    |   ↓ grows down (local vars, function frames)
|     ↓      |
|            |
|  (free     |
|   space)   |
|            |
|     ↑      |
|   HEAP     |   ↑ grows up (new, malloc)
|            |
|   BSS      |   uninitialized globals (zeroed by OS)
|            |
|   DATA     |   initialized globals/statics
|            |
|   TEXT     |   machine code (read-only)
|            |

Low Address   0x00000000
```

### TEXT Segment (Code)
- Contains compiled machine instructions
- Read-only and shared between processes running the same program
- Address of `main()` lives here

```
// TEXT: function addresses
cout << (void*)&main;    // prints a TEXT address
```

### DATA Segment (Initialized Globals)
- Global and static variables that have initial values
- Read-write

```
int globalVar = 100;        // DATA segment
static int staticVar = 200; // DATA segment
```

### BSS Segment (Uninitialized Globals)
- Global and static variables declared without initial values
- Automatically zeroed by the OS at program start
- Saves space in the executable file (no need to store zeros)

```
int uninitGlobal;  // BSS segment — value is 0
```

### HEAP Segment (Dynamic Memory)
- Memory allocated at runtime with `new` or `malloc`
- Grows **upward** toward higher addresses
- You must free it (`delete` / `free`) or you leak memory

```
int* p1 = new int(42);          // HEAP
int* p2 = (int*)malloc(sizeof(int)); // HEAP
// p2 has a HIGHER address than p1 (heap grows up)
```

### STACK Segment (Local Variables)
- Local variables, function parameters, return addresses
- Grows **downward** toward lower addresses
- Automatically managed (created on function entry, destroyed on exit)

```
int stackVar1 = 10;  // STACK (higher address)
int stackVar2 = 20;  // STACK (lower address — stack grew down)
```

## 3. Walk-through of main.cpp

The demo program `main.cpp` declares variables in all 5 segments and prints their addresses:

**Global scope (before main):**
- `int globalVar = 100;` — DATA segment (initialized global)
- `static int staticVar = 200;` — DATA segment (static initialized)
- `int uninitGlobal;` — BSS segment (uninitialized global)

**Inside main():**
- `int stackVar1, stackVar2` — STACK segment (local variables)
- `int* heapVar1 = new int(42)` — HEAP segment (dynamic allocation)
- `int* heapVar2 = (int*)malloc(sizeof(int))` — HEAP segment

**Output shows:**
- TEXT addresses are the lowest (code lives at bottom)
- DATA and BSS are above TEXT
- HEAP addresses are above BSS and grow upward (heapVar2 > heapVar1)
- STACK addresses are the highest and grow downward (stackVar1 > stackVar2)

This matches the memory layout diagram exactly.

## 4. Student Activity

Write your own C++ program that demonstrates all 5 memory segments:

- Declare at least one initialized global (DATA)
- Declare at least one uninitialized global (BSS)
- Declare at least two local variables (STACK)
- Allocate at least two variables on the heap with `new` (HEAP)
- Print the address of `main()` (TEXT)
- Print all addresses with segment labels
- Show that stack grows down and heap grows up
- Free all heap allocations with `delete`
- Run: `./a.out > result.txt`

### How to compile and test
```
g++ -std=c++17 -Wall -Wextra main.cpp -o a.out
./a.out > result.txt
pytest -rP
```

### Grading (100 pts total)
- Compile (20 pts): compiles with `-Wall -Wextra`
- T1 (20 pts): `result.txt` contains all 5 segment labels
- T2 (20 pts): `result.txt` contains hex addresses
- T3 (20 pts): stack addresses decrease (grows down)
- T4 (20 pts): heap addresses increase (grows up)
