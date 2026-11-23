#include "viewer_iface.h"
#include "util.h"
#include <iostream>
#include "level.h"

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
void renderAsciiBuilding(int playerFloor, int playerRoom){

    printHeader("Building Map");
    for(int i = NUM_FLOORS - 1; i >= 0; i--){

        cout<<"F" << (i + 1) << " | ";
        for (int j = 0; j < ROOMS_PER_FLOOR; j++){
            if(i == playerFloor && j == playerRoom){
                cout<<" [ P ] ";
            }
            else{
               cout<<" [R " << (j + 1) << " ] ";
            }
        }
        cout<<endl;


    }
    cout<<endl;
    cout<<"Stairs/Lifts: S1 (F1-R1 to F2-R1), L1 (F1-R2 or F1-R3 to F2-R2 or F2-R3), S2 (F2-R1 to F3-R1), L2 (F2-R4 to F5-R4), S3 (F4-R1 and F5-R1)";
    cout<<endl;
}
