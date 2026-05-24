
# 🛒 Supermarket Management System

> **A Personal Note:** <br>I built this project as a milestone in my programming journey.
<br>Prior to October 2024, I had absolutely zero knowledge of the C programming language.
<br>This repository represents my rapid progress from writing my first `printf` to implementing complex data structures and simulating object-oriented concepts in a procedural language.
<br>
<br> <b>It was made in 2 weeks.</b>

## 📖 About The Project

This is a comprehensive, Command-Line Interface (CLI) Supermarket Management System written entirely in C. 
<br>It provides an interactive menu-driven environment to manage supermarket operations, track inventory, handle customer registrations, and simulate real-time shopping sessions.

## 🛠️ Development Environment

This project was built, compiled, and tested using the following environment and tools:
* **Operating System:** Windows 10
* **IDE:** CLion
* **Compiler:** GCC 14.2 (via MinGW)
* **C Standard:** C17

## ✨ Features

Based on the core menu system, the application supports:
* **Supermarket Overview:** Display the current state of the supermarket.
* **Inventory Management:** Add new products and print existing products filtered by their specific types.
* **Customer Operations:** Register new customers into the system.
* **Interactive Shopping:** Allow customers to start shopping sessions, add items to their carts, and checkout.
* **Cart Management:** View and dynamically modify the contents of a shopping cart.

## 🧠 Technical Highlights & Skills Applied

This project heavily utilizes advanced C concepts. Here is what I learned and implemented:

* **V-Tables (Virtual Method Tables):** Implemented to simulate Object-Oriented Programming (OOP) polymorphism in C. This allows the system to handle different structural types (e.g., various product categories) dynamically at runtime.
* **Linked Lists:** Used to create dynamic data structures. Instead of relying on fixed-size arrays, the system can gracefully handle a growing number of customers and cart items, managing memory efficiently.
* **File Handling:** Implemented persistent storage. The application can read from and write to external files, ensuring that data (like inventory and customer lists) is saved between sessions.
* **Manual Memory Management:** Extensive use of pointers, `malloc`, `calloc`, and `free` to allocate memory dynamically while ensuring no memory leaks occur upon program termination (`finalizeSuperMarket`).

## 🚀 Getting Started

### Prerequisites
To compile and run this project, you will need a C compiler such as `GCC` (MinGW recommended for Windows). 

### Installation & Execution

1. **Clone the repository:**
```bash
   git clone [https://github.com/AsayagOmer/SupermarketManagementSystem.git](https://github.com/AsayagOmer/SupermarketManagementSystem.git)
   cd SupermarketManagementSystem

```

2. **Compile the source code:**
Using GCC, compile the C files. *(Note: Adjust the file paths below if your source files are organized in specific directories).*

```bash
   gcc -std=c17 -o supermarket main.c

```

3. **Run the executable:**

```bash
   # On Windows
   supermarket.exe
   
   # On Linux/macOS (if utilizing a compatible compiler)
   ./supermarket
```


## 🎮 Usage

Upon launching the application, you will be greeted with an interactive main menu. Use your keyboard to input the number corresponding to your desired action and press `Enter`:

1. **View Supermarket Status:** Check current inventory levels and view all registered customers.
2. **Manage Inventory:** Enter details for new items (name, category, price) to stock the shelves.
3. **Register Customer:** Input a new shopper's details to dynamically add them to the linked list database.
4. **Initiate Shopping Session:** Select a registered customer and begin adding items from the inventory to their cart.
5. **Checkout:** Calculate the total cost of a customer's cart, process the "payment," and finalize their session.
6. **Exit:** Safely terminate the program. This triggers the data saving process and safely frees all allocated memory.

## 📂 Project Structure

A brief overview of how the codebase is organized:

* `main.c` - The entry point and main menu loop of the application.
* `include/` - Contains the header files (`.h`) declaring structures, v-tables, and function prototypes.
* `src/` - Contains the implementation files (`.c`) for core modules (e.g., inventory management, customer linked lists, cart operations).
* `data/` - Directory where text or binary files are stored for persistent system data.

## 🔮 Future Improvements

While this project marks a significant milestone in my C learning journey, there is always room for expansion. Potential future updates include:

* **Binary File I/O:** Transitioning from plain text files to binary files for faster and more secure data storage.
* **Advanced Algorithms:** Implementing Merge Sort or Quick Sort to organize inventory by price or name, and Binary Search for faster customer lookups.
* **Robust Input Validation:** Hardening the `scanf` and string-parsing logic to completely prevent user-induced crashes or buffer overflows.

## 📄 License

Distributed under the Unlicensed license. See `Unlicense license` for more information.

## 📬 Contact

Omer Asayag - [GitHub Profile](https://www.google.com/search?q=https://github.com/AsayagOmer)

Project Link: [https://github.com/AsayagOmer/SupermarketManagementSystem](https://www.google.com/search?q=https://github.com/AsayagOmer/SupermarketManagementSystem)
