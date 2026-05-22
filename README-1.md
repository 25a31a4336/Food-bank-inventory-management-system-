# Food Bank Inventory Management System

A C++ project to manage food donations and track expiry dates using Priority Queue.

## Features
- Add food donations
- Display inventory
- Generate expiry alerts
- Remove expired food items
- Search food items
- Save and load data using file handling

## Technologies Used
- C++
- OOP
- STL Priority Queue
- File Handling
- Vectors

## Data Structures Used
- Vector
- Priority Queue

## How It Works
The system compares food expiry dates with the current system date and generates alerts for:
- Expired items
- Items expiring today
- Items expiring within 3 days

## How to Run

Compile:

```bash
g++ main.cpp -o foodbank