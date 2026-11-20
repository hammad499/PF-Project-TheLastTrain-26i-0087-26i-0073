#include "simulator.h"
#include "physics.h"
#include "switches.h"
#include "logger.h"
#include "viewer_iface.h"
#include "util.h"

bool checkEndConditions(int energy, int time, const bool objectives[NUM_OBJECTIVES]){
    
    if(energy <= 0 || time <= 0 || objectives[OBJ_MISSION_WON] == true || objectives[OBJ_MISSION_FAILED] == true){
        return true;
    }
    return false;

}
bool handleMoveCommand(int targetFloor, int targetRoom,   int& currentFloor, int& currentRoom,   int& energy, int& time,   const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],   const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,   int& emergencyTripsLeft, bool lift2Operational){

    int denergy = 0;
    int dtime = 0;
    char connType = CONN_NONE;
    if(!isDirectlyConnected(currentFloor , currentRoom , targetFloor , targetRoom , horizontalConn , connType)){
        return false;

    }
    if(!canTraverse(targetFloor, targetRoom, connType, inventory , inventoryCount, emergencyTripsLeft, lift2Operational)){
        return false;
    }
    if(connType == CONN_DOOR || connType == CONN_LOCKED_DOOR || connType == 'S'){
        denergy = COST_MOVE_ORDINARY_ENERGY;
        dtime = COST_MOVE_ORDINARY_TIME;
    }
    if(connType == CONN_WINDOW){
        denergy = COST_MOVE_WINDOW_ENERGY;
        dtime = COST_MOVE_WINDOW_TIME;
    }
    if(connType == '1' || connType == '2'){
        denergy = COST_MOVE_LIFT_ENERGY;
        dtime = COST_MOVE_LIFT_TIME;
    }
    currentFloor = targetFloor;
    currentRoom = targetRoom;
    if(connType == '2' && !lift2Operational){
        emergencyTripsLeft--;
    }
    applyResourceDelta(energy , time , denergy , dtime);
    return true;

}

bool handleInspectCommand(int currentFloor, int currentRoom,  int& energy, int& time,  const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],  int entityCount, bool objectives[NUM_OBJECTIVES], int& score){

    if(currentFloor < 0 || currentFloor >= NUM_FLOORS || currentRoom < 0 || currentRoom >= ROOMS_PER_FLOOR){
        return false;
    }
    applyResourceDelta(energy , time , COST_INSPECT_ENERGY , COST_INSPECT_TIME);

    for(int i = 0; i < entityCount; i++){
        char ent = roomEntities[currentFloor][currentRoom][i];
        if(ent == ENT_CLUE_4 && objectives[OBJ_CLUE_4] == false){
            objectives[OBJ_CLUE_4] = true;
            applyScoreDelta(score , SCORE_CLUE_FOUND);
        }
        if(ent == ENT_CLUE_2 && objectives[OBJ_CLUE_2] == false){
            objectives[OBJ_CLUE_2] = true;
            applyScoreDelta(score , SCORE_CLUE_FOUND);
        }
        if(ent == ENT_CLUE_7 && objectives[OBJ_CLUE_7] == false){
            objectives[OBJ_CLUE_7] = true;
            applyScoreDelta(score , SCORE_CLUE_FOUND);
        }

    }
    return true;


}
bool handleCollectCommand(char itemSymbol, int currentFloor, int currentRoom,  int& energy, int& time, int& score,  char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],  int& entityCount,  char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,  bool objectives[NUM_OBJECTIVES]){

    int denergy = COST_ITEM_PICKUP_ENERGY;
    int dtime = COST_ITEM_PICKUP_TIME;
    bool instant = false;
    if (currentFloor < 0 || currentFloor >= NUM_FLOORS || currentRoom < 0 || currentRoom >= ROOMS_PER_FLOOR){

        return false;
    }

    if(itemSymbol == ENT_CABINET_TRAIN_KEY && !objectives[OBJ_CABINET_UNLOCKED]){
        return false;
    }
    if (itemSymbol == ENT_NONE || itemSymbol == ENT_CONTROL_PANEL || itemSymbol == ENT_PASSENGER || itemSymbol == ENT_CAT || itemSymbol == ENT_CLUE_4 || itemSymbol == ENT_CLUE_2 || itemSymbol == ENT_CLUE_7){

        return false;
    }

    int foundindex = -1;
    for(int i = 0; i < entityCount; i++){

        if(roomEntities[currentFloor][currentRoom][i] == itemSymbol){
            foundindex = i;
            break;
        }


    }

    if(foundindex == -1){

        return false;
    }
    if(itemSymbol == ENT_ENERGY_PACK){
        denergy += BONUS_ENERGY_PACK;
        instant = true;

    }
    if(itemSymbol == ENT_EMERGENCY_TOOLKIT){
        denergy += BONUS_EMERGENCY_TOOLKIT;
        instant = true;
    }
    if(itemSymbol == ENT_OBSERVATION_TOKEN){
        dtime += BONUS_OBSERVATION_TOKEN;
        instant = true;
    }

    

    if(!instant && !addItem(inventory , inventoryCount , itemSymbol)){
        return false;
    }
    
    for(int i = foundindex; i < entityCount - 1 ; i++){

        roomEntities[currentFloor][currentRoom][i] = roomEntities[currentFloor][currentRoom][i + 1];
        

    }
    roomEntities[currentFloor][currentRoom][entityCount - 1] = ENT_NONE;
    entityCount--;

   

    applyResourceDelta(energy , time , denergy , dtime);
    if(itemSymbol == ENT_MASTER_KEY){
        if(!objectives[OBJ_MASTER_KEY]){
            objectives[OBJ_MASTER_KEY] = true;
            applyScoreDelta(score , SCORE_KEY_COLLECT);
        }

    }
    if(itemSymbol == ENT_CABINET_TRAIN_KEY && objectives[OBJ_CABINET_UNLOCKED]){
        if(!objectives[OBJ_TRAIN_KEY]){
            objectives[OBJ_TRAIN_KEY] = true;
            applyScoreDelta(score , SCORE_TRAIN_KEY);
        }
    }
    if(itemSymbol == ENT_CAT_CARRIER){
        if(!objectives[OBJ_CAT_CARRIER]){
            objectives[OBJ_CAT_CARRIER] = true;
        }
    }
    if(itemSymbol == ENT_FUSE){
        if(!objectives[OBJ_FUSE_COLLECTED]){
            objectives[OBJ_FUSE_COLLECTED] = true;
        }
    }
    if(itemSymbol == ENT_LIFT_PASS){
        if(!objectives[OBJ_LIFT_PASS]){
            objectives[OBJ_LIFT_PASS] = true;
            applyScoreDelta(score , SCORE_LIFT_PASS);
        }
        if (objectives[OBJ_FUSE_INSTALLED]) {
            objectives[OBJ_LIFT2_ACTIVE] = true;
        }
    }
    



    return true;

}
bool handleUseCommand(char itemSymbol, int currentFloor, int currentRoom,  int& energy, int& time, int& score,  char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,  bool objectives[NUM_OBJECTIVES]){

    int denergy = COST_ITEM_USE_ENERGY;
    int dtime = COST_ITEM_USE_TIME;

    if(currentFloor < 0 || currentFloor > NUM_FLOORS - 1 || currentRoom < 0 || currentRoom > ROOMS_PER_FLOOR - 1){
        return false;
    }

    if(!hasItem(inventory , inventoryCount , itemSymbol)){
        return false;
    }

    if(itemSymbol == ENT_FUSE && !objectives[OBJ_FUSE_INSTALLED]){
        if(currentFloor == 2 && currentRoom == 3){
            if (installFuseControlPanel(inventory, inventoryCount, objectives, score)){
                applyResourceDelta(energy, time, denergy, dtime);
                return true;
            }
            
        }
    }
    if(itemSymbol == ENT_MEDICAL_KIT && !objectives[OBJ_PASSENGER_RESCUED]){
        if(currentFloor == 3 && currentRoom == 0){
            objectives[OBJ_PASSENGER_RESCUED] = true;
            applyResourceDelta(energy , time , denergy , dtime);
            removeItem(inventory , inventoryCount , itemSymbol);
            applyScoreDelta(score , SCORE_RESCUE_PASSENGER);
            return true;
        }
    }
    if(itemSymbol == ENT_CAT_CARRIER && !objectives[OBJ_CAT_SECURED]){
        if(currentFloor == 2 && currentRoom == 2){
            objectives[OBJ_CAT_SECURED] = true;
            applyResourceDelta(energy , time , denergy , dtime);
            
            applyScoreDelta(score , SCORE_SECURE_CAT);
            return true;
        }
    }
    if(itemSymbol == ENT_CABINET_TRAIN_KEY && !objectives[OBJ_TRAIN_PREPARED] && objectives[OBJ_PASSENGER_RESCUED] && objectives[OBJ_CAT_SECURED] && objectives[OBJ_FUSE_INSTALLED]){
        if(currentFloor == 4 && currentRoom == 1){
            objectives[OBJ_TRAIN_PREPARED] = true;
            objectives[OBJ_MISSION_WON] = true;
            applyResourceDelta(energy , time , denergy , dtime);
            removeItem(inventory , inventoryCount , itemSymbol);
            applyScoreDelta(score , SCORE_PREPARE_TRAIN);
            applyScoreDelta(score , SCORE_MISSION_VICTORY);
            return true;
        }
    }

    return false;
}
bool handleUnlockCabinetCommand(const char* code, int currentFloor, int currentRoom,    bool objectives[NUM_OBJECTIVES], int& score,    char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],    int& entityCount){

    bool found = false;
    if(currentFloor == 4 && currentRoom == 0 && objectives[OBJ_CLUE_4] && objectives[OBJ_CLUE_2] && objectives[OBJ_CLUE_7] && !objectives[OBJ_CABINET_UNLOCKED]){
        if(stringEquals(code , "427")){
            objectives[OBJ_CABINET_UNLOCKED] = true;
            applyScoreDelta(score , SCORE_CABINET_UNLOCK);
            for(int i = 0; i<entityCount; i++){
                if(roomEntities[currentFloor][currentRoom][i] == ENT_CABINET_TRAIN_KEY){
                    found = true;
                    break;
                }
                
            }
            if(!found && entityCount < MAX_ENTITIES_PER_ROOM){
                    roomEntities[currentFloor][currentRoom][entityCount] = ENT_CABINET_TRAIN_KEY;
                    entityCount++;
                }
            return true;    
        }
    }
    return false;


}
