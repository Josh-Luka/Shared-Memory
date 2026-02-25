
# Parallel Palindrome & Path Optimization Suite

This repository contains a hybrid collection of system-level C programs and high-level Python algorithms. It explores the intersection of **Inter-Process Communication (IPC)** and **Combinatorial Optimization**.

## 📁 Project Structure

### 1. Multiprocess C Framework

A robust implementation of parallel processing using the parent-worker model.

-   **`Manager.c`**: The orchestrator. It manages the lifecycle of child processes, creates POSIX shared memory segments, and handles task distribution.
    
-   **`Palindrome.c`**: The worker. It utilizes a two-pointer algorithm to verify palindromes, reading from and writing to shared memory.
    
-   **`shared_mem.h`**: The central header file defining the `shared_mem_t` structure and POSIX feature macros.
    
-   **`Makefile`**: Automates compilation with high-level `-O3` optimizations.
    

### 2. Python Algorithm Suite

Complex solvers for graph-based and optimization problems.

-   **Genetic TSP Solver**: Implements a Genetic Algorithm to solve the Traveling Salesperson Problem using **Partially Mapped Crossover (PMX)** and **Elitism**.
    
-   **Graph Path Search**: A recursive backtracking DFS that identifies palindromic sequences within edge-weighted graphs.
    

----------

## 🚀 Getting Started

### Prerequisites

-   **C Environment**: A Linux/Unix-based system with `gcc` and `make`.
    
-   **Python Environment**: Python 3.x installed.
    

### Compilation

Build the C components using the provided Makefile:

Bash

```
make

```

### Execution

#### Running the Multi-Processed Checker

Pass a list of strings to the `Manager`. Each string will be processed by a unique child process:

Bash

```
./Manager racecar algorithm madam kayak

```

#### Running Python Algorithms

Ensure your local environment includes the `edgegraph` and `util` modules referenced in the code, then run:

Bash

```
python3 your_script_name.py

```

----------

## 🛠 Technical Implementation Details

### IPC Handshake Workflow

The C system uses a specialized communication flow:

1.  **Shared Memory**: The Manager creates a memory segment via `shm_open` and `mmap`.
    
2.  **The Pipe**: The Manager writes the _name_ of the shared memory segment into a pipe.
    
3.  **Exec**: The child process is transformed into the `./Palindrome` executable via `execl`, receiving the pipe's file descriptor as an argument.
    
4.  **Reporting**: The child writes the result ($1$ or $0$) back into the shared structure, which the Manager reads after the child exits.
    



## 🧹 Maintenance

To remove compiled executables and object files, run:

Bash

```
make clean

```
