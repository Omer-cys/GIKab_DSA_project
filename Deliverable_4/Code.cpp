#include <iostream>
#include <fstream>
#include <string>
using namespace std;

/* ===================== SECTION 1: USER MANAGEMENT ===================== */

struct User
{
    int id;
    string name;
    bool isDriver;
    double rating; // most recent rating
};

const int MAX_USERS = 100;
User *users[MAX_USERS];
int userCount = 0;
string fileName = "users.txt";

/* ---------------- Function Prototypes ---------------- */
void addUser(string name, bool isDriver);
void viewUsers();
User *linearSearch(int id);
void insertionSort();
User *binarySearch(int id);
void saveToFile();
void loadFromFile();

/* ---------------- Function Definitions ---------------- */

// Add new user
void addUser(string name, bool isDriver)
{
    if (userCount >= MAX_USERS)
    {
        cout << "User limit reached!" << endl;
        return;
    }

    User *newUser = new User;
    newUser->id = 100 + userCount;
    newUser->name = name;
    newUser->isDriver = isDriver;
    newUser->rating = 0;

    users[userCount] = newUser;
    userCount = userCount + 1;

    cout << "User added successfully! ID: " << newUser->id << endl;
    saveToFile();
}

// View all users
void viewUsers()
{
    if (userCount == 0)
    {
        cout << "No users found." << endl;
        return;
    }

    cout << "--- All Users ---" << endl;
    for (int i = 0; i < userCount; i = i + 1)
    {
        cout << "ID: " << users[i]->id
             << " | Name: " << users[i]->name
             << " | Role: ";
        if (users[i]->isDriver == true)
        {
            cout << "Driver";
        }
        else
        {
            cout << "Rider";
        }
        cout << " | Rating: " << users[i]->rating << endl;
    }
}

// Linear Search
User *linearSearch(int id)
{
    for (int i = 0; i < userCount; i = i + 1)
    {
        if (users[i]->id == id)
        {
            return users[i];
        }
    }
    return NULL;
}

// Insertion Sort by ID
void insertionSort()
{
    for (int i = 1; i < userCount; i = i + 1)
    {
        User *key = users[i];
        int j = i - 1;
        while (j >= 0 && users[j]->id > key->id)
        {
            users[j + 1] = users[j];
            j = j - 1;
        }
        users[j + 1] = key;
    }
}

// Binary Search
User *binarySearch(int id)
{
    int left = 0;
    int right = userCount - 1;
    while (left <= right)
    {
        int mid = (left + right) / 2;
        if (users[mid]->id == id)
        {
            return users[mid];
        }
        if (users[mid]->id < id)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }
    return NULL;
}

// Save to file
void saveToFile()
{
    ofstream file(fileName.c_str());
    if (!file)
    {
        cout << "Error saving to file." << endl;
        return;
    }

    for (int i = 0; i < userCount; i = i + 1)
    {
        file << users[i]->id << "," << users[i]->name << ",";
        if (users[i]->isDriver == true)
        {
            file << "Driver";
        }
        else
        {
            file << "Rider";
        }
        file << "," << users[i]->rating << endl;
    }
    file.close();
}

// Load from file
void loadFromFile()
{
    ifstream file(fileName.c_str());
    if (!file)
    {
        cout << "No existing data found." << endl;
        return;
    }

    string line;
    while (getline(file, line))
    {
        int firstComma = line.find(",");
        int secondComma = line.find(",", firstComma + 1);
        int thirdComma = line.find(",", secondComma + 1);

        if (firstComma == -1 || secondComma == -1 || thirdComma == -1)
        {
            continue;
        }

        string idStr = line.substr(0, firstComma);
        string nameStr = line.substr(firstComma + 1, secondComma - firstComma - 1);
        string roleStr = line.substr(secondComma + 1, thirdComma - secondComma - 1);
        string ratingStr = line.substr(thirdComma + 1);

        User *u = new User;
        u->id = atoi(idStr.c_str());
        u->name = nameStr;
        if (roleStr == "Driver")
        {
            u->isDriver = true;
        }
        else
        {
            u->isDriver = false;
        }
        u->rating = atof(ratingStr.c_str());

        users[userCount] = u;
        userCount = userCount + 1;
    }

    file.close();
    cout << "Loaded " << userCount << " users from file." << endl;
}

/* ===================== SECTION 2: RIDE HISTORY (NOW A CLASS) ===================== */

struct Ride
{
    int rideId;
    string pickup;
    string dropoff;
    double fare;
    Ride *next;
    Ride *prev;
};

struct RideList
{
    Ride *head;
    Ride *tail;
};

class RideHistoryLinkedList
{
public:
    RideList rideHistory[MAX_USERS];

    RideHistoryLinkedList()
    {
        for (int i = 0; i < MAX_USERS; i++)
        {
            rideHistory[i].head = NULL;
            rideHistory[i].tail = NULL;
        }
    }

    void addRideNodeToLinkedList(int riderId, string pickup, string dropoff, double fare)
    {
        int index = riderId - 100;
        if (index < 0 || index >= userCount)
        {
            cout << "Invalid Rider ID." << endl;
            return;
        }

        Ride *newRide = new Ride;
        newRide->rideId = 1000 + index; // simple ID without rand
        newRide->pickup = pickup;
        newRide->dropoff = dropoff;
        newRide->fare = fare;
        newRide->next = NULL;
        newRide->prev = NULL;

        if (rideHistory[index].head == NULL)
        {
            rideHistory[index].head = newRide;
            rideHistory[index].tail = newRide;
        }
        else
        {
            rideHistory[index].tail->next = newRide;
            newRide->prev = rideHistory[index].tail;
            rideHistory[index].tail = newRide;
        }

        cout << "Ride added. Ride ID: " << newRide->rideId << endl;

        // Ask for driver rating
        cout << "Enter Driver ID for this ride: ";
        int driverId;
        cin >> driverId;

        User *driver = linearSearch(driverId);
        if (driver != NULL && driver->isDriver == true)
        {
            double rating;
            cout << "Enter rating (0-5): ";
            cin >> rating;
            driver->rating = rating;
            cout << "Driver rating updated." << endl;
        }
        else
        {
            cout << "Invalid driver ID. Rating skipped." << endl;
        }
    }

    void traverseForwardLinkedList(int riderId)
    {
        int index = riderId - 100;
        if (index < 0 || index >= userCount || rideHistory[index].head == NULL)
        {
            cout << "No rides found." << endl;
            return;
        }

        Ride *current = rideHistory[index].head;
        while (current != NULL)
        {
            cout << "Ride " << current->rideId << ": " << current->pickup
                 << " -> " << current->dropoff << " | Fare: " << current->fare << endl;
            current = current->next;
        }
    }

    void traverseBackwardLinkedList(int riderId)
    {
        int index = riderId - 100;
        if (index < 0 || index >= userCount || rideHistory[index].tail == NULL)
        {
            cout << "No rides found." << endl;
            return;
        }

        Ride *current = rideHistory[index].tail;
        while (current != NULL)
        {
            cout << "Ride " << current->rideId << ": " << current->dropoff
                 << " <- " << current->pickup << " | Fare: " << current->fare << endl;
            current = current->prev;
        }
    }

    void traverseCircularLinkedListDemo(int riderId)
    {
        int index = riderId - 100;
        if (index < 0 || index >= userCount || rideHistory[index].head == NULL)
        {
            cout << "No rides found." << endl;
            return;
        }

        Ride *start = rideHistory[index].head;
        Ride *current = start;
        cout << "--- Circular Ride List ---" << endl;
        while (current != NULL)
        {
            cout << "Ride " << current->rideId << ": " << current->pickup
                 << " -> " << current->dropoff << " | Fare: " << current->fare << endl;
            current = current->next;
            if (current == NULL)
                current = start; // repeat once
            break;               // avoid infinite loop for simplicity
        }
    }
};

/* ===================== SECTION 3: ===================== */

/* ===================== MERGE SORT FOR RIDES ===================== */

Ride *mergeRideLists(Ride *a, Ride *b)
{
    if (a == NULL)
    {
        return b;
    }
    if (b == NULL)
    {
        return a;
    }

    Ride *result;
    if (a->fare <= b->fare)
    {
        result = a;
        result->next = mergeRideLists(a->next, b);
    }
    else
    {
        result = b;
        result->next = mergeRideLists(a, b->next);
    }
    return result;
}

void splitRideList(Ride *initial_unsplitted_linkedlist, Ride **front_half_pointer, Ride **back_half_pointer)
{
    if (initial_unsplitted_linkedlist == NULL || initial_unsplitted_linkedlist->next == NULL)
    {
        *front_half_pointer = initial_unsplitted_linkedlist;
        *back_half_pointer = NULL;
        return;
    }

    Ride *slow = initial_unsplitted_linkedlist;
    Ride *fast = initial_unsplitted_linkedlist->next;
    while (fast != NULL)
    {
        fast = fast->next;
        if (fast != NULL)
        {
            slow = slow->next;
            fast = fast->next;
        }
    }

    *front_half_pointer = initial_unsplitted_linkedlist;
    *back_half_pointer = slow->next;
    slow->next = NULL;
}

Ride *mergeSortRides(Ride *head)
{
    if (head == NULL || head->next == NULL)
        return head;

    Ride *a;
    Ride *b;
    splitRideList(head, &a, &b);

    a = mergeSortRides(a);
    b = mergeSortRides(b);

    return mergeRideLists(a, b);
}

void showRidesSortedByFare(RideList &rideList)
{
    if (rideList.head == NULL)
    {
        cout << "No rides found." << endl;
        return;
    }

    rideList.head = mergeSortRides(rideList.head);

    // reset tail
    Ride *cur = rideList.head;
    rideList.tail = NULL;
    while (cur != NULL)
    {
        if (cur->next == NULL)
        {
            rideList.tail = cur;
        }
        cur = cur->next;
    }

    cout << "--- Rides Sorted by Fare ---" << endl;
    cur = rideList.head;
    while (cur != NULL)
    {
        cout << "Ride " << cur->rideId << ": " << cur->pickup << " -> " << cur->dropoff << " | Fare: " << cur->fare << endl;
        cur = cur->next;
    }
}

/* ===================== RIDE REQUEST QUEUE ===================== */

struct RideRequest
{
    int riderId;
    string pickup;
    string dropoff;
    RideRequest *next;
};

RideRequest *frontReq = NULL;
RideRequest *rearReq = NULL;

void enqueueRideRequest(int riderId, string pickup, string dropoff)
{
    RideRequest *newReq = new RideRequest;
    newReq->riderId = riderId;
    newReq->pickup = pickup;
    newReq->dropoff = dropoff;
    newReq->next = NULL;

    if (rearReq == NULL)
    {
        frontReq = newReq;
        rearReq = newReq;
    }
    else
    {
        rearReq->next = newReq;
        rearReq = newReq;
    }
    cout << "Ride request added." << endl;
}

void dequeueRideRequest()
{
    if (frontReq == NULL)
    {
        cout << "No pending ride requests." << endl;
        return;
    }

    RideRequest *temp = frontReq;
    frontReq = frontReq->next;
    if (frontReq == NULL)
    {
        rearReq = NULL;
    }

    // Ask for Driver assignment
    cout << "Assigning ride for Rider " << temp->riderId << endl;
    int driverId;
    cout << "Enter Driver ID to assign: ";
    cin >> driverId;

    User *driver = linearSearch(driverId);
    if (driver != NULL && driver->isDriver)
    {
        cout << "Ride assigned to Driver " << driver->name << " (ID: " << driver->id << ") for Rider " << temp->riderId << endl;

        // Ask for rating immediately
        double rating;
        cout << "Enter rating for this driver (0-5): ";
        cin >> rating;
        driver->rating = rating;
        cout << "Driver rating updated." << endl;
    }
    else
    {
        cout << "Invalid driver ID. Ride not assigned." << endl;
    }

    delete temp;
}

void showPendingRequests()
{
    if (frontReq == NULL)
    {
        cout << "No pending requests." << endl;
        return;
    }
    RideRequest *cur = frontReq;
    while (cur != NULL)
    {
        cout << "Rider " << cur->riderId << ": " << cur->pickup
             << " -> " << cur->dropoff << endl;
        cur = cur->next;
    }
}

/* ===================== DRIVER RATING BST ===================== */

struct DriverNode
{
    User *driver;
    DriverNode *left;
    DriverNode *right;
};

DriverNode *ratingRoot = NULL;

DriverNode *insertDriverNode(DriverNode *root, User *driver)
{
    if (root == NULL)
    {
        DriverNode *newNode = new DriverNode;
        newNode->driver = driver;
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
    }
    if (driver->rating < root->driver->rating)
    {
        root->left = insertDriverNode(root->left, driver);
    }
    else
    {
        root->right = insertDriverNode(root->right, driver);
    }
    return root;
}

void inorderDriverTraversal(DriverNode *root)
{
    if (root == NULL)
    {
        return;
    }
    inorderDriverTraversal(root->left);
    cout << root->driver->name << " (ID: " << root->driver->id << ") | Rating: " << root->driver->rating << endl;
    inorderDriverTraversal(root->right);
}

void showDriversByRating()
{
    if (ratingRoot == NULL)
    {
        cout << "No drivers have ratings yet." << endl;
        return;
    }
    inorderDriverTraversal(ratingRoot);
}

/* ===================== MAIN PROGRAM ===================== */

RideHistoryLinkedList rideManager;

int main()
{
    loadFromFile();

    int choice = -1;
    while (choice != 0)
    {
        cout << "===== CAB MANAGEMENT SYSTEM =====" << endl;
        cout << "1. Add User (Create a new Driver or Rider)" << endl;
        cout << "2. View All Users (See all users with ID, role, and rating)" << endl;
        cout << "3. Search User by ID (Find a user quickly)" << endl;
        cout << "4. Sort Users by ID + Search (Faster search after sorting)" << endl;
        cout << "5. Add Ride for a Rider (Record pickup, dropoff, fare, and rate Driver)" << endl;
        cout << "6. Show Rider's Rides (Oldest to newest)" << endl;
        cout << "7. Show Rider's Rides (Newest to oldest)" << endl;
        cout << "8. Show First Ride in Circular Form (Demo of ride circular list)" << endl;
        cout << "9. Add Ride Request (Put a ride into waiting queue)" << endl;
        cout << "10. Assign Next Ride Request (Remove first request from queue)" << endl;
        cout << "11. View Pending Ride Requests (See waiting rides)" << endl;
        cout << "12. Show Drivers Sorted by Rating (Best-rated first)" << endl;
        cout << "13. Show Rider's Rides Sorted by Fare (Cheapest to most expensive)" << endl;
        cout << "0. Exit Program" << endl;
        cout << "Choose an option (0-13): ";
        cin >> choice;

        if (choice == 1)
        {
            string name;
            int role;
            cout << "Enter Name: ";
            cin >> name;
            cout << "Role (1=Driver, 0=Rider): ";
            cin >> role;
            addUser(name, role == 1);
        }
        else if (choice == 2)
            viewUsers();
        else if (choice == 3)
        {
            int id;
            cout << "Enter User ID to search: ";
            cin >> id;
            User *u = linearSearch(id);
            if (u != NULL)
                cout << "Found: " << u->name << " (Role: "
                     << (u->isDriver ? "Driver" : "Rider") << ")" << endl;
            else
                cout << "User not found." << endl;
        }
        else if (choice == 4)
        {
            insertionSort();
            int id;
            cout << "Enter User ID to search after sorting: ";
            cin >> id;
            User *u = binarySearch(id);
            if (u != NULL)
                cout << "Found: " << u->name << " (Role: "
                     << (u->isDriver ? "Driver" : "Rider") << ")" << endl;
            else
                cout << "User not found." << endl;
        }
        else if (choice == 5)
        {
            int id;
            string pickup, dropoff;
            double fare;
            cout << "Enter Rider ID: ";
            cin >> id;
            cout << "Pickup Location: ";
            cin >> pickup;
            cout << "Dropoff Location: ";
            cin >> dropoff;
            cout << "Fare Amount: ";
            cin >> fare;
            rideManager.addRideNodeToLinkedList(id, pickup, dropoff, fare);
        }
        else if (choice == 6)
        {
            int id;
            cout << "Enter Rider ID: ";
            cin >> id;
            rideManager.traverseForwardLinkedList(id);
        }
        else if (choice == 7)
        {
            int id;
            cout << "Enter Rider ID: ";
            cin >> id;
            rideManager.traverseBackwardLinkedList(id);
        }
        else if (choice == 8)
        {
            int id;
            cout << "Enter Rider ID: ";
            cin >> id;
            rideManager.traverseCircularLinkedListDemo(id);
        }
        else if (choice == 9)
        {
            int id;
            string pickup, dropoff;
            cout << "Enter Rider ID: ";
            cin >> id;
            cout << "Pickup Location: ";
            cin >> pickup;
            cout << "Dropoff Location: ";
            cin >> dropoff;
            enqueueRideRequest(id, pickup, dropoff);
        }
        else if (choice == 10)
        {
            dequeueRideRequest();
        }
        else if (choice == 11)
        {
            showPendingRequests();
        }
        else if (choice == 12)
        {
            ratingRoot = NULL;
            for (int i = 0; i < userCount; i = i + 1)
            {
                if (users[i]->isDriver == true)
                {
                    ratingRoot = insertDriverNode(ratingRoot, users[i]);
                }
            }
            showDriversByRating();
        }
        else if (choice == 13)
        {
            int id;
            cout << "Enter Rider ID: ";
            cin >> id;

            int index = id - 100;
            if (index < 0 || index >= userCount)
            {
                cout << "Invalid Rider ID." << endl;
            }
            else
            {
                showRidesSortedByFare(rideManager.rideHistory[index]);
            }
        }
        else if (choice == 0)
        {
            cout << "Exiting program. Goodbye!" << endl;
        }
        else
        {
            cout << "Invalid option. Please enter a number between 0 and 13." << endl;
        }
        cout << endl;
    
    }
}