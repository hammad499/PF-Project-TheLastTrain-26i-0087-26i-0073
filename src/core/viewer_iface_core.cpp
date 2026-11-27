#include "viewer_iface.h"
#include "util.h"
#include <iostream>
#include "level.h"
#include <fstream>

using namespace std;


void renderDashboard(int playerFloor, int playerRoom, int energy, int time, int score, const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount, int emergencyTripsLeft){

    printHeader("Player Position");

    cout<<"PLayer Floor : "<<(playerFloor + 1)<<" || PLayer Room : "<<(playerRoom + 1)<<endl;

    printHeader("Player Vitals");

    cout<<"Energy : "<<energy<<" || Time : "<<time<<" || Score : "<<score<<endl;

    printHeader("Inventory Information");

    if(inventoryCount <= 0){
        cout<<"Inventory : NONE"<<endl;

    }
    else{
        cout<<"Inventory : ";
        for(int i = 0; i < inventoryCount; i++){

            cout<<getEntityName(inventory[i])<<" : "<<inventory[i]<<" || ";

        }
        cout<<endl;
    }
    if(emergencyTripsLeft > 0){
        cout<<"Emergency Trips Left : "<<emergencyTripsLeft;
    }
    cout<<endl;

}
void renderRoomDetails(int floor, int room,
                       const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                       int entityCount, const bool objectives[NUM_OBJECTIVES]){
    if (floor < 0 || floor >= NUM_FLOORS || room < 0 || room >= ROOMS_PER_FLOOR) {
        return;
    }

    printHeader("Room Details");
    cout << "Current Room: F" << (floor + 1) << "-R" << (room + 1) << endl;

    if (entityCount <= 0) {
        cout << "Entities in Room: None" << endl;
        return;
    }

    cout << "Entities in Room:" << endl;
    for (int i = 0; i < entityCount && i < MAX_ENTITIES_PER_ROOM; i++) {
        if(roomEntities[floor][room][i] == ENT_CABINET_TRAIN_KEY && !objectives[OBJ_CABINET_UNLOCKED]){
            continue;
        }
        char sym = roomEntities[floor][room][i];
        cout << "  [" << sym << "] " << getEntityName(sym) << endl;
    }
}

void renderObjectivesChecklist(const bool objectives[NUM_OBJECTIVES]) {
    printHeader("Objectives Checklist");
    int completed = 0;

    for (int i = 0; i < NUM_OBJECTIVES; i++) {
        cout << "  " << (objectives[i] ? "[X] " : "[ ] ")
             << "Objective " << (i + 1) << endl;
        if (objectives[i]) {
            completed++;
        }
    }

    cout << "Completed: " << completed << " / " << NUM_OBJECTIVES << endl;
}
void renderAsciiBuilding(int playerFloor, int playerRoom) {
    printHeader("Building Map");

    ifstream file("data/building.txt");
    if (!file.is_open()) {
        cout << "Error: Could not open data/building.txt" << endl;
        return;
    }

    char line[MAX_LINE_LEN];

    while (file.getline(line, MAX_LINE_LEN)) {
        
        if (line[0] == 'D' && line[1] == ' ' && line[2] == '=' && line[3] == ' ') {
            break;
        }

        
        bool isHeaderLine = false;
        for (int i = 0; line[i] != '\0' && line[i + 1] != '\0' && line[i + 2] != '\0'; i++) {
            if (line[i] == 'F' && line[i + 1] >= '1' && line[i + 1] <= '5' && line[i + 2] == '-') {
                isHeaderLine = true;
                break;
            }
        }

        if (isHeaderLine) {
            cout << line << endl;
        } else {
           
            for (int k = 0; line[k] != '\0'; k++) {
                char ch = line[k];
                if (ch == ENT_MASTER_KEY || ch == ENT_ENERGY_PACK ||
                    ch == ENT_EMERGENCY_TOOLKIT || ch == ENT_CLUE_4 ||
                    ch == ENT_FUSE || ch == ENT_CAT ||
                    ch == ENT_CONTROL_PANEL || ch == ENT_LIFT_PASS ||
                    ch == ENT_PASSENGER || ch == ENT_MEDICAL_KIT ||
                    ch == ENT_EMERGENCY_CARD || ch == ENT_CLUE_2 ||
                    ch == ENT_CLUE_7 || ch == ENT_CABINET_TRAIN_KEY ||
                    ch == ENT_CAT_CARRIER || ch == ENT_OBSERVATION_TOKEN) {
                    cout << ' ';
                } else {
                    cout << ch;
                }
            }
            cout << endl;
        }
    }

    file.close();
    cout << endl;
    cout<<"D = Ordinary door\nW = Window\nL = Locked door (requires Master Key)\n# = Solid wall\nS1 = Staircase connecting F1-R1 and F2-R1\nS2 = Staircase connecting F2-R1 and F3-R1\nS3 = Staircase connecting F4-R1 and F5-R1\nL1 = Lift F1-R2 to F2-R2 and F1-R3 to F2-R3\nLIFT 2 = Lift 2 F2-R4 to F4-R4 and F5-R4"<<endl;
    cout << "Current Location: F" << (playerFloor + 1) << "-R" << (playerRoom + 1) << endl;
}