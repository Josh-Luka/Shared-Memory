Parallel Palindrome & Path Optimization Suite

This project is a dual-language repository featuring High-Performance C Multiprocessing and Complex Python Algorithmic Solvers. It explores Inter-Process Communication (IPC), shared memory management, and advanced combinatorial optimization.

Project Components

1. C Multiprocessing Framework
   
A system-level implementation of parallel task delegation.

Manager.c: The parent orchestrator. It creates POSIX shared memory segments, forks child processes, and coordinates task distribution.

Palindrome.c: The worker process. It attaches to shared memory, performs computational checks, and reports results back to the parent.

shared_mem.h: A shared header defining the data contract (shared_mem_t) and POSIX source macros.

Makefile: A build script configured with -O3 optimizations and standard warning flags.

2. Python Algorithm Suite
   
Advanced logic for graph theory and optimization problems.

Genetic TSP Solver: A Genetic Algorithm (GA) using Partially Mapped Crossover (PMX) and Tournament Selection to solve the Traveling Salesperson Problem.

Graph Path Search: A recursive backtracking DFS designed to find palindromic sequences within edge-weighted graphs.

Installation & Setup

Prerequisites

OS: Linux, macOS, or any POSIX-compliant Unix system.

Compiler: gcc

Python: Version 3.8+

Tools: make

Building the C Framework

To compile the Manager and Palindrome worker, run:

Bash
make

Execution

Running the Multi-Processed Checker

Pass a list of strings to the Manager. Each string will be processed by a unique child process:

Bash

./Manager racecar algorithm madam kayak

Running Python Algorithms

Ensure your local environment includes the edgegraph and util modules referenced in the code, then run:

Bash
python3 your_script_name.py

Technical Implementation Details
IPC Handshake Workflow

The C system uses a specialized communication flow:

Shared Memory: The Manager creates a memory segment via shm_open and mmap.

The Pipe: The Manager writes the name of the shared memory segment into a pipe.

Exec: The child process is transformed into the ./Palindrome executable via execl, receiving the pipe's file descriptor as an argument.

Reporting: The child writes the result ($1$ or $0$) back into the shared structure, which the Manager reads after the child exits.
