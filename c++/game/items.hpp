#include "inc.hpp"

#define TOTAL_ITEMS 11

// Given an item ID will return the corresponding item
Item lookup(int id) {
    switch (id) {
        case 0: {
            Item woodSword;
            woodSword.id = 0;
            woodSword.name = "Wood Sword";
            woodSword.weight = 0.09;
            woodSword.damage = 3;
            return woodSword;
        }
        case 1: {
            Item woodShield;
            woodShield.id = 1;
            woodShield.name = "Wood Shield";
            woodShield.weight = 0.13;
            woodShield.block = 2;
            return woodShield;
        }
        case 2: {
            Item stoneSword;
            stoneSword.id = 2;
            stoneSword.name = "Stone Sword";
            stoneSword.weight = 0.12;
            stoneSword.damage = 4;
            return stoneSword;
        }
        case 3: {
            Item stoneShield;
            stoneShield.id = 3;
            stoneShield.name = "Stone Shield";
            stoneShield.weight = 0.20;
            stoneShield.block = 3;
            return stoneShield;
        }
        case 4: {
            Item ironSword;
            ironSword.id = 4;
            ironSword.name = "Iron Sword";
            ironSword.weight = 0.18;
            ironSword.damage = 6;
            return ironSword;
        }
        case 5: {
            Item ironShield;
            ironShield.id = 5;
            ironShield.name = "Iron Shield";
            ironShield.weight = 0.25;
            ironShield.block = 5;
            return ironShield;
        }
        case 6: {
            Item tinSword;
            tinSword.id = 6;
            tinSword.name = "Tin Sword";
            tinSword.weight = 0.14;
            tinSword.damage = 4;
            return tinSword;
        }
        case 7: {
            Item tinShield;
            tinShield.id = 7;
            tinShield.name = "Tin Shield";
            tinShield.weight = 0.18;
            tinShield.block = 3;
            return tinShield;
        }
        case 8: {
            Item tungstenSword;
            tungstenSword.id = 8;
            tungstenSword.name = "Tungsten Sword";
            tungstenSword.weight = 0.22;
            tungstenSword.damage = 8;
            return tungstenSword;
        }
        case 9: {
            Item tungstenShield;
            tungstenShield.id = 9;
            tungstenShield.name = "Tungsten Shield";
            tungstenShield.weight = 0.30;
            tungstenShield.block = 7;
            return tungstenShield;
        }
        case 10: {
            Item copperSword;
            copperSword.id = 10;
            copperSword.name = "Copper Sword";
            copperSword.weight = 0.16;
            copperSword.damage = 5;
            return copperSword;
        }
        case 11: {
            Item copperShield;
            copperShield.id = 11;
            copperShield.name = "Copper Shield";
            copperShield.weight = 0.22;
            copperShield.block = 4;
            return copperShield;
        }
        default: {
            Item nullItem;
            nullItem.id = -1;
            nullItem.name = "NULL";
            nullItem.weight = 0.0;
            return nullItem;
        }
    }
}