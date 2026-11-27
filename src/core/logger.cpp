#include "logger.h"
#include "util.h"
#include <fstream>
#include <cstring>
using namespace std;


bool saveGameStateToFile(const char* filename, int playerFloor, int playerRoom, int energy, int time, int score, const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount, const bool objectives[NUM_OBJECTIVES], int emergencyTripsLeft, const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM], const int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR]){

    if(filename == nullptr){
        return false;
    }
    

    ofstream outfile(filename);

    if(!outfile.is_open()){
        return false;
    }
    outfile<<"PLAYER_FLOOR: "<<playerFloor<<endl;
    outfile<<"PLAYER_ROOM: "<<playerRoom<<endl;
    outfile<<"ENERGY: "<<energy<<endl;
    outfile<<"TIME: "<<time<<endl;
    outfile<<"SCORE: "<<score<<endl;
    outfile<<"EMERGENCY_TRIPS_LEFT: "<<emergencyTripsLeft<<endl;

    if(inventoryCount <= 0){
        outfile<<"INVENTORY: NONE"<<endl;
    }
    else{
        outfile<<"INVENTORY: ";
        for(int i = 0; i < inventoryCount; i++){
            outfile<<inventory[i]<<",";
        }
        outfile<<endl;

    }
    for(int i = 0; i < NUM_OBJECTIVES; i++){
        outfile<<"OBJECTIVE_"<<i<<": "<<objectives[i]<<endl;
    }
    for(int i = 0; i < NUM_FLOORS; i++){
        for(int j = 0; j < ROOMS_PER_FLOOR; j++){
            if(entityCounts[i][j] <= 0){
                outfile<<"ROOM_"<<i<<"_"<<j<<": "<<"NONE"<<endl;
                continue;
            }
            for(int k = 0; k < entityCounts[i][j]; k++){
                outfile<<"ROOM_"<<i<<"_"<<j<<": "<<roomEntities[i][j][k]<<endl;
            }
        }
    }

    outfile.close();
    return true;
}
bool loadGameStateFromFile(const char* filename,   int& playerFloor, int& playerRoom,   int& energy, int& time, int& score,   char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,   bool objectives[NUM_OBJECTIVES], int& emergencyTripsLeft,   char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],   int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR]){

    ifstream file(filename);
    if (!file.is_open())
        return false;

    playerFloor = -1;
    playerRoom = -1;
    energy = INITIAL_ENERGY;
    time = INITIAL_TIME;
    score = INITIAL_SCORE;
    inventoryCount = 0;
    emergencyTripsLeft = 0;

    for(int i = 0; i < NUM_FLOORS; i++){
        for(int j = 0; j < ROOMS_PER_FLOOR; j++){
            entityCounts[i][j] = 0;
             for(int k = 0; k < MAX_ENTITIES_PER_ROOM; k++){
                roomEntities[i][j][k] = ENT_NONE;
        }
        }
       
    }
    for (int i = 0; i < MAX_INVENTORY_CAPACITY; i++) {
        inventory[i] = ENT_NONE;
    }

    for (int i = 0; i < NUM_OBJECTIVES; i++) {
        objectives[i] = false;
    }

    if (filename == nullptr)
        return false;

    

    char line[MAX_LINE_LEN];

    while (file.getline(line, MAX_LINE_LEN)) {
        trimWhitespace(line);
        if (line[0] == '\0') continue;

        int colonIndex = -1;
        for (int i = 0; line[i] != '\0'; i++) {
            if (line[i] == ':') {
                colonIndex = i;
                break;
            }
        }
        if (colonIndex == -1) {
            continue;
        }

        line[colonIndex] = '\0';
        char* key = line;
        char* value = &line[colonIndex + 1];
        trimWhitespace(key);
        trimWhitespace(value);

        if(stringEquals(key , "PLAYER_FLOOR")){
            if(atoi(value) >= 0 && atoi(value) < NUM_FLOORS){
                playerFloor = atoi(value);
            }
        }
        if(stringEquals(key , "PLAYER_ROOM")){
            if(atoi(value) >= 0 && atoi(value) < ROOMS_PER_FLOOR){
                playerRoom = atoi(value);
            }
        }
        if(stringEquals(key , "EMERGENCY_TRIPS_LEFT")){
            if(atoi(value) >= 0){
                emergencyTripsLeft = atoi(value);
            }

        }
        if (stringEquals(key, "ENERGY")){
            energy = clampInt(atoi(value), 0, MAX_ENERGY);
        }
        if (stringEquals(key, "TIME")){
            time = clampInt(atoi(value), 0, MAX_TIME);
        }
        if (stringEquals(key, "SCORE")){
            if(atoi(value) >= 0){
                score = atoi(value);
            }
        }
        
        if (stringEquals(key, "INVENTORY")){
            if (!stringEquals(value, "NONE") && value[0] != '\0') {
                for (int i = 0; value[i] != '\0' && inventoryCount < MAX_INVENTORY_CAPACITY; i++) {
                    if (value[i] != ' ' && value[i] != ',') {
                        inventory[inventoryCount] = value[i];
                        inventoryCount++;
                    }
                }
            }
        }
        if(strncmp(key , "OBJECTIVE_" , 10) == 0){
            int idx = atoi(&key[10]);
            if(idx >= 0 && idx < NUM_OBJECTIVES){
                objectives[idx] = (atoi(value) != 0);
            }
        }
        if(strncmp(key , "ROOM_" , 5) == 0){
            int f = key[5] - '0';
            int r = key[7] - '0';
            if(f >= 0 && f < NUM_FLOORS && r >= 0 && r < ROOMS_PER_FLOOR){
                if(!stringEquals(value, "NONE") && value[0] != '\0'){
                    if(entityCounts[f][r] < MAX_ENTITIES_PER_ROOM){
                        roomEntities[f][r][entityCounts[f][r]] = value[0];
                        entityCounts[f][r]++;
                    }
                }
            }
        }




    }
    if(playerFloor < 0 ||  playerRoom < 0){
        return false;
    }
    file.close();
    return true;
}
void writeGameSummaryReport(const char* filename,    int score, int remainingEnergy, int remainingTime,    const bool objectives[NUM_OBJECTIVES], bool victory){

    if (filename == nullptr)
        return;

    ofstream outfile(filename);

    if(!outfile.is_open()){
        return;
    }

    if(victory){
        outfile<<"THE PLAYER HAS WON"<<endl;
    }
    else{
        outfile<<"THE PLAYER HAS BEEN DEFEATED"<<endl;
    }
    outfile<<"FINAL SCORE : "<<score<<endl;
    outfile<<"ENERGY : "<<remainingEnergy<<endl;
    outfile<<"TIME : "<<remainingTime<<endl;

    int counter = 0;
    
    for(int i = 0; i < NUM_OBJECTIVES; i++){
        
        if(objectives[i] == true){
            outfile<<"OBJECTIVE_"<<i<<" : COMPLETED"<<endl;
            counter++;
        }
        else{
            outfile<<"OBJECTIVE_"<<i<<" : INCOMPLETE"<<endl;
        }
    }
    outfile<<counter<<" out of "<< NUM_OBJECTIVES << " objectives are completed"<<endl;

    outfile<<"===SUMMARY==="<<endl;
    if(victory){
        outfile<<"The game ended because the player has completed required objectives and boarded train"<<endl;
    }
    else{
        if(remainingEnergy <= 0){
            outfile<<"The game ended because the player lost due to insufficent energy"<<endl;
        }
        else if(remainingTime <= 0){
            outfile<<"The game ended because the player lost due to insufficent time"<<endl;
        }
        else{
            outfile<<"The Game Ended due to early termination"<<endl;
        }
    }
    outfile.close();

}