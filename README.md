# Inventory Management System

A simple **C++ console-based inventory system** that lets players pick, drop, equip, and unequip items.

---

## Features

- Pick and drop items between inventory and available items.  
- Equip and unequip items using a hotbar.  
- Display inventory and hotbar contents.  
- Uses **smart pointers** (`shared_ptr` and `weak_ptr`) for memory management.

---

## How to Run

1. Clone the repo:  
   ```bash
   git clone https://github.com/Eyuzikfr/InventoryManagement.git

2. Navigate to the folder containing the main program:
   ```bash
   cd InventoryManagement
   cd InventoryManagement

3. Compile the program (example with g++):
   ```bash
     g++ -std=c++17 InventoryManagement.cpp -o InventoryManagement
   
4. Run the Program
   ```bash
   ./InventoryManagement
   ```
   or
   ```bash
   InventoryManagement.exe

P.S: For now, only compatible with Windows OS due to the usage of system("cls").
