#include "level.h"
#include "util.h"
#include <fstream>
#include <iostream>
using namespace std;

const char* getEntityName(char entitySymbol){

    switch (entitySymbol) {
        case ENT_NONE:              return "None";
        case ENT_MASTER_KEY:        return "Master Key";
        case ENT_ENERGY_PACK:       return "Energy Pack";
        case ENT_EMERGENCY_TOOLKIT: return "Emergency Toolkit";
        case ENT_CLUE_4:            return "Clue Digit 4";
        case ENT_FUSE:              return "Fuse";
        case ENT_CAT:               return "Cat";
        case ENT_CONTROL_PANEL:     return "Control Panel";
        case ENT_LIFT_PASS:         return "Lift Pass";
        case ENT_PASSENGER:         return "Passenger";
        case ENT_MEDICAL_KIT:       return "Medical Kit";
        case ENT_EMERGENCY_CARD:    return "Emergency Access Card";
        case ENT_CLUE_2:            return "Clue Digit 2";
        case ENT_CLUE_7:            return "Clue Digit 7";
        case ENT_CABINET_TRAIN_KEY: return "Cabinet / Train Key";
        case ENT_CAT_CARRIER:       return "Cat Carrier";
        case ENT_OBSERVATION_TOKEN: return "Observation Token";
        default:                    return "Unknown";
    }




    
}

bool validateStartingRoom(int startFloor, int startRoom, const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1]){

    if(startFloor < 0 || startFloor >= NUM_FLOORS || startRoom < 0 || startRoom > ROOMS_PER_FLOOR - 1){

        return false;
    }
    if(startRoom > 0){
            if(horizontalConn[startFloor][startRoom - 1] != CONN_NONE && horizontalConn[startFloor][startRoom - 1] != CONN_SOLID_WALL){
                return true;
            }
        }
    if(startRoom < ROOMS_PER_FLOOR - 1){

        if(horizontalConn[startFloor][startRoom] != CONN_NONE && horizontalConn[startFloor][startRoom] != CONN_SOLID_WALL){
            return true;
        }

    }
    return false;

}

bool loadInitialState(const char* filename, int& playerFloor, int& playerRoom, int& energy, int& time, int& score, char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount, bool objectives[NUM_OBJECTIVES], int& emergencyTripsLeft){

    playerFloor = -1;
    playerRoom = -1;
    energy = 100;
    time = 50;
    score = 0;
    inventoryCount = 0;
    emergencyTripsLeft = 0;

    for(int i = 0; i<MAX_INVENTORY_CAPACITY; i++){

        inventory[i] = ENT_NONE;

    }

    for(int i = 0; i < NUM_OBJECTIVES; i++){
        objectives[i] = false;


    }

    if(filename == nullptr)
        return false;
    
    ifstream file(filename);

    if(!file.is_open())
        return false;
    
    char line[256];

    while(file.getline(line , 256)){
        int colonIndex = -1;
        trimWhitespace(line);
        if(line[0] == '\0') continue;

        for(int i = 0 ; line[i] != '\0' ; i++){
            if(line[i] == ':'){
                colonIndex = i;
                break;

            }

        }
        if(colonIndex == -1){
            continue;
        }
        line[colonIndex] = '\0';

        char* key = line;
        char* value = &line[colonIndex + 1];
        trimWhitespace(key);
        trimWhitespace(value);

        if(stringEquals(key , "INITIAL_ENERGY")){

           energy = atoi(value); 

        }
        else if(stringEquals(key , "INITIAL_TIME")){

            time = atoi(value);

        }
        else if(stringEquals(key , "INITIAL_SCORE")){

            score = atoi(value);

        }
        else if(stringEquals(key , "START_ROOM")){
            if(!stringEquals(value , "PLAYER_SELECTS")){
                if(value[0] == 'F' && value[2] == '-' && value[3] == 'R'){
                    playerFloor = value[1] - '1';
                    playerRoom = value[4] - '1';
                }
            }

        }
        else if(stringEquals(key , "INITIAL_INVENTORY")){
            if(!stringEquals(value , "NONE") && value[0] != '\0'){
                for(int i = 0; value[i] != '\0' && inventoryCount < MAX_INVENTORY_CAPACITY; i++){
                    if(value[i] != ' ' && value[i] != ','){
                        inventory[inventoryCount] = value[i];
                        inventoryCount++;
                    }
                        
                        
                }

                
            }

        }
        else if (stringEquals(key, "PASSENGER_RESCUED")) {
            if (stringEquals(value, "YES")) 
            objectives[OBJ_PASSENGER_RESCUED] = true;
        } 
        else if (stringEquals(key, "CAT_RESCUED")) {
            if (stringEquals(value, "YES")) 
            objectives[OBJ_CAT_SECURED] = true;
        } 
        else if (stringEquals(key, "CABINET_UNLOCKED")) {
            if (stringEquals(value, "YES")) 
            objectives[OBJ_CABINET_UNLOCKED] = true;
        } 
        else if (stringEquals(key, "TRAIN_KEY_COLLECTED")) {
            if (stringEquals(value, "YES")) 
            objectives[OBJ_TRAIN_KEY] = true;
        } 
        else if (stringEquals(key, "FUSE_INSTALLED")) {
            if (stringEquals(value, "YES")) 
            objectives[OBJ_FUSE_INSTALLED] = true;
        } 
        else if (stringEquals(key, "LIFT_PASS_COLLECTED")) {
            if (stringEquals(value, "YES")) 
            objectives[OBJ_LIFT_PASS] = true;
        }
        else if (stringEquals(key, "TRAIN_PREPARED")) {
            if (stringEquals(value, "YES")) 
            objectives[OBJ_TRAIN_PREPARED] = true;
        }

        

    }

    return true;

                
}
    
    
