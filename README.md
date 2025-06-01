# Advanced-C-project
Command-line system in C for managing doctors, patients and queue operations

# Medical Queue Management System (C Project)

This is a command-line based system for managing queues of patients and doctors in a medical clinic environment.

## 🔧 Features
- Manage and store patient and doctor records
- Handle patient queue operations (enqueue, dequeue, prioritization)
- File-based data persistence using `.txt` files
- Modular architecture using C headers and source files
- Includes queue and tree data structures

## 🗂️ Project Structure
- `main.c` – Entry point
- `doctors.c/h`, `patient.c/h` – Doctor and patient logic
- `pline.c/h`, `ptree.c/h` – Queue and tree data structures
- `common.c/h`, `structs.h` – Shared logic and data types
- `Doctors.txt`, `Patients.txt`, `Line.txt` – Sample data files

## 💻 Compilation
```bash
gcc main.c common.c doctors.c patient.c pline.c ptree.c -o clinic
