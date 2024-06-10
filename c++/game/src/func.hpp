#include <string>
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cstring>
#include <cmath>
#include <unordered_map>
#include <iostream>
#include <variant>

#include "struct.hpp"

vector<int> getItemIds(const std::vector<Item>& vec) {
    std::vector<int> existingIds;
    for (const auto& item : vec) {
        if (item.id != 0) { // Assuming 0 means the element doesn't exist
            existingIds.push_back(item.id);
        }
    }
    return existingIds;
}

bool contains(const string& str, string term) {
    return str.find(term) != string::npos;
}

unordered_map<int, Item> items;

std::vector<Item> initializeItems() {
    std::vector<Item> itemsVector;

    // Tier One Sword and Shields
    itemsVector.push_back({0, "Wood Sword", 0.09, 3, 0});
    itemsVector.push_back({1, "Wood Shield", 0.13, 0, 2});
    itemsVector.push_back({2, "Stone Sword", 0.12, 4, 0});
    itemsVector.push_back({3, "Stone Shield", 0.20, 0, 3});
    itemsVector.push_back({4, "Iron Sword", 0.18, 6, 0});
    itemsVector.push_back({5, "Iron Shield", 0.25, 0, 5});
    itemsVector.push_back({6, "Tin Sword", 0.10, 4, 0});
    itemsVector.push_back({7, "Tin Shield", 0.14, 0, 3});
    itemsVector.push_back({8, "Tungsten Sword", 0.22, 8, 0});
    itemsVector.push_back({9, "Tungsten Shield", 0.30, 0, 7});
    itemsVector.push_back({10, "Copper Sword", 0.16, 5, 0});
    itemsVector.push_back({11, "Copper Shield", 0.22, 0, 4});

    // Tier Two Swords and Shields
    itemsVector.push_back({12, "Double Edged Wood Sword", 0.11, 5, 0});
    itemsVector.push_back({13, "Reinforced Wood Shield", 0.15, 0, 4});
    itemsVector.push_back({14, "Double Edged Stone Sword", 0.14, 6, 0});
    itemsVector.push_back({15, "Reinforced Stone Shield", 0.22, 0, 5});
    itemsVector.push_back({16, "Double Edged Iron Sword", 0.20, 8, 0});
    itemsVector.push_back({17, "Reinforced Iron Shield", 0.27, 0, 7});
    itemsVector.push_back({18, "Double Edged Tin Sword", 0.12, 5, 0});
    itemsVector.push_back({19, "Reinforced Tin Shield", 0.16, 0, 4});
    itemsVector.push_back({20, "Double Edged Tungsten Sword", 0.26, 10, 0});
    itemsVector.push_back({21, "Reinforced Tungsten Shield", 0.34, 0, 9});
    itemsVector.push_back({22, "Double Edged Copper Sword", 0.19, 7, 0});
    itemsVector.push_back({23, "Reinforced Copper Shield", 0.26, 0, 6});

    // Armor
    itemsVector.push_back({100, "Wooden Greaves", 0.14, 0, 3});
    itemsVector.push_back({101, "Wooden Chestplate", 0.20, 0, 4});
    itemsVector.push_back({102, "Stone Greaves", 0.18, 0, 4});
    itemsVector.push_back({103, "Stone Chestplate", 0.26, 0, 5});
    itemsVector.push_back({104, "Iron Greaves", 0.21, 0, 6});
    itemsVector.push_back({105, "Iron Chestplate", 0.27, 0, 7});
    itemsVector.push_back({106, "Tin Greaves", 0.16, 0, 4});
    itemsVector.push_back({107, "Tin Chestplate", 0.21, 0, 5});
    itemsVector.push_back({108, "Tungsten Greaves", 0.46, 0, 14});
    itemsVector.push_back({109, "Tungsten Chestplate", 0.58, 0, 20});
    itemsVector.push_back({110, "Copper Greaves", 0.22, 0, 7});
    itemsVector.push_back({111, "Copper Chestplate", 0.27, 0, 8});

    // Food
    itemsVector.push_back({200, "Apple", 0.0, 0, 0, true, 2});
    itemsVector.push_back({201, "Rotten Apple", 0.0, 0, 0, true, -3});
    itemsVector.push_back({202, "Infused Apple", 0.0, 0, 0, true, 10});
    itemsVector.push_back({203, "Sandwich", 0.0, 0, 0, true, 5});
    itemsVector.push_back({204, "Rotten Sandwich", 0.0, 0, 0, true, -6});
    itemsVector.push_back({205, "Infused Sandwich", 0.0, 0, 0, true, 25});
    itemsVector.push_back({206, "Smore", 0.0, 0, 0, true, 4});
    itemsVector.push_back({207, "Rotten Smore", 0.0, 0, 0, true, -5});
    itemsVector.push_back({200, "Infused Smore", 0.0, 0, 0, true, 20});

    //itemsVector.push_back({999, "Nothing", 0.0, 0, 0});

    return itemsVector;
}

