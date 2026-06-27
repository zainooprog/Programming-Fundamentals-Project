# Sweet Crave Corner

A Bakery Management System built in **C language** for managing products, customers, employees, and sales records.  
This project was developed as part of the Programming Fundamentals course.

## Features
- **Admin Panel**  
  - Add, delete, search, update products  
  - View inventory  
  - View sales log  
  - Sort products by price  
  - Show most expensive item  

- **Employee Panel**  
  - Login with employee credentials  
  - Add, delete, search, update products  
  - Verify payments  
  - Approve or reject pending orders  
  - View inventory and sales log  

- **Customer Panel**  
  - Sign up and login  
  - Browse menu  
  - Add items to cart  
  - View cart  
  - Place orders (saved to pending orders and sales log)  

## Technologies Used
- C programming language  
- File handling (`stdio.h`) for persistent storage  
- Arrays and pointers for managing products, customers, employees, and orders  
- String handling (`string.h`) for searching and updating records  

## How to Run
1. Clone or download the repository.  
2. Open the project in any C IDE (e.g., Dev-C, Code::Blocks, Turbo C).  
3. Compile and run `Final PF Project.c`.  
4. Follow the on-screen menu to interact with Admin, Employee, or Customer sections.  

## Project Structure
- `Final PF Project.c` → Main source code  
- `README.md` → Project description  
- `products.txt` → Stores product details  
- `customers.txt` → Stores customer accounts  
- `employees.txt` → Stores employee accounts  
- `sales_log.txt` → Records placed orders and approvals  
- `pending_orders.txt` → Tracks orders awaiting approval  

## Purpose
This project demonstrates the use of **file handling, arrays, pointers, and functions** in C by simulating a real-world bakery management system with multiple user roles.

## Authors
Developed by Syeda Zainab Sohail and Amna Waseem
