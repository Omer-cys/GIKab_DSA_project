#include <iostream>
#include <fstream>
#include <string>
using namespace std;




/* =====================================================================
   SECTION 1: USER MANAGEMENT SYSTEM (Created on 13/10/2025)
   =====================================================================
   DSA Concepts:
   - Arrays + Pointers
   - Linear Search, Binary Search, Insertion Sort
   - File I/O 
===================================================================== */




struct User {
    int id;
    string name;
    bool isDriver;
};

const int MAX_USERS = 100;
User* users[MAX_USERS];
int userCount = 0;
string fileName = "users.txt";

/* ---------------- Function Prototypes ---------------- */
void addUser(string name, bool isDriver);
void viewUsers();
User* linearSearch(int id);
void insertionSort();
User* binarySearch(int id);
void saveToFile();
void loadFromFile();

/* ---------------- Function Definitions ---------------- */

// Add new user (and auto-save)
void addUser(string name, bool isDriver) {
    if (userCount >= MAX_USERS) {
        cout << "User limit reached!\n";
        return;
    }

    User* newUser = new User;
    newUser->id = 100 + userCount;
    newUser->name = name;
    newUser->isDriver = isDriver;

    users[userCount++] = newUser;
    cout << "User added successfully! (ID: " << newUser->id << ")\n";

    saveToFile();
}

// View all users
void viewUsers() {
    cout << "\n--- All Users ---\n";
    for (int i = 0; i < userCount; i++) {
        cout << "ID: " << users[i]->id
             << " | Name: " << users[i]->name
             << " | Role: " << (users[i]->isDriver ? "Driver" : "Rider") << "\n";
    }
    if (userCount == 0) cout << "No users found.\n";
}

// Linear Search
User* linearSearch(int id) {
    for (int i = 0; i < userCount; i++)
        if (users[i]->id == id)
            return users[i];
    return nullptr;
}

// Sort by ID (Insertion Sort)
void insertionSort() {
    for (int i = 1; i < userCount; i++) {
        User* key = users[i];
        int j = i - 1;
        while (j >= 0 && users[j]->id > key->id) {
            users[j + 1] = users[j];
            j--;
        }
        users[j + 1] = key;
    }
}

// Binary Search (on sorted list)
User* binarySearch(int id) {
    int left = 0, right = userCount - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        if (users[mid]->id == id)
            return users[mid];
        else if (users[mid]->id < id)
            left = mid + 1;
        else
            right = mid - 1;
    }
    return nullptr;
}

// Save all user data to file
void saveToFile() {
    ofstream file(fileName.c_str()); 
    if (!file) {
        cout << "Error saving to file.\n";
        return;
    }

    for (int i = 0; i < userCount; i++) {
        file << users[i]->id << ","
             << users[i]->name << ","
             << (users[i]->isDriver ? "Driver" : "Rider") << "\n";
    }
    file.close();
}

// Load user data from file (manual parsing, no sstream)
void loadFromFile() {
    ifstream file(fileName.c_str());
    if (!file) {
        cout << "No existing data found. Starting fresh.\n";
        return;
    }

    string line;
    while (getline(file, line)) {
        int firstComma = line.find(',');
        int secondComma = line.find(',', firstComma + 1);

        if (firstComma == string::npos || secondComma == string::npos)
            continue; // skip malformed lines

        string idStr = line.substr(0, firstComma);
        string name = line.substr(firstComma + 1, secondComma - firstComma - 1);
        string role = line.substr(secondComma + 1);

        User* u = new User;
        u->id = stoi(idStr); 
        u->name = name;
        u->isDriver = (role == "Driver");

        users[userCount++] = u;
    }
    file.close();
    cout << "Loaded " << userCount << " users from file.\n";
}





/* =====================================================================
   SECTION 2: RIDE HISTORY (Linked Lists) (Created on 16/10/2025)
   =====================================================================
   DSA Concepts:
   - Singly Linked List (Ride History)
   - Doubly Linked List (Forward & Backward Traversal)
   - Circular Linked List (Active Ride Loop)
===================================================================== */





struct Ride {
    int rideId;
    string pickup, dropoff;
    double fare;
    Ride* next;
    Ride* prev;
};

struct RideList {
    Ride* head;
    Ride* tail;
    RideList() { head = tail = nullptr; }
};

RideList rideHistory[MAX_USERS];

// Add a new ride (Doubly Linked List)
void addRide(int riderId, string pickup, string dropoff, double fare) {
    int index = riderId - 100;
    if (index < 0 || index >= userCount) {
        cout << "Invalid rider ID!\n";
        return;
    }

    Ride* newRide = new Ride;
    newRide->rideId = 1000 + rand() % 9000;
    newRide->pickup = pickup;
    newRide->dropoff = dropoff;
    newRide->fare = fare;
    newRide->next = nullptr;
    newRide->prev = nullptr;

    RideList& list = rideHistory[index];
    if (!list.head) {
        list.head = list.tail = newRide;
    } else {
        list.tail->next = newRide;
        newRide->prev = list.tail;
        list.tail = newRide;
    }

    cout << "Ride added (Ride ID: " << newRide->rideId << ")\n";
}

// Display ride history forward
void showRidesForward(int riderId) {
    int index = riderId - 100;
    RideList& list = rideHistory[index];

    if (!list.head) {
        cout << "No rides found.\n";
        return;
    }

    cout << "\n--- Ride History (Forward) ---\n";
    for (Ride* cur = list.head; cur; cur = cur->next)
        cout << "Ride " << cur->rideId << ": " << cur->pickup
             << " -> " << cur->dropoff << " | Fare: " << cur->fare << "\n";
}

// Display ride history backward
void showRidesBackward(int riderId) {
    int index = riderId - 100;
    RideList& list = rideHistory[index];

    if (!list.tail) {
        cout << "No rides found.\n";
        return;
    }

    cout << "\n--- Ride History (Backward) ---\n";
    for (Ride* cur = list.tail; cur; cur = cur->prev)
        cout << "Ride " << cur->rideId << ": " << cur->dropoff
             << " <- " << cur->pickup << " | Fare: " << cur->fare << "\n";
}

// Circular ride traversal
void showCircularRides(int riderId) {
    int index = riderId - 100;
    RideList& list = rideHistory[index];

    if (!list.head) {
        cout << "No rides found.\n";
        return;
    }

    list.tail->next = list.head;
    cout << "\n--- Circular Ride Traversal ---\n";
    Ride* cur = list.head;
    do {
        cout << "Ride " << cur->rideId << " | " << cur->pickup << " -> " << cur->dropoff << "\n";
        cur = cur->next;
    } while (cur != list.head);
    list.tail->next = nullptr;
}




/* =====================================================================
   SECTION 3: RIDE BOOKING QUEUE (Created on 19/10/2025)
   =====================================================================
   DSA Concepts:
   - Queue using Linked List
   - Enqueue (Add Ride Request)
   - Dequeue (Assign Ride)
===================================================================== */





struct RideRequest {
    int riderId;
    string pickup;
    string dropoff;
    RideRequest* next;
};

RideRequest* frontReq = nullptr;
RideRequest* rearReq = nullptr;

// Enqueue (Add new ride request)
void enqueueRideRequest(int riderId, string pickup, string dropoff) {
    RideRequest* newReq = new RideRequest{riderId, pickup, dropoff, nullptr};

    if (rearReq == nullptr) {
        frontReq = rearReq = newReq;
    } else {
        rearReq->next = newReq;
        rearReq = newReq;
    }

    cout << "Ride Request Added (Rider ID: " << riderId << ")\n";
}

// Dequeue (Assign ride)
void dequeueRideRequest() {
    if (frontReq == nullptr) {
        cout << "No pending ride requests.\n";
        return;
    }

    RideRequest* req = frontReq;
    frontReq = frontReq->next;
    if (frontReq == nullptr) rearReq = nullptr;

    cout << "Ride Assigned: Rider " << req->riderId
         << " | " << req->pickup << " -> " << req->dropoff << "\n";

    delete req;
}

// Show pending requests
void showPendingRequests() {
    cout << "\n--- Pending Ride Requests (FIFO) ---\n";
    if (!frontReq) {
        cout << "No pending requests.\n";
        return;
    }

    RideRequest* cur = frontReq;
    while (cur) {
        cout << "Rider " << cur->riderId << " | " << cur->pickup
             << " -> " << cur->dropoff << "\n";
        cur = cur->next;
    }
}




/* =====================================================================
   MAIN PROGRAM (Created on 19/10/2025)
===================================================================== */




int main() {
    srand(time(nullptr));
    loadFromFile();

    int choice;
    do {
        cout << "\n========== CABGORITHM MENU ==========\n";
        cout << "1. Add User\n";
        cout << "2. View All Users\n";
        cout << "3. Search User (Linear Search)\n";
        cout << "4. Sort + Binary Search\n";
        cout << "5. Add Ride (History)\n";
        cout << "6. Show Ride History (Forward)\n";
        cout << "7. Show Ride History (Backward)\n";
        cout << "8. Show Ride History (Circular)\n";
        cout << "9. Add Ride Request (Enqueue)\n";
        cout << "10. Assign Next Ride (Dequeue)\n";
        cout << "11. View Pending Ride Requests\n";
        cout << "0. Exit\n";
        cout << "Choose option: ";
        cin >> choice;

        if (choice == 1) {
            string name;
            int role;
            cout << "Enter name: ";
            cin >> ws;
            getline(cin, name);
            cout << "Role (1 = Driver, 0 = Rider): ";
            cin >> role;
            addUser(name, role == 1);
        }
        else if (choice == 2) viewUsers();
        else if (choice == 3) {
            int id; cout << "Enter ID: "; cin >> id;
            User* u = linearSearch(id);
            if (u) cout << "Found: " << u->name << " (" << (u->isDriver ? "Driver" : "Rider") << ")\n";
            else cout << "User not found.\n";
        }
        else if (choice == 4) {
            insertionSort();
            int id; cout << "Enter ID: "; cin >> id;
            User* u = binarySearch(id);
            if (u) cout << "Found: " << u->name << "\n";
            else cout << "User not found.\n";
        }
        else if (choice == 5) {
            int id; string pickup, dropoff; double fare;
            cout << "Enter Rider ID: "; cin >> id;
            cout << "Pickup: "; cin >> ws; getline(cin, pickup);
            cout << "Dropoff: "; getline(cin, dropoff);
            cout << "Fare: "; cin >> fare;
            addRide(id, pickup, dropoff, fare);
        }
        else if (choice == 6) {
            int id; cout << "Enter Rider ID: "; cin >> id;
            showRidesForward(id);
        }
        else if (choice == 7) {
            int id; cout << "Enter Rider ID: "; cin >> id;
            showRidesBackward(id);
        }
        else if (choice == 8) {
            int id; cout << "Enter Rider ID: "; cin >> id;
            showCircularRides(id);
        }
        else if (choice == 9) {
            int id; string pickup, dropoff;
            cout << "Enter Rider ID: "; cin >> id;
            cout << "Pickup: "; cin >> ws; getline(cin, pickup);
            cout << "Dropoff: "; getline(cin, dropoff);
            enqueueRideRequest(id, pickup, dropoff);
        }
        else if (choice == 10) dequeueRideRequest();
        else if (choice == 11) showPendingRequests();

    } while (choice != 0);

    cout << "Goodbye!\n";
    return 0;
}
