# Car-Rental-Project
Console-based Car Rental Management System in C++ with user authentication (SHA-1 hashed passwords), admin/client roles, car inventory management, and reservation booking — data persisted via CSV files.
Car Rental Management System

A console-based Car Rental Management System built in C++, featuring user authentication, admin/client role separation, car inventory management, and a full reservation (rent/cancel/modify) workflow — with data persisted to CSV files.

Features


User Accounts

Sign-up with validated first/last name, password, email, and phone number (regex-based validation)
Passwords are hashed with SHA-1 before being stored
Login authentication against stored credentials



Admin Panel (Admin = User ID 1)

Add a new car to the fleet
Remove a car by plate number
Modify existing car details



Client Panel

Browse available cars
Rent a car for a given date range (with overlap/availability checking)
Cancel an existing reservation
Modify an existing reservation's dates



Data Persistence

Users, cars, and rentals are stored in .csv files (users.csv, cars.csv, rented_cars.csv) and reloaded on every run



Input Validation

Name, email, phone number, password strength, and date format (YYYY-MM-DD) are all validated before being accepted





Tech Stack


Language: C++
IDE/Build: Visual Studio (.sln / .vcxproj)
Libraries: Standard C++ (iostream, fstream, regex, ctime, sstream), custom SHA-1 implementation (sha1.h/sha1.cpp)


Project Structure

Car_Rental_Project/
├── Car_Rental_Project.cpp     # Main program logic
├── sha1.h / sha1.cpp          # SHA-1 hashing implementation for passwords
├── cars.csv                   # Car inventory data
├── users.csv                  # Registered user data
├── Car_Rental_Project.sln     # Visual Studio solution file
├── Car_Rental_Project.vcxproj # Visual Studio project file
└── x64/                       # Build output (ignored in .gitignore)

How It Works


On first run, if users.csv or cars.csv don't exist, they are created automatically.
The first registered user (User ID 1) acts as the admin and gets access to car management actions.
All other users are treated as clients and can rent, cancel, or modify their own reservations.
Reservation conflicts are checked against rented_cars.csv before a rental is confirmed.


Possible Future Improvements


Replace raw CSV parsing with a more robust format (e.g. JSON) or a lightweight database
Add unit tests for validation functions
Cross-platform support (currently uses Windows.h)
Export reservation summaries as an actual PDF instead of plain text


License

This project is open source and available for learning purposes.
