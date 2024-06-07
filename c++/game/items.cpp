#include <unordered_map>
#include <string>
#include <vector>

using namespace std;

class Item {
public:

    virtual ~Item() {}

    string name;
    int id = -1;
    int damage = 0;
    int block = 0;
    double weight = 0.0;

    Item() : id(-1), name(""), weight(0.0), damage(0), block(0) {}

    Item(int i, const std::string& n, double w, int d = 0, int b = 0)
        : id(i), name(n), weight(w), damage(d), block(b) {}
};

#define TOTAL_ITEMS 24

std::unordered_map<int, Item> items;

void initializeItems() {
    // Tier One
    items[0] = {0, "Wood Sword", 0.09, 3, 0};
    items[1] = {1, "Wood Shield", 0.13, 0, 2};
    items[2] = {2, "Stone Sword", 0.12, 4, 0};
    items[3] = {3, "Stone Shield", 0.20, 0, 3};
    items[4] = {4, "Iron Sword", 0.18, 6, 0};
    items[5] = {5, "Iron Shield", 0.25, 0, 5};
    items[6] = {6, "Tin Sword", 0.10, 4, 0};
    items[7] = {7, "Tin Shield", 0.14, 0, 3};
    items[8] = {8, "Tungsten Sword", 0.22, 8, 0};
    items[9] = {9, "Tungsten Shield", 0.30, 0, 7};
    items[10] = {10, "Copper Sword", 0.16, 5, 0};
    items[11] = {11, "Copper Shield", 0.22, 0, 4};
    // Tier Two
    items[12] = {12, "Double Edged Wood Sword", 0.11, 5, 0};
    items[13] = {13, "Reinforced Wood Shield", 0.15, 0, 4};
    items[14] = {14, "Double Edged Stone Sword", 0.14, 6, 0};
    items[15] = {15, "Reinforced Stone Shield", 0.22, 0, 5};
    items[16] = {16, "Double Edged Iron Sword", 0.20, 8, 0};
    items[17] = {17, "Reinforced Iron Shield", 0.27, 0, 7};
    items[18] = {18, "Double Edged Tin Sword", 0.12, 5, 0};
    items[19] = {19, "Reinforced Tin Shield", 0.16, 0, 4};
    items[20] = {20, "Double Edged Tungsten Sword", 0.26, 10, 0};
    items[21] = {21, "Reinforced Tungsten Shield", 0.34, 0, 9};
    items[22] = {22, "Double Edged Copper Sword", 0.19, 7, 0};
    items[23] = {23, "Reinforced Copper Shield", 0.26, 0, 6};
    // Armor
    items[999] = {999, "Nothing", 0.0, 0, 0};

    vector<Item> itemsVector;
    for (const auto& pair : items) {
        itemsVector.push_back(pair.second);
    }
}
