
# 🛒 Supermarket Management System

> **A Personal Note:** 
<br>I built this project as a milestone in my programming journey.
<br>Prior to October 2024, I had absolutely zero knowledge of the C programming language.
<br>This repository represents my rapid progress from writing my first `printf` to implementing complex data structures and simulating object-oriented concepts in a procedural language.
<br>
<br> <b>It was made in 2 weeks.</b>

## 📖 About The Project

This is a comprehensive, Command-Line Interface (CLI) Supermarket Management System written entirely in C. 
<br>It provides an interactive menu-driven environment to manage supermarket operations, track inventory, handle customer registrations, simulate real-time shopping sessions, and analyze purchasing data.

## 🛠️ Development Environment

This project was built, compiled, and tested using the following environment and tools:
* **Operating System:** Windows 10
* **IDE:** CLion
* **Compiler:** GCC 14.2 (via MinGW)
* **C Standard:** C17

## ✨ Features

Based on the core menu system, the application supports:
* **Supermarket Overview:** Display the current state of the supermarket.
* **Inventory Management:** Add new products and restock existing items.
* **Customer Operations:** Register new customers with unique ID validation.
* **Interactive Shopping:** Allow customers to start shopping sessions, add items to their carts, and checkout.
* **Cart Management:** View and dynamically modify shopping cart contents using a Linked List.
* **Product Filtering & Sorting:** Filter products by type, name (substring search), price range, and expiry date. Sort by price or name.
* **Data Persistence:** Automatically save and load products, customers, and purchase history to/from text files.
* **Data Extraction & Analytics:** View customer purchase history, top customers by spending, top products by units sold, and revenue reports.

## 🧠 Technical Highlights & Skills Applied

This project heavily utilizes advanced C concepts:

* **V-Tables (Virtual Method Tables):** Implemented to simulate Object-Oriented Programming (OOP) polymorphism in C. Each product type (Fruit & Vegetable, Fridge, Frozen, Shelf) has its own V-Table with specialized print, discount calculation, and expiry validation functions.
* **Linked Lists:** Shopping carts use a singly linked list data structure. Items are dynamically added, searched, and removed via pointer-based traversal.
* **File Handling:** Full persistent storage implementation. Products, customers, and purchase history are saved to text files in the `data/` directory and automatically loaded on startup.
* **Generic Programming:** The `utils.c` module provides generic `arraySearch` and `printArray` functions using `void*` pointers and function pointer callbacks — a form of runtime polymorphism.
* **Manual Memory Management:** Extensive use of pointers, `malloc`, `calloc`, and `free` to allocate memory dynamically while ensuring no memory leaks occur upon program termination (`finalizeSuperMarket`).

## 🚀 Getting Started

### Prerequisites
To compile and run this project, you will need a C compiler such as `GCC` (MinGW recommended for Windows). 

### Installation & Execution

1. **Clone the repository:**
```bash
   git clone https://github.com/AsayagOmer/SupermarketManagementSystem.git
   cd SupermarketManagementSystem

```

2. **Compile the source code (using CMake):**
```bash
   cmake -B build -DCMAKE_C_STANDARD=17
   cmake --build build

```

3. **Or compile directly with GCC:**
```bash
   gcc -std=c17 -o supermarket main.c date.c utils.c product.c customer.c shopping_cart.c shoppingitem.c supermarket.c vtable.c fileio.c analytics.c

```

4. **Run the executable:**

```bash
   # On Windows
   supermarket.exe
   
   # On Linux/macOS (if utilizing a compatible compiler)
   ./supermarket
```


## 🎮 Usage

Upon launching the application, you will be greeted with an interactive main menu:

0. **View Supermarket Status:** Check current inventory levels and view all registered customers.
1. **Add Product:** Enter details for new items or restock existing ones.
2. **Add Customer:** Register a new shopper with validated name and 9-digit ID.
3. **Customer Shopping:** Select a customer and add products to their cart by barcode.
4. **Print Shopping Cart:** View a customer's current cart with item breakdown and total.
5. **Cart Management:** Pay for or cancel a customer's shopping cart.
6. **Product Filter & Sort:** Access the filter/sort sub-menu (by type, name, price, expiry, or sort options).
7. **Data Extraction & Analytics:** View purchase history, top customers, top products, and revenue reports.
8. **Save Data:** Manually save all current data to files.
-1. **Exit:** Safely terminate the program. This triggers data saving and safely frees all allocated memory.

## 📂 Project Structure

A brief overview of how the codebase is organized:

| File | Description |
|------|-------------|
| `main.c` | Entry point and main menu loop |
| `supermarket.h/c` | Core supermarket operations, filtering, and sorting |
| `customer.h/c` | Customer management (registration, search, comparison) |
| `product.h/c` | Product definitions, barcode generation, type management |
| `shopping_cart.h/c` | Shopping cart linked list operations |
| `shoppingitem.h/c` | Shopping item node definition and initialization |
| `vtable.h/c` | Virtual Method Tables for product type polymorphism |
| `fileio.h/c` | File I/O for data persistence (save/load) |
| `analytics.h/c` | Data extraction and purchase analytics |
| `date.h/c` | Date structure with validation and formatting |
| `utils.h/c` | Generic utility functions (search, print, input) |
| `data/` | Directory for persistent data files (auto-created) |

## 📄 License

Distributed under the Unlicensed license. See `Unlicense license` for more information.
