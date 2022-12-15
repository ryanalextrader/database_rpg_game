//credit sources :)

#include <iostream>
#include <iomanip>
#include <ctime>
#include "windows.h"
#include "..\headers\cursor.h"
#include "..\headers\map.h"
#include "..\headers\consInteraction.h"
using std::cout;
using std::endl;
using std::string;

void detectInput(Map& map, bool & received_input_last_check){
    //check to move the cursor with WASD
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
    //space to act
    if(GetAsyncKeyState(' ') & 0x8000){
        if(!received_input_last_check){
            map.phaseAct();
        }
        input = true;
    }
    //e to skip
    if(GetAsyncKeyState('E') & 0x8000){
        if(!received_input_last_check){
            map.phaseAct(true);
        }
        input = true;
    }
    //q to open inventory
    if(GetAsyncKeyState('Q') & 0x8000){
        map.inventorySelection();
    }
    //escape to quit
    if(GetAsyncKeyState(VK_ESCAPE) & 0x8000){
        map.gameOver();
    }
    
    //ensure the cursor moves once per key press
    //this increases accuracy in selecting tiles or removes frustration of holding a key too long and getting double inputs
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
    bool last_input_moved = false;
    while(!map.getGameOver()){
        //run the game until we get a game over (die or quit)
        detectInput(map, last_input_moved);
        Sleep(10);
    }
}

int main(){
    srand(time(NULL));
    Map game;
    //make sure there is no console bloat
    clearScreen();
    clearScreen();
    //start/run the game
    game.printGrid();
    gameLoop(game);
    //wait a little after a game over before clearing the screen/exiting
    Sleep(3000);
    clearScreen();
}