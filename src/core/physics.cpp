#include "physics.h"
#include "util.h"
#include "switches.h"

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
void applyResourceDelta(int& energy, int& time, int deltaEnergy, int deltaTime){

    energy = clampInt(energy + deltaEnergy , 0 , MAX_ENERGY);
    time = clampInt(time + deltaTime , 0 , MAX_TIME);
}
void applyScoreDelta(int& score, int deltaScore){

    score = score + deltaScore;
    if(score < 0) score = 0;
}
bool isDirectlyConnected(int fromF, int fromR, int toF, int toR, const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1], char& outConnType){
    
    outConnType = CONN_NONE;
    if((fromF == toF && fromR == toR) || fromF < 0 || fromR < 0 || toF < 0 || toR < 0 || fromF > NUM_FLOORS-1 || fromR > ROOMS_PER_FLOOR-1 || toF > NUM_FLOORS-1 || toR > ROOMS_PER_FLOOR-1){
        return false;

    }
    if(fromF == toF){
        if(fromR - toR == 1 || toR - fromR == 1){
            int minRoom = (fromR < toR)? fromR : toR;
            char conn = horizontalConn[fromF][minRoom];
            if(conn != CONN_NONE && conn != CONN_SOLID_WALL){
                outConnType = conn;
                return true;
            }
        }
    }
    if(fromR == toR){
        if(fromR == 0){
            bool s1 = (fromF == 0 && toF == 1) || (fromF == 1 && toF == 0);
            bool s2 = (fromF == 1 && toF == 2) || (fromF == 2 && toF == 1);
            bool s3 = (fromF == 3 && toF == 4) || (fromF == 4 && toF == 3);
            if(s1 || s2 || s3){
                outConnType = 'S';
                return true;
            }
        }
        

        if (fromR == 3) {
        bool validFrom = (fromF == 1 || fromF == 3 || fromF == 4);
        bool validTo   = (toF == 1 || toF == 3 || toF == 4);
        if (validFrom && validTo) {
            outConnType = '2';
            return true;

        }
    }


    }
    if((fromR == 1 || fromR == 2) && (toR == 1 || toR == 2)){
            if(canUseLift1(fromF , toF)){
                outConnType = '1';
                return true;
            }
        }

return false;

}
bool canTraverse(int targetFloor, int targetRoom, char connType, const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount, int emergencyTripsLeft, bool lift2Operational){
    if(targetFloor < 0 || targetFloor >= NUM_FLOORS || targetRoom < 0 || targetRoom >= ROOMS_PER_FLOOR){
        return false;
    }
    if(connType == CONN_DOOR || connType == CONN_WINDOW || connType == 'S' || connType == '1'){
        return true;
    }
    if(connType == CONN_LOCKED_DOOR){
        return unlockFloor3Door(inventory , inventoryCount);
    }
    if(connType == '2'){
        return lift2Operational || emergencyTripsLeft > 0;

    }
    return false;

}

