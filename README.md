# Library Management System in C

A console-based library management system developed in C, using a chained hash table data structure for efficient data storage and retrieval.

This project was built for educational purposes, focusing on dynamic data structures, modular programming, and input validation.

---

## Project Overview

The system manages a collection of books using a hash table with separate chaining to handle collisions.

It was designed to reinforce concepts such as:

- Dynamic memory allocation
- Linked lists
- Hash functions
- Collision handling
- Modular code organization
- User input validation

---

## Features

- Add books (title, author, year, genre)
- Automatic ID generation with ID reuse
- Borrow and return books
- Remove books by ID
- Search books by ID
- List all registered books
- Display internal hash table structure
- Fully clear the library (free allocated memory)

---

## Data Structure

The system uses:

- Hash table
- Separate chaining (linked lists) for collision handling
- Dynamic memory allocation via `malloc` and `free`

This ensures efficient insertion, search, and deletion operations.

---

## Technologies

- C Programming Language
- Standard Libraries:
  - `stdio.h`
  - `stdlib.h`
  - `string.h`
  - `ctype.h`
  - `locale.h`

---

## How to Compile

```bash
gcc main.c -o library
./library
