#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <ctime>

using namespace std;

// ================= FOOD ITEM CLASS =================

class FoodItem {
public:
    string name;
    int quantity;
    int day, month, year;
    string donor;

    FoodItem() {}

    FoodItem(string n, int q, int d, int m, int y, string dn) {
        name = n;
        quantity = q;
        day = d;
        month = m;
        year = y;
        donor = dn;
    }
};

// ================= PRIORITY QUEUE COMPARATOR =================

class CompareExpiry {
public:
    bool operator()(FoodItem a, FoodItem b) {

        if (a.year != b.year)
            return a.year > b.year;

        if (a.month != b.month)
            return a.month > b.month;

        return a.day > b.day;
    }
};

// ================= GLOBAL VARIABLES =================

vector<FoodItem> inventory;

priority_queue<FoodItem,
               vector<FoodItem>,
               CompareExpiry> expiryQueue;

// ================= GET CURRENT DATE =================

void getCurrentDate(int &d, int &m, int &y) {

    time_t now = time(0);

    tm *ltm = localtime(&now);

    d = ltm->tm_mday;
    m = 1 + ltm->tm_mon;
    y = 1900 + ltm->tm_year;
}

// ================= DATE CONVERSION =================

int convertToDays(int d, int m, int y) {

    return y * 365 + m * 30 + d;
}

// ================= REMAINING DAYS =================

int getRemainingDays(FoodItem item) {

    int cd, cm, cy;

    getCurrentDate(cd, cm, cy);

    int currentDays = convertToDays(cd, cm, cy);

    int expiryDays = convertToDays(item.day,
                                   item.month,
                                   item.year);

    return expiryDays - currentDays;
}

// ================= ADD FOOD =================

void addFood() {

    string name, donor;
    int quantity, d, m, y;

    cin.ignore();

    cout << "\nEnter Food Name: ";
    getline(cin, name);

    cout << "Enter Quantity: ";
    cin >> quantity;

    cout << "Enter Expiry Date (DD MM YYYY): ";
    cin >> d >> m >> y;

    cin.ignore();

    cout << "Enter Donor Name: ";
    getline(cin, donor);

    FoodItem item(name,
                  quantity,
                  d,
                  m,
                  y,
                  donor);

    inventory.push_back(item);

    expiryQueue.push(item);

    cout << "\nFood Added Successfully!\n";
}

// ================= DISPLAY INVENTORY =================

void displayInventory() {

    if (inventory.empty()) {

        cout << "\nInventory is Empty!\n";
        return;
    }

    cout << "\n=========== FOOD INVENTORY ===========\n";

    for (FoodItem item : inventory) {

        int remaining = getRemainingDays(item);

        cout << "\nFood Name : " << item.name;

        cout << "\nQuantity  : " << item.quantity;

        cout << "\nExpiry    : "
             << item.day << "/"
             << item.month << "/"
             << item.year;

        cout << "\nDonor     : " << item.donor;

        if (remaining < 0) {

            cout << "\nStatus    : EXPIRED";
        }

        else if (remaining == 0) {

            cout << "\nStatus    : Expires TODAY";
        }

        else if (remaining <= 3) {

            cout << "\nStatus    : Expiring Soon";
        }

        else {

            cout << "\nStatus    : Safe";
        }

        cout << "\nRemaining : "
             << remaining
             << " day(s)\n";
    }
}

// ================= GENERATE EXPIRY ALERTS =================

void generateExpiryAlerts() {

    if (expiryQueue.empty()) {

        cout << "\nNo Food Items Available!\n";
        return;
    }

    priority_queue<FoodItem,
                   vector<FoodItem>,
                   CompareExpiry> tempQueue = expiryQueue;

    cout << "\n=========== EXPIRY ALERTS ===========\n";

    bool found = false;

    while (!tempQueue.empty()) {

        FoodItem item = tempQueue.top();

        tempQueue.pop();

        int remaining = getRemainingDays(item);

        if (remaining < 0) {

            cout << "\n[EXPIRED] "
                 << item.name
                 << " expired "
                 << abs(remaining)
                 << " day(s) ago.";

            found = true;
        }

        else if (remaining == 0) {

            cout << "\n[URGENT] "
                 << item.name
                 << " expires TODAY.";

            found = true;
        }

        else if (remaining <= 3) {

            cout << "\n[WARNING] "
                 << item.name
                 << " expires in "
                 << remaining
                 << " day(s).";

            found = true;
        }
    }

    if (!found) {

        cout << "\nNo expiry alerts.\n";
    }
}

// ================= REMOVE EXPIRED ITEMS =================

void removeExpiredItems() {

    vector<FoodItem> updatedInventory;

    while (!expiryQueue.empty())
        expiryQueue.pop();

    for (FoodItem item : inventory) {

        int remaining = getRemainingDays(item);

        if (remaining >= 0) {

            updatedInventory.push_back(item);

            expiryQueue.push(item);
        }
    }

    inventory = updatedInventory;

    cout << "\nExpired Items Removed Successfully!\n";
}

// ================= SEARCH FOOD =================

void searchFood() {

    cin.ignore();

    string searchName;

    cout << "\nEnter Food Name to Search: ";

    getline(cin, searchName);

    bool found = false;

    for (FoodItem item : inventory) {

        if (item.name == searchName) {

            found = true;

            cout << "\nFood Found!\n";

            cout << "Quantity : "
                 << item.quantity;

            cout << "\nExpiry : "
                 << item.day << "/"
                 << item.month << "/"
                 << item.year;

            cout << "\nRemaining Days : "
                 << getRemainingDays(item)
                 << endl;
        }
    }

    if (!found) {

        cout << "\nFood Item Not Found!\n";
    }
}

// ================= SAVE DATA =================

void saveToFile() {

    ofstream file("inventory.txt");

    for (FoodItem item : inventory) {

        file << item.name << ","
             << item.quantity << ","
             << item.day << ","
             << item.month << ","
             << item.year << ","
             << item.donor << endl;
    }

    file.close();

    cout << "\nData Saved Successfully!\n";
}

// ================= LOAD DATA =================

void loadFromFile() {

    ifstream file("inventory.txt");

    if (!file)
        return;

    inventory.clear();

    while (!file.eof()) {

        string name, donor;
        int quantity, d, m, y;

        getline(file, name, ',');

        if (name == "")
            break;

        file >> quantity;
        file.ignore();

        file >> d;
        file.ignore();

        file >> m;
        file.ignore();

        file >> y;
        file.ignore();

        getline(file, donor);

        FoodItem item(name,
                      quantity,
                      d,
                      m,
                      y,
                      donor);

        inventory.push_back(item);

        expiryQueue.push(item);
    }

    file.close();
}

// ================= MENU =================

void menu() {

    int choice;

    do {

        cout << "\n\n=========== FOOD BANK INVENTORY SYSTEM ===========\n";

        cout << "1. Add Food Donation\n";
        cout << "2. Display Inventory\n";
        cout << "3. Generate Expiry Alerts\n";
        cout << "4. Search Food Item\n";
        cout << "5. Remove Expired Items\n";
        cout << "6. Save Data\n";
        cout << "7. Exit\n";

        cout << "\nEnter Choice: ";

        cin >> choice;

        switch (choice) {

        case 1:
            addFood();
            break;

        case 2:
            displayInventory();
            break;

        case 3:
            generateExpiryAlerts();
            break;

        case 4:
            searchFood();
            break;

        case 5:
            removeExpiredItems();
            break;

        case 6:
            saveToFile();
            break;

        case 7:
            saveToFile();
            cout << "\nExiting Program...\n";
            break;

        default:
            cout << "\nInvalid Choice!\n";
        }

    } while (choice != 7);
}

// ================= MAIN FUNCTION =================

int main() {

    loadFromFile();

    menu();

    return 0;
}