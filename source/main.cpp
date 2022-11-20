#include <iostream>
#include <ctime>
#include "windows.h"
#include "..\headers\cursor.h"
#include "..\headers\map.h"
#include "..\headers\consInteraction.h"
using std::cout;
using std::endl;
using std::string;

void detectInput(Map& map, bool & received_input_last_check){
    char dir[] = {'W', 'A', 'S', 'D'};
    bool input = false;
    for(int i = 0; i < 4; i++){
        if(GetAsyncKeyState(dir[i]) & 0x8000) {
            if(!received_input_last_check){
                map.moveCursor(dir[i]);
            }
            input = true;
        }
    }
    if(GetAsyncKeyState(' ') & 0x8000){
        if(!received_input_last_check){
            map.phaseAct();
        }
        input = true;
    }
    if(GetAsyncKeyState('E') & 0x8000){
        if(!received_input_last_check){
            map.phaseAct(true);
        }
        input = true;
    }
    
    if(input){
        if(!received_input_last_check){
            map.printGrid();
            received_input_last_check = true;
        }
    }
    else{
        received_input_last_check = false;
    }
}

void gameLoop(Map& map){
    bool game_run = true;
    bool last_input_moved = false;
    while(game_run){
        detectInput(map, last_input_moved);
        Sleep(40);
    }
}

int main(){
    srand(time(NULL));
    Map game;
    clearScreen();
    clearScreen();
    game.printGrid();
    gameLoop(game);
    // int rise = -4;
    // int run = -3;
    // int move = 3;
    // int num = sqrt((move*move) * (1 + ((run*run) / (rise*rise))));
    // cout << num << endl;
}