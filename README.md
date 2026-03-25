🧠 Custom Memory Allocator in C

A low-level systems project that simulates the core behavior of malloc() and free() using manual heap management.

🚀 Overview

This project implements a simple memory allocator using a fixed-size memory pool. It demonstrates how dynamic memory allocation works internally in operating systems and runtime libraries.

⚙️ Features
Manual heap management using a static memory buffer
First-fit allocation strategy
Block splitting for efficient memory usage
Block coalescing (merging) to reduce fragmentation
Linked-list based memory tracking
Debug visualization of memory layout
🧱 Memory Model

The allocator uses a contiguous memory region:

[ Block Metadata | User Data | Block Metadata | User Data | ... ]

Each block contains:

Size of the block
Free/allocated flag
Pointer to next block
🔄 Allocation Strategy
First-Fit:

The allocator scans memory and selects the first block large enough to satisfy the request.

✂️ Block Splitting

If a block is larger than required:

It is split into:
Allocated block
Remaining free block
🔗 Coalescing (Merging)

Adjacent free blocks are merged to reduce external fragmentation.

🧠 Key Concepts Demonstrated
Heap memory management
Pointer arithmetic
Memory fragmentation (internal & external)
Linked list manipulation
Low-level system design principles
🧪 Sample Output
Allocated 200 bytes
Allocated 100 bytes

Memory Layout:
[Size: 200 | Free: 0] -> [Size: 100 | Free: 0] -> [Size: 680 | Free: 1] -> NULL

Freed memory block

Memory Layout:
[Size: 200 | Free: 1] -> [Size: 100 | Free: 0] -> [Size: 680 | Free: 1] -> NULL
▶️ How to Run
gcc src/allocator.c -o allocator
./allocator
📌 Future Improvements
Best-fit / Worst-fit allocation strategies
Thread-safe allocator (mutex-based)
Dynamic memory pool expansion
Visualization using GUI