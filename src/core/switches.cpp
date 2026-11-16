#include "switches.h"
#include "util.h"
#include <physics.h>

bool canUseLift1(int fromFloor, int toFloor){

    if((fromFloor == 0 || fromFloor == 1) && (toFloor == 0 || toFloor == 1) && (fromFloor != toFloor)){
        return true;
    }
    return false;

}
bool canUseLift2(int fromFloor, int toFloor, bool lift2Operational, int emergencyTripsLeft){

    if(lift2Operational == true || emergencyTripsLeft > 0){
        if((fromFloor == 1 || fromFloor == 3 || fromFloor == 4) && (toFloor == 1 || toFloor == 3 || toFloor == 4) && (fromFloor != toFloor)){
            return true;

        }
    }
    return false;

}
bool unlockFloor3Door(const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount){

    if(hasItem(inventory , inventoryCount , ENT_MASTER_KEY)) return true;
    return false;

}
bool installFuseControlPanel(char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount, bool objectives[NUM_OBJECTIVES], int& score){

    if(objectives[OBJ_FUSE_INSTALLED] == true){
        return false;
    }
    if(removeItem(inventory , inventoryCount , ENT_FUSE)){
        objectives[OBJ_FUSE_INSTALLED] = true;
        score = score + SCORE_INSTALL_FUSE;
        if(objectives[OBJ_LIFT_PASS]){
            objectives[OBJ_LIFT2_ACTIVE] = true;
        }
        return true;
    }
    return false;
}
bool verifyCabinetCode(const char* enteredCode, const bool objectives[NUM_OBJECTIVES]){
    
    if(objectives[OBJ_CLUE_2] == true && objectives[OBJ_CLUE_4] == true && objectives[OBJ_CLUE_7] == true){
        if(stringEquals(enteredCode , "427")){
            return true;
        }
    
    }
    return false;

}
