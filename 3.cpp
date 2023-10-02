#include <iostream>
#include <ctime>
#include <vector>
#include <list>
#include <string>
using namespace std;

class FoodItem {
protected:
    string name;
    string useBy;
    string frozenOn;
    string category;

public:
    FoodItem(const string& name, const string& useBy, const string& category)
        : name(name), useBy(useBy), category(category) {
        time_t currentTime = time(nullptr);
        tm* currentDate = localtime(&currentTime);
        int currentYear = currentDate->tm_year + 1900;
        int currentMonth = currentDate->tm_mon + 1;
        int currentDay = currentDate->tm_mday;
        frozenOn = to_string(currentYear) + "-" + to_string(currentMonth) + "-" + to_string(currentDay);
    }

    string getName() const {
        return name;
    }

    string getUseBy() const {
        return useBy;
    }

    string getFrozenOn() const {
        return frozenOn;
    }

    string getCategory() const {
        return category;
    }

    virtual void displayDetails() const = 0;
};

class FridgeItem : public FoodItem {
private:
    int fridgeNumber;
    int shelfNumber;

public:
    FridgeItem(const string& name, const string& useBy, const string& category, int fridgeNumber, int shelfNumber)
        : FoodItem(name, useBy, category), fridgeNumber(fridgeNumber), shelfNumber(shelfNumber) {}

    int getFridgeNumber() const {
        return fridgeNumber;
    }

    int getShelfNumber() const {
        return shelfNumber;
    }

    void displayDetails() const override {
        cout << "Name: " << name << endl;
        cout << "Use By: " << useBy << endl;
        cout << "Frozen On: " << frozenOn << endl;
        cout << "Category: " << category << endl;
        cout << "Fridge Number: " << fridgeNumber << endl;
        cout << "Shelf Number: " << shelfNumber << endl;
    }
};

class FreezerItem : public FoodItem {
private:
    string position;

public:
    FreezerItem(const string& name, const string& useBy, const string& category, const string& position)
        : FoodItem(name, useBy, category), position(position) {}

    string getPosition() const {
        return position;
    }

    void displayDetails() const override {
        cout << "Name: " << name << endl;
        cout << "Use By: " << useBy << endl;
        cout << "Frozen On: " << frozenOn << endl;
        cout << "Category: " << category << endl;
        cout << "Position: " << position << endl;
    }
};

class InventoryManager {
private:
    vector<FoodItem*> inventory;

public:
    void addItem(FoodItem* item) {
        inventory.push_back(item);
    }

    void removeItem(const string& itemName) {
        for (auto it = inventory.begin(); it != inventory.end(); ++it) {
            if ((*it)->getName() == itemName) {
                delete *it;
                inventory.erase(it);
                break;
            }
        }
    }

    void displayOldestItem() const {
        if (inventory.empty()) {
            cout << "Inventory is empty." << endl;
            return;
        }

        time_t currentTime = time(nullptr);
        tm* currentDate = localtime(&currentTime);
        int currentYear = currentDate->tm_year + 1900;
        int currentMonth = currentDate->tm_mon + 1;
        int currentDay = currentDate->tm_mday;

        FoodItem* oldestItem = nullptr;
        string oldestDate = "9999-99-99";

        for (const auto& item : inventory) {
            string useBy = item->getUseBy();
            int year, month, day;
            sscanf(useBy.c_str(), "%d-%d-%d", &year, &month, &day);

            if (year < currentYear || (year == currentYear && month < currentMonth) ||
                (year == currentYear && month == currentMonth && day < currentDay)) {
                if (useBy < oldestDate) {
                    oldestItem = item;
                    oldestDate = useBy;
                }
            }
        }

        if (oldestItem != nullptr) {
            cout << "Oldest Item Details:" << endl;
            oldestItem->displayDetails();
            cout << " " << endl;
        } else {
            cout << "No expired items found." << endl;
        }
    }

    void displayExpiredItemNames() const {
        if (inventory.empty()) {
            cout << "Inventory is empty." << endl;
            return;
        }

        cout << "Expired Item Names:" << endl;
        bool expiredItemsFound = false;

        for (const auto& item : inventory) {
            string useBy = item->getUseBy();
            time_t currentTime = time(nullptr);
            tm* currentDate = localtime(&currentTime);
            int currentYear = currentDate->tm_year + 1900;
            int currentMonth = currentDate->tm_mon + 1;
            int currentDay = currentDate->tm_mday;

            int year, month, day;
            sscanf(useBy.c_str(), "%d-%d-%d", &year, &month, &day);

            if (year < currentYear || (year == currentYear && month < currentMonth) ||
                (year == currentYear && month == currentMonth && day < currentDay)) {
                cout << item->getName() << endl;
                expiredItemsFound = true;
            }
        }

        if (!expiredItemsFound) {
            cout << "No expired items found." << endl;
        }
    }

    bool isShelfFull(int fridgeNumber, int shelfNumber) const {
        int count = 0;
        for (const auto& item : inventory) {
            FridgeItem* fridgeItem = dynamic_cast<FridgeItem*>(item);
            if (fridgeItem != nullptr && fridgeItem->getFridgeNumber() == fridgeNumber && fridgeItem->getShelfNumber() == shelfNumber) {
                count++;
                if (count >= 2) {
                    return true;
                }
            }
        }
        return false;
    }

    ~InventoryManager() {
        for (auto item : inventory) {
            delete item;
        }
    }
};

// Function to validate the date format
bool isValidDate(const string& date) {
    if (date.length() != 10)
        return false;

    int year, month, day;
    if (sscanf(date.c_str(), "%d-%d-%d", &year, &month, &day) != 3)
        return false;

    if (year < 0 || month < 1 || month > 12 || day < 1 || day > 31)
        return false;

    return true;
}

// Function to get a valid date from the user
string getValidDate() {
    string date;
    cout << "Enter the item use by date (YYYY-MM-DD): ";
    cin >> date;

    while (!isValidDate(date)) {
        cout << "Invalid date format. Please enter the date in the format (YYYY-MM-DD): ";
        cin >> date;
    }

    return date;
}


bool isInteger(const string& input) {
    if (input.empty())
        return false;

    for (char c : input) {
        if (!isdigit(c))
            return false;
    }

    return true;
}

int stringToInt(const string& input) {
    int result = 0;
    int sign = 1;
    size_t i = 0;

    if (input[0] == '-') {
        sign = -1;
        i = 1;
    }

    for (; i < input.length(); ++i) {
        result = result * 10 + (input[i] - '0');
    }

    return result * sign;
}

int main() {
    InventoryManager inventoryManager;

    string choiceStr;
    string itemName, itemUseBy, itemCategory;
    int fridgeNumber, shelfNumber;

    while (true) {
        cout << "----- Food Inventory Manager -----" << endl;
        cout << "1. Add Item" << endl;
        cout << "2. Remove Item" << endl;
        cout << "3. Display Oldest Item" << endl;
        cout << "4. Show All Expired Item Names" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";

        cin >> choiceStr;

        if (!isInteger(choiceStr)) {
            cout << "Invalid choice. Please try again." << endl;
            continue;
        }

        int choice = stringToInt(choiceStr);
        
        switch (choice) {
            case 1:
                cout << "Enter the item name: ";
                cin >> itemName;
                itemUseBy = getValidDate();

                cout << "Select the item category:" << endl;
                cout << "1. Vegetable" << endl;
                cout << "2. Fruit" << endl;
                cout << "3. Milk" << endl;
                cout << "4. Meat" << endl;
                cout << "Enter the category number: ";
                int categoryNumber;
                cin >> categoryNumber;

                switch (categoryNumber) {
                    case 1:
                        itemCategory = "vegetable";
                        break;
                    case 2:
                        itemCategory = "fruit";
                        break;
                    case 3:
                        itemCategory = "milk";
                        break;
                    case 4:
                        itemCategory = "meat";
                        break;
                    default:
                        cout << "Invalid category number. Item not added." << endl;
                        break;
                }

                cout << "Enter the fridge number: ";
                cin >> fridgeNumber;
                cout << "Enter the shelf number: ";
                cin >> shelfNumber;

                if (inventoryManager.isShelfFull(fridgeNumber, shelfNumber)) {
                    cout << "Shelf has been full. Try to add the item to another shelf or another fridge. You can also try removing an item from this shelf" << endl;
                } else {
                    inventoryManager.addItem(new FridgeItem(itemName, itemUseBy, itemCategory, fridgeNumber, shelfNumber));
                }
                break;
            case 2:
                cout << "Enter the item name to remove: ";
                cin >> itemName;

                inventoryManager.removeItem(itemName);
                break;
            case 3:
                inventoryManager.displayOldestItem();
                break;
            case 4:
                inventoryManager.displayExpiredItemNames();
                break;
            case 5:
                cout << "Exiting the program..." << endl;
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }

        cout << endl;
    }
}