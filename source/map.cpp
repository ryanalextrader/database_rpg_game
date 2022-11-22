#include <iostream>
#include <iomanip>
#include "..\headers\map.h"
#include "..\headers\consInteraction.h"
#include "windows.h"

void Map::changeColor(int row_coord, int col_coord) const{
    // color for player token when player needs to move
    if(grid[row_coord][col_coord] == plr.getToken() && phase == 0){
        setTextColor(1);
    }
    // color for player token when player needs to attack
    else if(grid[row_coord][col_coord] == plr.getToken() && phase == 1){
        setTextColor(14);
    }
    // color for active monster (if any)
    else if(row_coord == active_mons_coord[0] && col_coord == active_mons_coord[1]){
        setTextColor(13);
    }
    // color for monsters
    else if (grid[row_coord][col_coord] != bkgrnd){
        setTextColor(4);
    }
    // color for background tiles that player can move to (movement radius)
    else if(plr.canMove(row_coord, col_coord)){
        setTextColor(10);
    }
    // color for background tiles that the player cannot move to
    else{
        setTextColor(15);
    }
}

bool Map::checkOverlap(int i) {
    if(mnstr[i].isDead()) {
        return false;
    }
    if(mnstr[i].getCol() == plr.getCol() && mnstr[i].getRow() == plr.getRow()) {
        return true;
    }
    for(int j = 0; j < mnstr.size(); j++) {
        if((mnstr[i].getCol() == mnstr[j].getCol() && mnstr[i].getRow() == mnstr[j].getRow()) && (j != i) && !mnstr[j].isDead()) {
            return true;
        }
    }
    return false;
}

void Map::handleOverlap(Monster & mnstr){
    int dir = rand() % 4;
    bool flag = true;
    while(flag) { //this loop will run AT MOST twice. If we are against 1 col wall, we are not against the other, and same with row walls
        flag = false;
        if(dir == 0 && mnstr.getCol() - 1 >= 0){
            mnstr.setCoords(mnstr.getRow(), mnstr.getCol() - 1);
            return;
        } else if(dir == 0) {
            dir++;
        }
        if(dir == 1 && mnstr.getCol() + 1 < grid[0].size()){
            mnstr.setCoords(mnstr.getRow(), mnstr.getCol() + 1);
            return;
        } else if(dir == 1){
            dir--; //go back through the loop to hit the prior statement
            flag = true;
        }
        if(dir == 2 && mnstr.getRow() - 1 >= 0){
            mnstr.setCoords(mnstr.getRow() - 1, mnstr.getCol());
            return;
        } else if(dir == 2){
            dir++;
        }
        if(dir == 3 && mnstr.getRow() + 1 < grid.size()){
            mnstr.setCoords(mnstr.getRow() + 1, mnstr.getCol());
            return;
        } else if(dir == 3){
            dir--;
            flag = true;
        }
    }
}

int Map::findMonster(int row_coord, int col_coord) const {
    // iterate through all monsters
    for(int i = 0; i < mnstr.size(); i++) {
        // if monster's coordinates matches row_coord and col_coord, return index of monster
        if(row_coord == mnstr[i].getRow() && col_coord == mnstr[i].getCol()) {
            return i;
        }
    }
    return -1; // signifies that no such monster was found within the monster vector
}

void Map::createNewMap(){
    clearScreen();

    level++;
    // to be replaced with queries at future date
    theme = "Fire";
    wall_color = rand() % 16;
    phase = 0;
    active_mons_coord[0] = -1;
    active_mons_coord[1] = -1;

    int min_rows_and_cols = 10;
    int range = 10;

    int rows = min_rows_and_cols + (rand() % range);
    int cols = min_rows_and_cols + (rand() % range);

    plr.setCoords(rand() % rows, rand() % cols);

    grid.clear();
    grid.assign(rows, vector<char>());
    for(int i = 0; i < rows; i++){
        grid[i].assign(cols, bkgrnd);
    }
    grid[plr.getRow()][plr.getCol()] = plr.getToken();

    int max_num_monsters = 7;
    int num_monsters = 1 + (rand() % max_num_monsters);
    string monster_name = "MONSTER";
    string description = "Indescribable creature, indescribably ugly";
    char symbol = '0';
    int behaveP = 2;
    int behaveS = 1;
    int behaveD = 0;
    int spd = 3;
    int max_health = 10;
    int mons_range = 1;
    int damage = 3;
    int damage_var = 1;
    float accuracy = 0.75;
    float accuracy_decay = 0.0;
    
    for(int i = 0; i < num_monsters; i++){
        mnstr.push_back(Monster(monster_name, description, rand() % cols, rand() % rows, 
        symbol, behaveP, behaveS, behaveD, spd, max_health, mons_range, damage, damage_var, 
        accuracy, accuracy_decay));
    }

    //make sure no monsters overlap with any other monsters or the player
    bool overlap = true;
    while(overlap) {
        overlap = false;
        for(int i = 0; i < mnstr.size(); i++) {
            if(checkOverlap(i)) {
                handleOverlap(mnstr[i]);
                overlap = true;
            }
        }
    }

    //add monsters to grid
    for(int i = 0; i < mnstr.size(); i++) {
        grid[mnstr[i].getRow()][mnstr[i].getCol()] = mnstr[i].getToken();
    }

    activity = "You enter a " + theme + " room with " + to_string(mnstr.size()) + " monsters. Move when ready.";
    printGrid();
}

Map::Map(){
    level = 0;
    bkgrnd = '+';
    block_width = 20;
    plr = Player(0, 0, "Bastard sword", "melee");
    createNewMap();
}

// Map::Map() : Map(12, 12, '+', 7, "Fire", 12) {}

// Map::Map(int rows, int cols, char back, int num_monst, string dungeon_theme, int wall_clr){
//     plr = Player(rand() % rows, rand() % cols, "Bastard sword", "melee");
//     block_width = 20;
//     level = 0;
    
//     phase = 0;
//     theme = dungeon_theme;
//     wall_color = wall_clr;
//     active_mons_coord[0] = -1;
//     active_mons_coord[1] = -1;
    

//     for(int i = 0; i < num_monst; i++) {
//         mnstr.push_back(Monster(rand() % cols, rand() % rows, '0', 2, 1, 0));
//     }
    
//     bkgrnd = back;
//     grid.assign(rows, vector<char>());
//     for(int i = 0; i < rows; i++){
//         grid[i].assign(cols, back);
//     }
//     grid[plr.getRow()][plr.getCol()] = plr.getToken();

//     //make sure no monsters overlap with any other monsters or the player
//     bool overlap = true;
//     while(overlap) {
//         overlap = false;
//         for(int i = 0; i < mnstr.size(); i++) {
//             if(checkOverlap(i)) {
//                 handleOverlap(mnstr[i]);
//                 overlap = true;
//             }
//         }
//     }

//     //add monsters to grid
//     for(int i = 0; i < mnstr.size(); i++) {
//         grid[mnstr[i].getRow()][mnstr[i].getCol()] = mnstr[i].getToken();
//     }

//     activity = "You enter a " + theme + " room with " + to_string(mnstr.size()) + " monsters. Move when ready.";
// }

void Map::setNumRowsCols(int row, int col){
    // set number of rows in the grid
    grid.resize(row);
    // for each row in grid, set column size
    for(int i = 0; i < row; i++){
        grid[i].resize(col);
    }
    crsr.moveCursor(0,0,row,col); // readjust cursor so it points to top left of the grid
}

void Map::moveCursor(char dir){
    if(dir == 'W'){ // move cursor up one when 'W' key is pressed (cursor row - 1)
        crsr.moveCursor(crsr.getRow() - 1, crsr.getCol(), grid.size(), grid[0].size());
    } else if(dir == 'A') { // move cursor left one when 'A' key is pressed (cursor col - 1)
        crsr.moveCursor(crsr.getRow(), crsr.getCol() - 1, grid.size(), grid[0].size());
    } else if(dir == 'S') { // move cursor down one when 'S' key is pressed (cursor row + 1)
        crsr.moveCursor(crsr.getRow() + 1, crsr.getCol(), grid.size(), grid[0].size());
    } else if(dir == 'D') {
        crsr.moveCursor(crsr.getRow(), crsr.getCol() + 1, grid.size(), grid[0].size());
    } else {
        return;
    }
}

void Map::phaseAct(bool skip){
    if(plr.getCurHp() == 0) {
        activity = "YOU DIED! GAME OVER!";
        return;
    }

    bool new_map_created = false;
    
    if(skip){
        if(phase == 0){
            phase++;
        }
        else if(phase == 1){
            handleMonsters();
            phase = 0;
        }
    }
    else if(phase == 0){
        if(movePlayer()){
            phase++;
        }
    }
    else if (phase == 1){
        if(playerAttack()){
            printGrid();
            Sleep(1500);
            handleMonsters();
            phase = 0;
        }
        //game state
        if(mnstr.empty()) {
            activity = "ALL MONSTERS VANQUISHED! (Press Space to Continue)";
            printGrid();
            while(!(GetAsyncKeyState(' ') & 0x8000)){
                Sleep(40);
            }
            createNewMap();
            new_map_created = true;
    }
    }
    if(phase == 0 && !new_map_created){
        activity = "Move when ready.";
    }
    if(phase == 1){
        activity = "Attack with all your might!";
    }
}

bool Map::movePlayer() {
    bool hasMoved = false;
    for(int i = 0; i < mnstr.size(); i++) {
        //check if we are moving on top of a monster
        if(crsr.getCol() == mnstr[i].getCol() && crsr.getRow() == mnstr[i].getRow() && !mnstr[i].isDead()) {
            return false;
        }
    }

    grid[plr.getRow()][plr.getCol()] = bkgrnd;
    if(plr.updateCoords(crsr.getRow(), crsr.getCol())) {
        hasMoved = true;
    }
    grid[plr.getRow()][plr.getCol()] = plr.getToken();

    return hasMoved;
}

bool Map::playerAttack(){
    int mnstr_index = findMonster(crsr.getRow(), crsr.getCol());
    if(plr.canAttack(crsr.getRow(), crsr.getCol()) && mnstr_index >= 0){
        int damage = plr.rollAttack(crsr.getRow(), crsr.getCol());
        mnstr[mnstr_index].receiveAttack(damage);
        if(mnstr[mnstr_index].isDead()){
            activity = "You killed the " + mnstr[mnstr_index].getName() + "!";
            grid[mnstr[mnstr_index].getRow()][mnstr[mnstr_index].getCol()] = bkgrnd;
            mnstr.erase(mnstr.begin() + mnstr_index);
        }
        else{
            if(plr.getWeaponClass() == "melee"){
                activity = "You swung your ";
            }
            else if(plr.getWeaponClass() == "range"){
                activity = "You aimed and fired your ";
            }
            activity += (plr.getWeaponName() + " and dealt " + to_string(damage) + " damage to the " + mnstr[mnstr_index].getName() + "!"); 
        }
        return true;
    }
    else{
        return false;
    }
}

void Map::handleMonsters(){
    bool held = false;
    for(int i = 0; i < mnstr.size(); i++){
        if(!mnstr[i].isDead()) {
            moveMonster(i);
            active_mons_coord[0] = mnstr[i].getRow();
            active_mons_coord[1] = mnstr[i].getCol();
            printGrid();
            Sleep(750);
            monsterAttack(i);
            printGrid();
            Sleep(750);
        }
    }
    active_mons_coord[0] = -1;
    active_mons_coord[1] = -1;
}

void Map::moveMonster(int index) {
    mnstr[index].setDest(plr, grid[0].size(), grid.size());

    //activity feed
    if(mnstr[index].getCurBehavior() == 0) {
        activity = mnstr[index].getName() + " wanders aimlessly.";
    } else if(mnstr[index].getCurBehavior() == 1) {
        activity = mnstr[index].getName() + " charges toward you!";
    } else if(mnstr[index].getCurBehavior() == 2) {
        activity = mnstr[index].getName() + " turns and flees!";
    }

    //move based on behavior
    grid[mnstr[index].getRow()][mnstr[index].getCol()] = bkgrnd;
    mnstr[index].updateCoords();

    //handle any overlaps
    bool overlap = true;
    while(overlap) {
        overlap = false;
        for(int i = 0; i < mnstr.size(); i++) {
            if(checkOverlap(i)) {
                overlap = true;
                handleOverlap(mnstr[index]);
            }
        }
    }
    grid[mnstr[index].getRow()][mnstr[index].getCol()] = mnstr[index].getToken();
}

void Map::monsterAttack(int index) {
    if(mnstr[index].canAttack(plr.getRow(), plr.getCol())) {
        int dmg = mnstr[index].rollAttack(plr.getRow(), plr.getCol());
        plr.receiveAttack(dmg);
        
        if(dmg < 0) {
            activity = mnstr[index].getName() + " tried to attack you but missed!";
        } else if(dmg == 0) {
            activity = "You deflected " + mnstr[index].getName() + "'s attack!";
        } else {
            activity = mnstr[index].getName() + " attacked you for " + to_string(dmg) + " damage!";
        }
    }
}

void Map::completeFloor() {
    //generate a new floor
}

void Map::gameOver() {
    //uhhh game over screen?
    //restart?
}

int Map::getNumRows() const {
    return grid.size();
}

int Map::getNumCols() const {
    return grid[0].size();
}

void Map::printGrid() {

    setCursorPos(0, 0);

    setTextColor(wall_color);
    for(int i = 0; i < 2 * (grid[0].size() + 2) - 1; i++) {
        cout << "#";
    }
    
    cout << endl;

    bool cursor_edge = false;
    for(int i = 0; i < grid.size(); i++){
        setTextColor(wall_color);
        cout << "#";
        for(int j = 0; j < grid[0].size(); j++){
            changeColor(i, j);
            if(i == crsr.getRow() && j == crsr.getCol()) {
                if(grid[i][j] != plr.getToken() && grid[i][j] != bkgrnd && plr.canAttack(i, j)){
                    setTextColor(14);
                    cout << "[";
                    changeColor(i,j);
                    cout << grid[i][j];
                    setTextColor(14);
                    cout << "]";
                }
                else{
                    cout << "[" << grid[i][j] << "]"; 
                }
                cursor_edge = true;
            }
            else{
                if(!cursor_edge && !(plr.canAttack(i, j) && grid[i][j] != plr.getToken() && grid[i][j] != bkgrnd)){
                    cout << " ";
                }
                else if(!cursor_edge && plr.canAttack(i, j) && grid[i][j] != plr.getToken() && grid[i][j] != bkgrnd){
                    cout << "!";
                }
                else{
                    cursor_edge = false;
                }
                cout << grid[i][j];
            }
        }
        if(!cursor_edge){
            cout << " ";
        }
        setTextColor(wall_color);
        cout << "#\n";
        cursor_edge = false;
    }
    
    setTextColor(wall_color);
    for(int i = 0; i < 2 * (grid[0].size() + 2) - 1; i++) {
        cout << "#";
    }
    printMonstBlock();
    printPlrBlock();
}

void Map::printMonstBlock(){
    int mnstr_index = findMonster(crsr.getRow(), crsr.getCol());
    cout << string(200, ' ');
    setCursorPos(0, 1 + grid.size());
    cout << endl << activity << endl;
    block_width = 15;
    cout << "+" << string(block_width, '-') << "+" << endl;
    if(mnstr_index >=0){
    //name
        cout << "|" << setw(block_width) << mnstr[mnstr_index].getName() << "|" << endl;
    //hp
        string m_hp = to_string(mnstr[mnstr_index].getCurHp()) + "/" + to_string(mnstr[mnstr_index].getMaxHp());
        cout << "|" << setw(block_width) << m_hp << "|" << endl;
    //attack
        string m_attack = to_string(mnstr[mnstr_index].getAtk()) + " ATK";
        cout << "|" << setw(block_width) << m_attack << "|" << endl;
    //move, range
        string m_threat_range = to_string(mnstr[mnstr_index].getMove()) + " M, " + to_string(mnstr[mnstr_index].getAtkRange()) + " R";
        cout << "|" << setw(block_width) << m_threat_range << "|" << endl;
    }
    else{
        for(int i = 0; i < 4; i++){
            cout << "|" << setw(block_width + 1) << "|" << endl;
        }
    }
    cout << "+" << string(block_width, '-') << "+" << endl;
    if(mnstr_index >= 0)
        cout << mnstr[mnstr_index].getDesc();
    else
        cout << string(200, ' ');
}

void Map::printPlrBlock(){
    int y_pos = 3 + grid.size();
    int x_pos = 2 + block_width;
    setCursorPos(x_pos, y_pos);
    cout << "+" << string(block_width, '-') << "+";
//name
    y_pos++;
    setCursorPos(x_pos, y_pos);
    cout << "|" << setw(block_width) << "YOU" << "|";
//hp
    y_pos++;
    setCursorPos(x_pos, y_pos);
    string hp = to_string(plr.getCurHp()) + "/" + to_string(plr.getMaxHp());
    cout << "|" << setw(block_width) << hp << "|";
//atk
    y_pos++;
    setCursorPos(x_pos, y_pos);
    string attack = to_string(plr.getAtk()) + " ATK";
    cout << "|" << setw(block_width) << attack << "|";
//move, range
    y_pos++;
    setCursorPos(x_pos, y_pos);
    string range = to_string(plr.getMove()) + " M, " + to_string(plr.getAtkRange()) + " R";
    cout << "|" << setw(block_width) << range << "|";
//closing
    y_pos++;
    setCursorPos(x_pos,y_pos);
    cout << "+" << string(block_width, '-') << "+";
}