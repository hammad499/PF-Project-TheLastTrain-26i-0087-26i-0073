#include "physics.h"
#include "util.h"

bool hasItem(const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount, char itemSymbol){

    if(inventoryCount < 0 || inventoryCount > MAX_INVENTORY_CAPACITY) {
        return false;
    }

    for(int i = 0; i < inventoryCount; i++){
        if(inventory[i] == itemSymbol){
            return true;
        }

    }
    return false;



}
bool addItem(char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount, char itemSymbol){

    if(inventoryCount < 0 || inventoryCount >= MAX_INVENTORY_CAPACITY) {
        return false;
    }
    inventory[inventoryCount] = itemSymbol;
    inventoryCount++;
    return true;

}
bool removeItem(char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount, char itemSymbol){

    if(inventoryCount < 0 || inventoryCount > MAX_INVENTORY_CAPACITY) {
        return false;
    }

    for(int i =0; i < inventoryCount ; i++){
        if(inventory[i] == itemSymbol){
            for(int j = i; j < inventoryCount - 1; j++){
                inventory[j] = inventory[j+1];
            }
            inventory[inventoryCount-1] = ENT_NONE;
            inventoryCount--;
            return true;
        }
    }
    return false;

}
