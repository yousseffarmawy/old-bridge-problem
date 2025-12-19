# Old Bridge Problem – Multithreaded C++ Simulation

## Overview
This project implements a classic synchronization problem known as the **Old Bridge Problem**.
Multiple cars attempt to cross a single bridge while respecting strict concurrency constraints.

The solution is implemented using **C++ multithreading primitives** to ensure safety, fairness, and correctness.

## Problem Requirements
- At most **3 cars** can be on the bridge at the same time.
- All cars on the bridge must move in the **same direction**.
- When the bridge becomes empty, waiting cars are allowed to enter immediately.
- Cars going in the same direction may enter as long as the bridge is not full.
- Only **one thread** can access shared state at any time.

## Solution Approach
- Used **std::thread** to represent each car as a separate thread.
- Applied **std::mutex** and **std::condition_variable** to synchronize access to shared resources.
- Implemented direction control and capacity checks to prevent race conditions.
- Ensured **fairness** by allowing waiting cars to enter once conditions are satisfied.
- Encapsulated synchronization logic using **Object-Oriented Design**.

## Technologies
- C++
- std::thread
- std::mutex
- std::condition_variable

## How to Run
```bash
g++ bridge.cpp -o bridge -pthread
./bridge
