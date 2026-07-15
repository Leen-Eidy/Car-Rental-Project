#include <iostream>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <regex>
#include<sstream>
#include"sha1.h"
#include<Windows.h>
using namespace std;

struct Date {
    string startDate;
    string endDate;
};

struct Car {
    string plateNumber;
    string brand;
    string model;
    int year;
    string color;
    double pricePerDay;
    Date d;
};

struct Client {
    int userID;
    string firstName;
    string lastName;
    string password;
    string phone;
    string email;
    int nbReservation = 0;
    Car* c;
};

struct RentedCar {
    int userId;
    string plateNumber;
    string startDate;
    string endDate;
};

void addUserToFile(const Client& user)
{
    cout << "addUserToFile" << endl;
    ofstream file("users.csv", ios::app);
    if (file.is_open()) {
        file << user.userID << "," << user.firstName << "," << user.lastName << ","
            << sha1(user.password) << "," << user.email << "," << user.phone << endl;
        file.close();
    }
    else {
        cout << "Unable to open file for writing user data." << endl;
    }
}
bool validname(const string& name)
{
    regex pattern("^[A-Za-z]+(?: [A-Za-z]+)*$");

    return regex_match(name, pattern); 
}
bool validatePassword(const string& password) {
    // Password should be at least 8 characters and contain at least one digit, one uppercase letter, and one special character
    return password.size() >= 8 &&
        any_of(password.begin(), password.end(), ::isdigit) &&
        any_of(password.begin(), password.end(), ::isupper) &&
        any_of(password.begin(), password.end(), [](char c) { return !isalnum(c); });
}

bool validateEmail(const string& email) {
    // Using a simple regex for email validation
    const regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
    return regex_match(email, pattern);
}

bool validatePhoneNumber(const string& phone) {
    // Using a simple regex for phone number validation (10 digits)
    const regex pattern("^(03|70|71|76|78|79|81)-\\d{6}$");
    return regex_match(phone, pattern);
}

void createUser(Client*& users, int& numUsers) {
    // Check if the pointer is null
    if (users == nullptr) {
        // Allocate memory for the first user
        users = new Client[1];
    }
    else {
        // Allocate memory for the new user by creating a temporary array
        Client* tempUsers = new Client[numUsers + 1];

        // Copy existing users to the temporary array
        for (int i = 0; i < numUsers; ++i) {
            tempUsers[i] = users[i];
        }

        // Assign the temporary array to the users pointer
        users = tempUsers;
    }

    // Create a new user object
    Client newUser;
    // validate the first and last name 
    while (true) {
        cout << "Enter your last name: ";
        getline(cin, newUser.lastName);
        if (validname(newUser.lastName))
        {
            break; 
        }
        else 
            cout << "name not valid . Try again." << endl;
    } 
    while (true) {
    cout << "Enter your first name: ";
    getline(cin,newUser.firstName);
    if (validname(newUser.firstName))
    {
        break;
    }
    else
        cout << "name not valid . Try again." << endl;
    }

    // Validate and set password
    while (true) {
        cout << "Enter your password (at least 8 characters containing numbers, letters, and special characters): ";
        getline(cin,newUser.password);
        if (validatePassword(newUser.password))
            break;
        else
            cout << "Password does not meet requirements. Try again." << endl;
    }

    // Validate and set email address
    while (true) {
        cout << "Enter your email address: ";
        getline(cin, newUser.email);
        if (validateEmail(newUser.email))
            break;
        else
            cout << "Invalid email address format. Try again." << endl;
    }

    // Validate and set phone number
    while (true) {
        cout << "Enter your phone number: ";
        getline(cin,newUser.phone);
        if (validatePhoneNumber(newUser.phone))
            break;
        else
            cout << "Invalid phone number format. Try again." << endl;
    }

    // Generate unique userID 
    newUser.userID = numUsers + 1;
    newUser.nbReservation = 0;
    newUser.c = nullptr;

    // Add the new user to the users array
    users[numUsers++] = newUser;
    addUserToFile(newUser);

    cout << "Account created successfully! Your userID is: " << newUser.userID << endl;
}
// check the password and ID if match 
bool authenticateUser(Client* users, int& userID, int numUsers) {
    string password;

    cout << "Enter your userID: ";
    cin >> userID;
    cin.ignore(); // Ignore the newline character left in the input buffer
    cout << "Enter your password: ";
    getline(cin, password);
    for (int i = 0; i < numUsers; i++) {
        if (users[i].userID == userID && users[i].password == sha1(password))
        {
            cout << "Authentication successful!" << endl;
            return true;
        }
    }

    cout << "Invalid username or password. Please try again." << endl;
    return false;
}


bool isValidDate(const string& date) {
    const regex pattern("^\\d{4}-\\d{2}-\\d{2}$");

    if (!regex_match(date, pattern)) {
        cout << "Invalid date format. Date must be in YYYY-MM-DD format." << endl;
        return false;
    }

    int year, month, day;

    // Extract year, month, and day from the date string
    year = stoi(date.substr(0, 4));
    month = stoi(date.substr(5, 2));
    day = stoi(date.substr(8, 2));

    // Validate year, month, and day
    if (year < 2024 || month < 1 || month > 12 || day < 1 || day > 31) {
        cout << "Invalid date. Please enter a valid date." << endl;
        return false;
    }

    // Additional validation ( February)
    if (month == 2 && day > 29) {
        cout << "Invalid date for February." << endl;
        return false;
    }

    return true;
}

void addCar(Car*& cars, int& numCars) {
    cout << "numcars : " << numCars << endl;
    if (cars == nullptr) {
        // Allocate memory for the first car
        cars = new Car[1];
    }
    else {
        // Allocate memory for the new car by creating a temporary array
        Car* tempCars = new Car[numCars + 1];

        // Copy existing cars to the temporary array
        for (int i = 0; i < numCars; ++i) {
            tempCars[i] = cars[i];
        }
        // Assign the temporary array to the cars pointer
        cars = tempCars;
    }

    // Create a new car object
    Car temp;
    cin.ignore();
    cout << "Enter car details:" << endl;
    cout << "Plate Number: ";
    getline(cin , temp.plateNumber);
    cout << "Brand: ";
    getline(cin, temp.brand);
    cout << "Model: ";
    getline(cin ,temp.model);
    cout << "Year: ";
    cin >> temp.year;
    cout << "Color: ";
    getline( cin, temp.color);
    cout << "Price per Day: ";
    cin >> temp.pricePerDay;

    // Append the new car to the cars array
    cars[numCars++] = temp;

    // Append the new car to the cars.txt file
    ofstream carFile("cars.csv", ios::app);
    if (carFile.is_open()) {
        carFile << temp.plateNumber << " ,"
            << temp.brand << ", "
            << temp.model << ", "
            << temp.year << " "
            << temp.color << ", "
            << temp.pricePerDay << endl;
        carFile.close();
    }
    else {
        cout << "Unable to open cars file for appending." << endl;
        return;
    }

    cout << "Car added successfully!" << endl;
}

void removeCar(Car* cars, int& numCars) {
    if (numCars > 0) {
        cout << "Available Cars:" << endl;
        for (int i = 0; i < numCars; ++i)
            cout << i + 1 << ". Plate Number: " << cars[i].plateNumber << ", Brand: " << cars[i].brand << ", Model: " <<
            cars[i].model << ", Price Per Day: " << cars[i].pricePerDay << endl;

        string plateNumber;
        cout << "Enter plate number of the car to remove: ";
        getline(cin, plateNumber);

        bool found = false;
        if (cars == nullptr)
            cars = new Car;
        for (int i = 0; i < numCars; ++i) {
            if (cars[i].plateNumber == plateNumber) {
                // Open cars.csv for writing
                ofstream carFile("cars.csv");
                if (carFile.is_open()) {
                    // Write all cars to the file except the one to be removed
                    for (int j = 0; j < numCars; ++j) {
                        if (j != i) {
                            carFile << cars[j].plateNumber << ", "
                                << cars[j].brand << ", "
                                << cars[j].model << ", "
                                << cars[j].year << " "
                                << cars[j].color << " ,"
                                << cars[j].pricePerDay << endl;
                        }
                    }
                    carFile.close();
                }
                else {
                    cout << "Unable to open cars file for writing." << endl;
                    return;
                }

                // Shift remaining cars to fill the gap
                for (int j = i; j < numCars - 1; ++j) {
                    cars[j] = cars[j + 1];
                }
                numCars--;
                found = true;
                cout << "Car removed successfully!" << endl;
                break;
            }
        }
        if (!found) {
            cout << "Car with plate number " << plateNumber << " not found." << endl;
        }
    }
    else {
        cout << "No cars available to remove." << endl;
    }
}

void modifyCar(Car* cars, int numCars) {
    if (numCars > 0) {
        cout << "Available Cars:" << endl;
        for (int i = 0; i < numCars; ++i)
            cout << i + 1 << ". Plate Number: " << cars[i].plateNumber << ", Brand: " << cars[i].brand << ", Model: " <<
            cars[i].model << ", Price Per Day: " << cars[i].pricePerDay << endl;

        string plateNumber;
        cout << "Enter plate number of the car to modify: ";
        cin >> plateNumber;

        bool found = false;
        if (cars == nullptr)
            cars = new Car;
        for (int i = 0; i < numCars; ++i) {
            if (cars[i].plateNumber == plateNumber) {
                cout << "Enter new details for car with plate number " << plateNumber << ":" << endl;
                cout << "Brand: ";
                getline(cin, cars[i].brand);
                cout << "Model: ";
                getline( cin,cars[i].model);
                cout << "Year: ";
                cin >> cars[i].year;
                cout << "Color: ";
                getline(cin, cars[i].color);
                cout << "Price per Day: ";
                cin >> cars[i].pricePerDay;

                // Open cars.csv for writing
                ofstream carFile("cars.csv");
                if (carFile.is_open()) {
                    // Write all cars to the file
                    for (int j = 0; j < numCars; ++j) {
                        carFile << cars[j].plateNumber << ","
                            << cars[j].brand << ","
                            << cars[j].model << ","
                            << cars[j].year << ","
                            << cars[j].color << ","
                            << cars[j].pricePerDay << endl;
                    }
                    carFile.close();
                }
                else {
                    cout << "Unable to open cars file for writing." << endl;
                    return;
                }

                found = true;
                cout << "Car modified successfully!" << endl;
                break;
            }
        }
        if (!found) {
            cout << "Car with plate number " << plateNumber << " not found." << endl;
        }
    }
    else {
        cout << "No cars available to modify." << endl;
    }
}


bool isCarAvailable(const string& plateNumber, const string& startDate, const string& endDate)
{
    ifstream rentedCarFile("rented_cars.csv");
    string line;

    if (rentedCarFile.is_open()) {
        while (getline(rentedCarFile, line)) {
            stringstream ss(line);
            RentedCar rentedCar;

            ss >> rentedCar.userId >> rentedCar.plateNumber
                >> rentedCar.startDate >> rentedCar.endDate;

            // Check if the plate numbers match
            if (rentedCar.plateNumber == plateNumber) {
                // Check if the rental period overlaps
                if (!(endDate < rentedCar.startDate || startDate > rentedCar.endDate)) {
                    rentedCarFile.close();
                    return false; // Car is not available
                }
            }
        }
        rentedCarFile.close();
        return true; // Car is available
    }
    else {
        cout << "Unable to open rented cars file." << endl;
        return false; // Error occurred
    }
}

bool isStartDateBeforeEndDate(const string& startDate, const string& endDate)
{
    // Create istringstream objects for parsing dates
    istringstream startStream(startDate);
    istringstream endStream(endDate);

    // Create tm structs to store parsed dates
    tm start_tm = {};
    tm end_tm = {};

    // Parse start date
    startStream >> get_time(&start_tm, "%Y-%m-%d");
    if (startStream.fail()) {
        cerr << "Error: Invalid start date format." << endl;
        return false;
    }

    // Parse end date
    endStream >> get_time(&end_tm, "%Y-%m-%d");
    if (endStream.fail()) {
        cerr << "Error: Invalid end date format." << endl;
        return false;
    }

    // Convert tm structs to time_t values
    time_t start_time = mktime(&start_tm);
    time_t end_time = mktime(&end_tm);

    // Check if start time is before end time
    return start_time < end_time;
}

int getPriceForPlate(Car* cars, int numCars, const string& plateNumber) {
    for (int i = 0; i < numCars; ++i) {
        if (cars[i].plateNumber == plateNumber) {
            return cars[i].pricePerDay;
        }
    }
    return -1; // Return -1 if plate number is not found
}

void rentCar(Client& user, Car* cars, int numCars) {
    string plateNumber, startDate, endDate;

    cout << "Available Cars:" << endl;
    for (int i = 0; i < numCars; ++i)
        cout << i + 1 << ". Plate Number: " << cars[i].plateNumber << ", Brand: " << cars[i].brand << ", Model: " <<
        cars[i].model << ", Price Per Day: " << cars[i].pricePerDay << endl;

    while (true) {
        cout << "Enter the plate number of the car you want to rent: ";
        cin >> plateNumber;

        bool found = false;
        for (int i = 0; i < numCars; ++i) {
            if (cars[i].plateNumber == plateNumber) {
                found = true;
                break;
            }
        }

        if (found) {
            break; // Break out of the loop if the plate number is found
        }
        else {
            cout << "Error: Car with plate number " << plateNumber << " not found. Please try again." << endl;
        }
    }

    while (true) {
        cout << "Enter the start date of the rental (YYYY-MM-DD): ";
        getline (cin, startDate);
        if (isValidDate(startDate))
            break;
        cout << "Invalid date format. Please enter the date in YYYY-MM-DD format." << endl;
    }

    while (true) {
        cout << "Enter the end date of the rental (YYYY-MM-DD): ";
        getline(cin, endDate);
        if (isValidDate(endDate))
            break;
        cout << "Invalid date format. Please enter the date in YYYY-MM-DD format." << endl;
    }

    if (!isCarAvailable(plateNumber, startDate, endDate)) {
        cout << "Error: The car is not available for the specified dates." << endl;
        return;
    }

    // Initialize selectedCar pointer
    Car* selectedCar = nullptr;

    // Iterate over the cars array to find the selected car
    for (int i = 0; i < numCars; ++i) {
        if (cars[i].plateNumber == plateNumber) {
            selectedCar = &cars[i];
            break;
        }
    }

    // Check if the selected car was found
    if (selectedCar == nullptr) {
        cout << "Error: Car not found." << endl;
        return;
    }

    // Dynamically allocate memory for a new car in user's reservation list
    Car* newCarReservation = new Car;
    newCarReservation->plateNumber = selectedCar->plateNumber;
    newCarReservation->brand = selectedCar->brand;
    newCarReservation->model = selectedCar->model;
    newCarReservation->year = selectedCar->year;
    newCarReservation->color = selectedCar->color;
    newCarReservation->pricePerDay = selectedCar->pricePerDay;
    newCarReservation->d.startDate = startDate;
    newCarReservation->d.endDate = endDate;

    // Find the correct position to insert the new reservation in the rented_cars.csv file
    ifstream inputFile("rented_cars.csv");
    ofstream tempFile("temp.csv");

    bool inserted = false;

    if (inputFile.is_open() && tempFile.is_open()) {
        string line;
        while (getline(inputFile, line)) {
            stringstream ss(line);
            string userId, plate, start, end;
            ss >> userId >> plate >> start >> end;
            int price = getPriceForPlate(cars, numCars, plate);
            if (newCarReservation->pricePerDay < price && !inserted) {
                tempFile << user.userID << "," << newCarReservation->plateNumber << "," << startDate << "," << endDate
                    << endl;
                inserted = true;
            }
            tempFile << line << endl;
        }

        if (!inserted) {
            tempFile << user.userID << "," << newCarReservation->plateNumber << "," << startDate << "," << endDate <<
                endl;
        }

        inputFile.close();
        tempFile.close();

        remove("rented_cars.csv");
        rename("temp.csv", "rented_cars.csv");
    }
    else {
        cout << "Unable to open rented cars file." << endl;
    }

    cout << "Car rented successfully!" << endl;
}


void adminActions(Car* cars, int& numCars) {
    int choice;
    cout << "Admin Actions:" << endl;
    cout << "1. Add a car" << endl;
    cout << "2. Remove a car" << endl;
    cout << "3. Modify a car" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice) {
    case 1:
        addCar(cars, numCars);
        break;
    case 2:
        removeCar(cars, numCars);
        break;
    case 3:
        modifyCar(cars, numCars);
        break;
        
    default:
        cout << "Invalid choice." << endl;
        break;
    }
}

void loadCarsFromFile(Car*& cars, int& numCars) {
    ifstream carFile("cars.csv");
    if (!carFile.is_open()) {
        cout << "Cars file not found. Creating new file." << endl;
        ofstream newCarFile("cars.csv");
        if (!newCarFile.is_open()) {
            cout << "Error creating cars file. Exiting program." << endl;
            return;
        }
        newCarFile.close();
    }
    else {
        while (true) {
            // Dynamically allocate memory for a new car
            Car* newCar = new Car;

            // Read car details from file
            if (!(carFile >> newCar->plateNumber
                >> newCar->brand
                >> newCar->model
                >> newCar->year
                >> newCar->color
                >> newCar->pricePerDay)) {
                // If unable to read, delete the dynamically allocated memory and break the loop
                delete newCar;
                break;
            }

            // Allocate memory for temporary array to hold previous cars plus the new one
            Car* newTempCars = new Car[numCars + 1];

            // Copy existing cars to temporary array
            for (int i = 0; i < numCars; ++i) {
                newTempCars[i] = cars[i];
            }

            newTempCars[numCars] = *newCar;
            cars = newTempCars;
            ++numCars;
        }

        carFile.close();
    }
}

void loadUsersFromFile(Client*& users, int& numUsers) {
    ifstream userFile("users.csv");

    // Load users from file into dynamically allocated array
    if (userFile.is_open()) {
        // Temporary array to hold loaded users
        Client* tempUsers = nullptr;
        int capacity = 0;

        // Load users from file
        Client tempUser;
        while (userFile >> tempUser.userID
            >> tempUser.firstName
            >> tempUser.lastName
            >> tempUser.password
            >> tempUser.email
            >> tempUser.phone) {
            // Check if more memory needs to be allocated
            if (numUsers >= capacity) {
                // Double the capacity if needed, or allocate initial memory
                int newCapacity = (capacity == 0) ? 1 : capacity * 2;
                Client* newTempUsers = new Client[newCapacity];

                // Copy existing users to the new temporary array
                for (int i = 0; i < numUsers; ++i) {
                    newTempUsers[i] = tempUsers[i];
                }

                tempUsers = newTempUsers;
                capacity = newCapacity;
            }
            tempUsers[numUsers++] = tempUser;
        }

        users = tempUsers;

        userFile.close();
    }
    else {
        cout << "Unable to open user file. Creating new file." << endl;
        ofstream newUserFile("users.csv");
        if (!newUserFile.is_open()) {
            cout << "Error creating users file. Exiting program." << endl;
        }
        newUserFile.close();
    }
}

void loadRentedCarsFromFile(Client*& users, int numUsers) {
    ifstream rentedCarFile("rented_cars.csv");
    if (rentedCarFile.is_open()) {
        int userID;
        string plateNumber, startDate, endDate;
        while (rentedCarFile >> userID >> plateNumber >> startDate >> endDate) {
            // Find the user with the matching userID
            for (int i = 0; i < numUsers; ++i) {
                if (users[i].userID == userID) {
                    // Dynamically allocate memory for the new rented car
                    Car* newRentedCar = new Car;
                    newRentedCar->plateNumber = plateNumber;
                    newRentedCar->d.startDate = startDate;
                    newRentedCar->d.endDate = endDate;

                    Car* newTempRentedCars = new Car[users[i].nbReservation + 1];

                    for (int j = 0; j < users[i].nbReservation; ++j) {
                        newTempRentedCars[j] = users[i].c[j];
                    }
                    newTempRentedCars[users[i].nbReservation] = *newRentedCar;

                    users[i].c = newTempRentedCars;
                    users[i].nbReservation++;

                    break;
                }
            }
        }
        rentedCarFile.close();
    }
    else {
        cout << "Unable to open rented cars file." << endl;
    }
}

void cancelReservation(int userID, Client* users, int numUsers, int& numRentals) {
    // Find the user based on userID
    Client* user = nullptr;
    for (int i = 0; i < numUsers; ++i) {
        if (users[i].userID == userID) {
            user = &users[i];
            break;
        }
    }

    if (user == nullptr) {
        cout << "Error: User not found." << endl;
        return;
    }

    // Display user's reservations
    cout << "Your reservations:" << endl;
    for (int i = 0; i < user->nbReservation; ++i) {
        const Car& car = user->c[i];
        cout << "Plate Number: " << car.plateNumber << " | Start Date: " << car.d.startDate << " | End Date: " << car.d.
            endDate << endl;
    }

    // Ask user to select a reservation to cancel
    cout << "Enter the plate number of the car you want to cancel reservation for: ";
    string plateNumber;
    getline(cin, plateNumber);

    // Find the car based on plate number
    int carIndex = -1;
    for (int i = 0; i < user->nbReservation; ++i) {
        if (user->c[i].plateNumber == plateNumber) {
            carIndex = i;
            break;
        }
    }

    if (carIndex == -1) {
        cout << "Error: Car not found in your reservations." << endl;
        return;
    }

    // Remove the reservation
    for (int i = carIndex; i < user->nbReservation - 1; ++i) {
        user->c[i] = user->c[i + 1];
    }
    user->nbReservation--;

    // Update rented cars file by rewriting the remaining reservations
    ofstream rentedCarFile("rented_cars.csv");
    if (rentedCarFile.is_open()) {
        for (int i = 0; i < numUsers; ++i) {
            for (int j = 0; j < users[i].nbReservation; ++j) {
                if (users[i].userID != userID || users[i].c[j].plateNumber != plateNumber) {
                    rentedCarFile << users[i].userID << " " << users[i].c[j].plateNumber << " " << users[i].c[j].d.
                        startDate << " " << users[i].c[j].d.endDate << endl;
                }
            }
        }
        rentedCarFile.close();
    }
    else {
        cout << "Unable to open rented cars file for rewriting." << endl;
    }

    cout << "Reservation canceled successfully!" << endl;
}

void modifyReservation(int userID, Car* cars, int numCars, Client* users, int numUsers, int& numRentals) {
    // Find the user based on userID
    Client* user = nullptr;
    for (int i = 0; i < numUsers; ++i) {
        if (users[i].userID == userID) {
            user = &users[i];
            break;
        }
    }

    if (user == nullptr) {
        cout << "Error: User not found." << endl;
        return;
    }

    // Display user's reservations
    cout << "Your reservations:" << endl;
    for (int i = 0; i < user->nbReservation; ++i) {
        const Car& car = user->c[i];
        cout << "Plate Number: " << car.plateNumber << " | Start Date: " << car.d.startDate << " | End Date: " << car.d.
            endDate << endl;
    }

    // Ask user to select a reservation to modify
    cout << "Enter the plate number of the car you want to modify reservation for: ";
    string plateNumber;
    cin >> plateNumber;

    // Find the car based on plate number
    int carIndex = -1;
    for (int i = 0; i < user->nbReservation; ++i) {
        if (user->c[i].plateNumber == plateNumber) {
            carIndex = i;
            break;
        }
    }

    if (carIndex == -1) {
        cout << "Error: Car not found in your reservations." << endl;
        return;
    }

    while (true) {
        cout << "Enter the new start date of the reservation (YYYY-MM-DD): ";
        cin >> user->c[carIndex].d.startDate;
        if (isValidDate(user->c[carIndex].d.startDate))
            break;
        cout << "Invalid date format. Please enter the date in YYYY-MM-DD format." << endl;
    }

    while (true) {
        cout << "Enter the new end date of the reservation (YYYY-MM-DD): ";
        cin >> user->c[carIndex].d.endDate;
        if (isValidDate(user->c[carIndex].d.endDate))
            break;
        cout << "Invalid date format. Please enter the date in YYYY-MM-DD format." << endl;
    }

    // Update rented_cars.txt file
    ifstream inputFile("rented_cars.csv");
    ofstream tempFile("temp.csv");

    if (inputFile.is_open() && tempFile.is_open()) {
        string line;
        while (getline(inputFile, line)) {
            stringstream ss(line);
            string userId, plate, start, end;
            ss >> userId >> plate >> start >> end;
            if (userId == to_string(userID) && plate == plateNumber) {
                // Modify the reservation in the file
                tempFile << userID << " " << plateNumber << " " << user->c[carIndex].d.startDate << " " << user->c[
                    carIndex].d.endDate << endl;
            }
            else {
                // Copy the unchanged reservation to the temp file
                tempFile << line << endl;
            }
        }
        inputFile.close();
        tempFile.close();

        // Replace the original file with the temp file
        remove("rented_cars.csv");
        rename("temp.csv", "rented_cars.csv");

        cout << "Reservation modified successfully!" << endl;
    }
    else {
        cout << "Unable to open rented cars file." << endl;
    }
}

void clientActions(Client& user, Car* cars, int numCars, Client* users, int numUsers, int& numRentals) {
    int choice;
    cout << "Client Actions:" << endl;
    cout << "1. Rent a car" << endl;
    cout << "2. Cancel a reservation" << endl;
    cout << "3. Modify a reservation" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice) {
    case 1:
        rentCar(user, cars, numCars);
        break;
    case 2:
        cancelReservation(user.userID, users, numUsers, numRentals);
        break;
    case 3:
        modifyReservation(user.userID, cars, numCars, users, numUsers, numRentals);
        break;
        
    default:
        cout << "Invalid choice." << endl;
        break;
    }
}
void writeClientInfoToPDF(const Client& client) {
    fstream pdf;
    pdf.open("client_info.pdf", ios::out);
    if (!pdf.is_open()) {
        cout << "Error opening the PDF file." << endl;
        return;
    }

    // Write client information to PDF
    pdf << "User ID: " << client.userID << endl;
    pdf << "Name: " << client.firstName << " " << client.lastName << endl;
    pdf << "Phone: " << client.phone << endl;
    pdf << "Email: " << client.email << endl;
    pdf << "Number of Reservations: " << client.nbReservation << endl << endl;

    if (client.nbReservation > 0 && client.c != nullptr) {
        pdf << "Reservations Sorted by Price (Ascending):" << endl;
        for (int i = 0; i < client.nbReservation; i++) {
            pdf << "Reservation " << (i + 1) << ":" << endl;
            pdf << "Plate Number: " << client.c[i].plateNumber << endl;
            pdf << "Price: " << client.c[i].pricePerDay << endl;
            pdf << "Start Date: " << client.c[i].d.startDate << endl;
            pdf << "End Date: " << client.c[i].d.endDate << endl << endl;
        }
    }

    pdf.close();
    cout << "Client information written to client_info.pdf" << endl;
}


int main() {
    Client* users = nullptr;
    Car* cars = nullptr;
    int numUsers = 0;
    int numCars = 0;
    int numRentals = 0;
    int userID = 0; // Initialize userID to avoid undefined behavior

    loadUsersFromFile(users, numUsers);
    loadCarsFromFile(cars, numCars);
    loadRentedCarsFromFile(users, numUsers);

    char choice;
    do {
        cout << "Do you have an account? (y/n): ";
        cin >> choice;
    } while (choice != 'Y' && choice != 'y' && choice != 'n' && choice != 'N');


    if (choice == 'n' || choice == 'N') {
        createUser(users, numUsers);
    }
    else 
    {
        bool authenticated = false;
        while (!authenticated) {
            authenticated = authenticateUser(users, userID, numUsers);
        }
    }

    // Continue with the rest of the program based on user type (admin or client)
    while (true)
    {
        if (userID == 1)
 {
            // Admin actions
            adminActions(cars, numCars);
        }
        else
        {
            // Client actions
            clientActions(users[userID - 1], cars, numCars, users, numUsers, numRentals);
        }
        do {
            cout << "Do you want to perform another action? (y/n): " << endl;

            cin >> choice;
        } while (choice != 'Y' && choice != 'y' && choice != 'n' && choice != 'N'); 
        if (choice == 'n' || choice == 'N') 
        {
            break;
        }
    }

    // Cleanup: deallocate memory
    delete[] users;
    delete[] cars;

    return 0;
}

	
