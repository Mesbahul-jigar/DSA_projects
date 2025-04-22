#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
using namespace std;

#define MAX_ROOMS 50
#define MAX_EMPLOYEES 50
#define MAX_NAME 50

class Room {
public:
    int roomNumber;
    int isAvailable;
    char guestName[MAX_NAME];
    int daysBooked;
    float ratePerDay;
    int isBooked;
    int otp;
    char type[MAX_NAME];
    char startDate[MAX_NAME];
    int numberOfGuests;

    Room() {
        roomNumber = 0;
        isAvailable = 1;
        isBooked = 0;
        otp = 0; // Set to 0 initially
        daysBooked = 0;
        numberOfGuests = 0;
        strcpy(guestName, "");
        strcpy(startDate, "");
        strcpy(type, "");
        ratePerDay = 0.0;
    }
};

class Employee {
public:
    int id;
    char name[MAX_NAME];
    char role[MAX_NAME];
    float salary;

    Employee() {
        id = 0;
        strcpy(name, "");
        strcpy(role, "");
        salary = 0.0;
    }
};

Room rooms[MAX_ROOMS];
Employee employees[MAX_EMPLOYEES];
int employeeCount = 0;
char adminUsername[MAX_NAME] = "admin";
char adminPassword[MAX_NAME] = "password";
float totalIncome = 0;
float checkedInIncome = 0;
float canceledIncome = 0;

void initializeRooms() {
    for (int i = 0; i < MAX_ROOMS; i++) {
        rooms[i].roomNumber = i + 1;
        rooms[i].isAvailable = 1;
        rooms[i].isBooked = 0;
        if (i < 10) {
            strcpy(rooms[i].type, "Suite");
            rooms[i].ratePerDay = 1000.0;
        } else if (i < 30) {
            strcpy(rooms[i].type, "King");
            rooms[i].ratePerDay = 500.0;
        } else {
            strcpy(rooms[i].type, "Travel");
            rooms[i].ratePerDay = 250.0;
        }
    }
}

int login(int panel) {
    char username[MAX_NAME], password[MAX_NAME];
    cout << (panel == 1 ? "\nAdmin Login\n" : "\nUser Login\n");
    cout << "Username: "; cin >> username;
    cout << "Password: "; cin >> password;
    if (panel == 1 && strcmp(username, adminUsername) == 0 && strcmp(password, adminPassword) == 0) {
        return 1;
    } else if (panel == 2) {
        return 1;
    } else {
        cout << "\nIncorrect username or password!\n";
        return 0;
    }
}

void viewRoomDetails() {
    for (int i = 0; i < MAX_ROOMS; i++) {
        cout << "Room Number: " << rooms[i].roomNumber << ", Type: " << rooms[i].type << ", Rate: $" << rooms[i].ratePerDay
             << ", Status: " << (rooms[i].isAvailable ? "Available" : "Booked") << endl;
    }
}

void bookRoom() {
    int roomNumber, days, numberOfGuests;
    char guestName[MAX_NAME], startDate[MAX_NAME];
    cout << "\nEnter Room Number to Book: ";
    cin >> roomNumber;
    if (cin.fail() || roomNumber < 1 || roomNumber > MAX_ROOMS || rooms[roomNumber - 1].isBooked) {
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cout << "Invalid room number or room already booked.\n";
        return;
    }
    cout << "Enter Guest Name: ";
    cin >> guestName;
    cout << "Enter Number of Days: ";
    cin >> days;
    cout << "Enter Start Date: ";
    cin >> startDate;
    cout << "Enter Number of Guests: ";
    cin >> numberOfGuests;
    rooms[roomNumber - 1].isBooked = 1;
    strcpy(rooms[roomNumber - 1].guestName, guestName);
    rooms[roomNumber - 1].daysBooked = days;
    strcpy(rooms[roomNumber - 1].startDate, startDate);
    rooms[roomNumber - 1].numberOfGuests = numberOfGuests;
    cout << "\nRoom booked successfully for " << guestName << " from " << startDate 
         << "! Total Bill: $" << rooms[roomNumber - 1].ratePerDay * days << endl;
}

void confirmBooking() {
    int roomNumber;
    cout << "\nEnter Room Number to Confirm: ";
    cin >> roomNumber;
    if (roomNumber < 1 || roomNumber > MAX_ROOMS || !rooms[roomNumber - 1].isBooked) {
        cout << "No booking exists.\n";
        return;
    }
    rooms[roomNumber - 1].isAvailable = 0;
    rooms[roomNumber - 1].otp = rand() % 9000 + 1000; // Generate random OTP
    cout << "\nBooking confirmed! OTP for check-in: " << rooms[roomNumber - 1].otp << endl;
}

void checkIn() {
    int roomNumber, enteredOtp;
    cout << "\nEnter Room Number for Check-In: ";
    cin >> roomNumber;
    if (roomNumber < 1 || roomNumber > MAX_ROOMS || rooms[roomNumber - 1].isAvailable || rooms[roomNumber - 1].otp == 0) {
        cout << "Room is not confirmed or OTP not generated.\n";
        return;
    }
    cout << "Enter OTP: ";
    cin >> enteredOtp;
    if (enteredOtp == rooms[roomNumber - 1].otp) {
        rooms[roomNumber - 1].isAvailable = 0;
        rooms[roomNumber - 1].otp = 0;
        cout << "\nChecked in successfully!\n";
    } else {
        cout << "\nIncorrect OTP.\n";
    }
}

void checkOut() {
    int roomNumber;
    cout << "\nEnter Room Number for Check-Out: ";
    cin >> roomNumber;
    if (roomNumber < 1 || roomNumber > MAX_ROOMS || rooms[roomNumber - 1].isAvailable) {
        cout << "Room is already vacant.\n";
        return;
    }
    float bill = rooms[roomNumber - 1].daysBooked * rooms[roomNumber - 1].ratePerDay;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    if (tm.tm_mon == 11 || tm.tm_mday == 25) {
        bill *= 0.9;
    }
    checkedInIncome += bill;
    totalIncome += bill;

    cout << "\nGuest Name: " << rooms[roomNumber - 1].guestName << "\nTotal Bill: $" << bill << endl;
    rooms[roomNumber - 1].isAvailable = 1;
    rooms[roomNumber - 1].isBooked = 0;
    strcpy(rooms[roomNumber - 1].guestName, "");
    rooms[roomNumber - 1].daysBooked = 0;
    cout << "Checked out successfully!\n";
}

void cancelBooking() {
    int roomNumber;
    cout << "\nEnter Room Number to Cancel Booking: ";
    cin >> roomNumber;
    if (roomNumber < 1 || roomNumber > MAX_ROOMS || !rooms[roomNumber - 1].isBooked) {
        cout << "No booking exists.\n";
        return;
    }
    float refund = rooms[roomNumber - 1].ratePerDay * 0.9 * rooms[roomNumber - 1].daysBooked;
    float bill = rooms[roomNumber - 1].daysBooked * rooms[roomNumber - 1].ratePerDay;
    canceledIncome += (bill - refund);

    cout << "\nBooking canceled for " << rooms[roomNumber - 1].guestName << ".\nRefund: $" << refund << endl;
    rooms[roomNumber - 1].isBooked = 0;
    rooms[roomNumber - 1].isAvailable = 1;
    strcpy(rooms[roomNumber - 1].guestName, "");
    rooms[roomNumber - 1].daysBooked = 0;
}

void addEmployee() {
    if (employeeCount >= MAX_EMPLOYEES) {
        cout << "\nEmployee limit reached!\n";
        return;
    }
    cout << "\nEnter Employee ID: ";
    cin >> employees[employeeCount].id;
    cout << "Enter Employee Name: ";
    cin >> employees[employeeCount].name;
    cout << "Enter Employee Role: ";
    cin >> employees[employeeCount].role;
    cout << "Enter Employee Salary: ";
    cin >> employees[employeeCount].salary;
    employeeCount++;
    cout << "\nEmployee added successfully!\n";
}

void showIncome() {
    cout << "\nIncome Report:\n";
    cout << "1. Checked-in Income: $" << checkedInIncome << endl;
    cout << "2. Canceled Booking Income: $" << canceledIncome << endl;
    cout << "3. Total Income: $" << totalIncome << endl;
}

void listEmployees() {
    cout << "\nEmployee List:\n";
    for (int i = 0; i < employeeCount; i++) {
        cout << "ID: " << employees[i].id << ", Name: " << employees[i].name << ", Role: " << employees[i].role
             << ", Salary: $" << employees[i].salary << endl;
    }
}

int main() {
    srand(time(NULL));
    initializeRooms();
    int panel, choice;
    while (true) {
        cout << "Select Panel:\n1. Admin\n2. User\n3. Exit\nEnter choice: ";
        cin >> panel;
        if (panel == 3) {
            cout << "Exiting system. Goodbye!\n";
            exit(0);
        }
        if (!login(panel)) {
            continue;
        }
        while (true) {
            cout << "\nMenu:\n";
            if (panel == 1) {
                cout << "1. View Room Details\n2. Confirm Booking\n3. Show Income\n4. Add Employee\n5. List Employees\n6. Switch Panel\n";
            } else {
                cout << "1. View Available Rooms\n2. Book Room\n3. Check-In\n4. Check-Out\n5. Cancel Booking\n6. Switch Panel\n";
            }
            cout << "Enter your choice: ";
            cin >> choice;
            if (panel == 1) {
                switch (choice) {
                    case 1: viewRoomDetails(); break;
                    case 2: confirmBooking(); break;
                    case 3: showIncome(); break;
                    case 4: addEmployee(); break;
                    case 5: listEmployees(); break;
                    case 6: break;
                    default: cout << "Invalid choice. Please try again.\n";
                }
            } else {
                switch (choice) {
                    case 1: viewRoomDetails(); break;
                    case 2: bookRoom(); break;
                    case 3: checkIn(); break;
                    case 4: checkOut(); break;
                    case 5: cancelBooking(); break;
                    case 6: break;
                    default: cout << "Invalid choice. Please try again.\n";
                }
            }
            if (choice == 6) {
                break;
            }
        }
    }
    return 0;
}