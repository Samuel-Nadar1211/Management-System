Management System implemented using OOPs Concept in C++

# Library Management System

Library Management System is implemented as a command line application. There are predefined commands for running the software.

### 1. Inventory Management

* Add copies of book to the inventory
* Issue copies and maintain remaining available copies
* Maintain reservation of copies

### 2. User Management

* Add library members by giving unique id
* Maintain the books issued by the member
* Calculate the fine owned till date
* Calculate the fine payed

### 3. Fine

- There is no fine for the first two weeks
- For the next two weeks the fine is Rs. 2 per day
- After four weeks the fine is Rs. 5 per day
- Return copies along with paying the fine

### 4. Reservation

- If there is more reservation then available copies unreserved members cannot issue the copy, only reserved member can issue the copy if available
- If the member does not own any fine only then they can reserve the copy, i.e. only after returning the copy and paying fine a member can reserve a copy
- At any time if reserved member owns fine after reservation, the reservation get cancelled

### Screenshot

![Screenshot 2024-07-08 130617](https://github.com/Samuel-Nadar1211/Management-System/assets/98077236/ce3c7296-b9da-44a1-a368-903387f7ee92)

<br>

# Student Exam Management System

Student Exam Management System is implemented as an interactive applications.

### Assumptions

* In an academic year there will be many class, namely 1st CSE or 3rd ECE
* A class will have a maximum of 5 subjects
* A class will have many test, namely Cycle Test, Assignments or End Sem for subject
* Result is available for each tests
* If a student fails in one subject then he fails the test in the class
* Final Result is based on the average percentage of each subject

### Features

1. Store student exam details manually
2. Calculate the result
3. Get all the student exam details of a particular student as a JSON File
4. Retrieve student exam details from database / binary files (binary file handling)
5. Store / Update student exam details to database / binary files in sorted order
6. Find toppers in all class for a particular test conducted in academic year
7. Find students who have failed from a class for a particular test conducted in academic year

### Screenshot

![image](https://github.com/Samuel-Nadar1211/Management-System/assets/98077236/b16c5515-5a8a-4c38-a1a4-1011cdb1adfc)

