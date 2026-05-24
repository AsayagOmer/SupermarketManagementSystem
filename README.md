# 🛒 Supermarket Management System

> **A Personal Note:** I built this project as a milestone in my programming journey. Prior to January 2025, I had absolutely zero knowledge of the C programming language. This repository represents my rapid progress from writing my first `printf` to implementing complex data structures and simulating object-oriented concepts in a procedural language.

## 📖 About The Project

This is a comprehensive, Command-Line Interface (CLI) Supermarket Management System written entirely in C. It provides an interactive menu-driven environment to manage supermarket operations, track inventory, handle customer registrations, and simulate real-time shopping sessions.

**Note:** This project was developed, compiled, and tested specifically on **Windows 10**.

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
   git clone [https://github.com/GurevichTom/DigitalStore.git](https://github.com/GurevichTom/DigitalStore.git)
   cd DigitalStore