#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>
#include "..\headers\map.h"
#include "..\headers\consumable.h"
#include "..\headers\consInteraction.h"
#include "windows.h"

//.ignore(std::numeric_limits<std::streamsize>::max()) FROM:
//https://stackoverflow.com/questions/477408/ifstream-end-of-line-and-move-to-next-line

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
    //don't check if dead monsters are overlapping with anything
    if(mnstr[i].isDead()) {
        return false;
    }
    //check overlap with the player
    if(mnstr[i].getCol() == plr.getCol() && mnstr[i].getRow() == plr.getRow()) {
        return true;
    }
    //check overlap with every other monster
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
        if(dir == 0 && mnstr.getCol() - 1 >= 0){ //left
            mnstr.setCoords(mnstr.getRow(), mnstr.getCol() - 1);
            return;
        } else if(dir == 0) { //wall
            dir++;
        }
        if(dir == 1 && mnstr.getCol() + 1 < grid[0].size()){ //right
            mnstr.setCoords(mnstr.getRow(), mnstr.getCol() + 1);
            return;
        } else if(dir == 1){ //wall
            dir--; //go back through the loop to hit the prior statement
            flag = true;
        }

        if(dir == 2 && mnstr.getRow() - 1 >= 0){ //up
            mnstr.setCoords(mnstr.getRow() - 1, mnstr.getCol());
            return;
        } else if(dir == 2){ //wall
            dir++;
        }
        if(dir == 3 && mnstr.getRow() + 1 < grid.size()){ //down
            mnstr.setCoords(mnstr.getRow() + 1, mnstr.getCol());
            return;
        } else if(dir == 3){ //wall
            dir--; //go back through loop to hit prior statement
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

    fstream data;
    data.open(DATA_FILE, fstream::in);
    // theme = "Fire";
    phase = 0;
    active_mons_coord[0] = -1;
    active_mons_coord[1] = -1;

//map data formatting:
    //[reward_type];[height];[width];[theme];[num_monsters];
    //read map data
    reward_type = readNum(data);
    int rows = readNum(data);
    int cols = readNum(data);
    theme = readTheme(data);
    int num_monsters = readNum(data);
    getThemeColor();

    //read data for all the monsters
    for(int i = 0; i < num_monsters; i++) {
        //see citation at top of file
        data.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip to next line of file

        mnstr.push_back(readMonster(data, rows, cols));
    }

    data.close();

    //place the player on the map
    plr.setCoords(rand() % rows, rand() % cols);

    //create our char grid to print
    grid.clear();
    grid.assign(rows, vector<char>());
    for(int i = 0; i < rows; i++){
        grid[i].assign(cols, bkgrnd);
    }
    grid[plr.getRow()][plr.getCol()] = plr.getToken();

    //place the player cursor on top of the player
    crsr.moveCursor(plr.getRow(), plr.getCol(), grid.size(), grid[0].size());

    //make sure no monsters overlap with any other monsters or the player
    bool overlap = true;
    while(overlap) {
        //keep looping until we do not find any overlaps
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

void Map::createPlayer() {
    fstream data;
    data.open(DATA_FILE, fstream::in);

    //keep the save_id for calling future queries
    save_id = readNum(data);

    //read player data
    int max_hp = readNum(data);
    int cur_hp = readNum(data);
    int str = readNum(data);
    int spd = readNum(data);

    //see citation at top of the file
    data.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip to next line in file

    //read weapon data
    string wep_name = readEntry(data);
    string weapon_class = readEntry(data);
    int wep_atk = readNum(data);
    int wep_var = readNum(data);
    int atk_range = readNum(data);
    float wep_acc = stof(readEntry(data));
    float wep_decay = stof(readEntry(data));

    //see citation at the top of the file
    data.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip to next line of file

    //read enchant data
    string ench_name = readEntry(data);
    float ench_atk = stof(readEntry(data));
    float ench_var = stof(readEntry(data));
    float ench_acc = stof(readEntry(data));
    float ench_decay = stof(readEntry(data));


    //create a weapon out of the weapon and enchant data
    string weapon_name = ench_name + " " + wep_name;
    int atk = wep_atk * ench_atk;
    int atk_var = wep_var * ench_var;
    float acc = wep_acc * ench_acc;
    if(acc > 1.000) { //ensure accuracy is less than 1
        acc = 1.000;
    }
    float acc_decay = wep_decay * ench_decay;

    plr = Player(0, 0, weapon_name, weapon_class, spd, max_hp, atk_range, atk, atk_var, acc, acc_decay, str);
    plr.setCurHP(cur_hp);

    createInventory(data);

    data.close();
}

void Map::createInventory(fstream& data) {
    int num_items = readNum(data);
    if(num_items <= 0) { //no items, no inventory
        return;
    }
    for(int i = 0; i < num_items; i++) {
        //see citation at the top of the file
        data.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip to the next line in file
        plr.addItem(readConsumable(data));
    }
}

void Map::mainMenu() {
    bool loaded_file = false;
    while(!loaded_file) {
        system("python db_interface\\db_interface.py 1"); //list of save files from db
        loaded_file = saveList();
    }

    system(("python db_interface\\db_interface.py 5 " + to_string(save_id)).c_str()); //retrieve the save file to be used
    createPlayer();

    system(("python db_interface\\db_interface.py 6 " + to_string(save_id) + " 1").c_str()); //retrieve the map data from the save
    createNewMap();
}

void Map::unlockList() {
    clearScreen();
    printTitle();
    fstream data;
    data.open(DATA_FILE, fstream::in);

    int num_unlocked = readNum(data);
    vector<int> id_list;

    //read every character line in the file
    cout << "Unlocked characters:" << endl;
    for(int i = 0; i < num_unlocked; i++) {
        // see citation at the top of the file
        data.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip to the next line in file
        id_list.push_back(readNum(data));
        cout << '[' << i+1 << "] " << readCharacter(data) << "\n" << endl;
    }

    data.close();

    //check the number keys for the user selection
    int index;
    bool input = false;
    while(!input) {
        Sleep(40);
        for(int i = 1; i <= num_unlocked; i++) {
            if(GetAsyncKeyState('0' + i) & 0x8000) { //ASCII values for numbers are in numeric order, '0' + 1 == '1'
                index = i-1;
                input = true;
            }
        }
    }

    //id_list[index] is the character id we are using
    system(("python db_interface\\db_interface.py 3 " + to_string(id_list[index])).c_str()); //create a new save file using the selected character as a template
}

bool Map::saveList() {
    clearScreen();
    printTitle();
    fstream data;
    data.open(DATA_FILE, fstream::in);

    int num_saves = readNum(data);
    vector<int> id_list;
    vector<int> level_list;

    //display save info
    cout << "Save files:" << endl;
    for(int i = 0; i < 9; i++) {
        //run til eof
        if(i < num_saves) {
            //see citation at the top of the file
            data.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip to next line of file
            //id, name, max_hp, cur_hp, str, spd
            id_list.push_back(readNum(data));
            cout << "[" << i+1 << "] " << readSave(data);
            readEntry(data);
            readEntry(data); //burn wep_id and enchant_id so we can get to level
            level_list.push_back(readNum(data));
            cout << ". On level " << level_list[i] << ".\n" << endl;
        } else {
            cout << '[' << i+1 << "] NEW FILE" << '\n' << endl;
        }
    }
    cout << "Options:" << endl;
    cout << "[0] DELETE A SAVE";

    data.close();

    //read number keys
    int index;
    bool input = false;
    while(!input) {
        Sleep(40);
        for(int i = 1; i <= 9; i++) {
            if(GetAsyncKeyState('0' + i) & 0x8000) { //select a save
                index = i-1;
                input = true;
            }
        }
        if(GetAsyncKeyState('0') & 0x8000) {
            deleteSave();
            return false;
        }
    }

    if(index < num_saves) {
        //select existing save
        save_id = id_list[index];
        level = level_list[index];
        return true;
    } else {
        //create new game
        system("python db_interface\\db_interface.py 2"); //retrieve the list of unlocked characters
        unlockList();
        return false;
    }
}

void Map::deleteSave() {
    clearScreen();
    printTitle();
    fstream data;
    data.open(DATA_FILE, fstream::in);

    vector<int> id_list;
    int num_saves = readNum(data);

    //display save info
    cout << "Delete a save:" << endl;
    for(int i = 0; i < 9; i++) {
        if(i < num_saves) {
            //see citation at the top of the file
            data.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip to the next line in file
            id_list.push_back(readNum(data));
            cout << "[" << i+1 << "] " << readSave(data);
            readEntry(data); //burn wep_id and ench_id to get to level
            readEntry(data);
            cout << ". On level " << readEntry(data) << ".\n" << endl;;
        } else {
            cout << "[" << i+1 << "] NEW FILE\n" << endl;
        }
    }

    cout << "[0] CANCEL";

    data.close();

    Sleep(50); //wait so we don't get junk leftover inputs from calling to this function
               //otherwise we will immediately cancel this function call and cannot delete saves

    //read user input
    bool input = false;
    int index;
    while(!input) {
        Sleep(40);
        for(int i = 1; i <= num_saves; i++) {
            if(GetAsyncKeyState('0' + i) & 0x8000) { //character is selected
                index = i-1;
                input = true;
            }
        }
        if(GetAsyncKeyState('0') & 0x8000) { //cancel
            index = -1;
            input = true;
        }
    }

    if(index < 0) {
        return;
    }

    system(("python db_interface\\db_interface.py 4 " + to_string(id_list[index])).c_str()); //delete save file
}

void Map::generateReward(){
    // int reward = rand() % 3;
    // int reward = 1;
    system(("python db_interface\\db_interface.py 7 " + to_string(save_id) + " " + to_string(reward_type)).c_str()); //call the basic generate reward script (prints weapon or item to file)

    if(reward_type == 1){ // new weapon
        generateWeapon();
    }
    else if(reward_type == 2){ //new item
        generateItem();
    }
    else if (reward_type == 3) { //stat increase
        generateStatBuff();
    } else { //new character (called after boss fights)
        generateCharReward();
        plr.setCurHP(plr.getMaxHp()); //heal to full
    }
}

void Map:: generateWeapon() {
    fstream data;
    data.open(DATA_FILE, fstream::in);

    //read weapon stats
        //id;name;class;atk;atk_var;range;acc;acc_decay
    int wep_id = readNum(data);
    string wep_name = readEntry(data);
    string weapon_class = readEntry(data);
    int wep_atk = readNum(data);
    int wep_var = readNum(data);
    int atk_range = readNum(data);
    float wep_acc = stof(readEntry(data));
    float wep_decay = stof(readEntry(data));

    //see citation at the top of the file
    data.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip to the next line in file

    //read enchant stats
        //id;name;atk;atk_var;acc;acc_decay
    int ench_id = readNum(data);
    string ench_name = readEntry(data);
    float ench_atk = stof(readEntry(data));
    float ench_var = stof(readEntry(data));
    float ench_acc = stof(readEntry(data));
    float ench_decay = stof(readEntry(data));

    data.close();

    //create the weapon using the enchant and weapon stats
    string weapon_name = ench_name + " " + wep_name;
    int atk = wep_atk * ench_atk;
    int atk_var = wep_var * ench_var;
    float acc = wep_acc * ench_acc;
    float acc_decay = wep_decay * ench_decay;

    //accuracy over 1.00 doesn't make sense
    if(acc > 1.000) {
        acc = 1.000;
    }

    //prompt user to swap weapons
    printWeaponBlock(weapon_name, weapon_class, atk_range, atk, atk_var, acc, acc_decay);
    cout << "Do you wish to take this weapon and replace your current weapon with it? (Y/N)" << endl;
    
    bool input = false;
    while(!input){
        Sleep(40);
        if((GetAsyncKeyState('Y') & 0x8000)){
            plr.tradeWeapons(weapon_name, weapon_class, atk, atk_range, atk_var, acc, acc_decay);
            input = true;
            //update save's weapon_id and enchant_id
            system(("python db_interface\\db_interface.py 11 " + to_string(save_id) + " " + to_string(wep_id) + " " + to_string(ench_id)).c_str());
        }
        else if((GetAsyncKeyState('N') & 0x8000)){
            input = true;
        }
    }
}

void Map::generateItem() {
    fstream data;
    data.open(DATA_FILE, fstream::in);

    //hold the item to add
    Consumable item(readConsumable(data));

    data.close();

    clearScreen();
    bool item_state = plr.addItem(item); //if this is false, the user's inventory is full and we need to swap out an item
    bool input = false;
    cout << "You received:\n" << item.getStats() << endl;

    Sleep(1500);
    if(!item_state) {
        //swap an item
        cout << "Which item would you like to replace?" << endl;
        cout << plr.getInventoryList() << "\n[0] keep current items" << endl;
        while(!input){
            Sleep(40);
            for(int i = 1; i < 10; i++) {
                if(GetAsyncKeyState('0' + i) & 0x8000) {
                    //find and replace the item in inventory table
                    system(("python db_interface\\db_interface.py 9 " + to_string(save_id) + " " + to_string(plr.getItemId(i)) + " " + to_string(item.getId())).c_str());
                    plr.replaceItem(item, i);
                    input = true;
                }
            }
            if(GetAsyncKeyState('0') & 0x8000) {
                input = true;
            }
        }
    } else {
        //add item_id to inventory table
        cout << "Press space to continue.";
        while(!input) {
            Sleep(40);
            if(GetAsyncKeyState(' ') & 0x8000)
                input = true;
        }
    }
}

void Map::generateStatBuff(){
    //+5 hp
    //+2 str
    //+1 move (I'm a little shaky on this, I think any point of movement is way stronger than any point anywhere else)
    printStatBuffBlock();
    bool input = false;
    while(!input){
        Sleep(40);
        if(GetAsyncKeyState('1') & 0x8000){
            plr.levelUp("max_hp", 5);
            input = true;
        }
        else if(GetAsyncKeyState('2') & 0x8000){
            plr.levelUp("strength", 2);
            input = true;
        }
        else if(GetAsyncKeyState('3') & 0x8000){
            plr.levelUp("speed", 1);
            input = true;
        }
    }
}

void Map::generateCharReward() {
    clearScreen();
    cout << "New character unlocked!\nThey will be available for your next run.\n\n";
    
    bool input = false;
    cout << "Press space to continue.";
    while(!input) {
        Sleep(40);
        if(GetAsyncKeyState(' ') & 0x8000)
            input = true;
    }
}

void Map::printMonstBlock(){
    int mnstr_index = findMonster(crsr.getRow(), crsr.getCol()); //check if/which monster the cursor is on
    cout << string(2000, ' '); //clear the monster description line
    setCursorPos(0, 1 + grid.size()); //move to the end of the grid
    cout << endl << activity << endl; //activity line
    block_width = 15;
    cout << "+" << string(block_width, '-') << "+" << endl; //top of box
    if(mnstr_index >=0){ //there is a monster selected
    //name
        cout << "|" << setw(block_width) << mnstr[mnstr_index].getName() << "|" << endl;
    //hp
        string m_hp = to_string(mnstr[mnstr_index].getCurHp()) + "/" + to_string(mnstr[mnstr_index].getMaxHp());
        cout << "|" << setw(block_width) << m_hp << "|" << endl;
    //attack
        string m_attack = to_string(mnstr[mnstr_index].getAtk() + mnstr[mnstr_index].getStr()) + " ATK";
        cout << "|" << setw(block_width) << m_attack << "|" << endl;
    //move, range
        string m_threat_range = to_string(mnstr[mnstr_index].getMove()) + " M, " + to_string(mnstr[mnstr_index].getAtkRange()) + " R";
        cout << "|" << setw(block_width) << m_threat_range << "|" << endl;
    }
    else{
        for(int i = 0; i < 4; i++){ //there is no monster selected
            cout << "|" << setw(block_width + 1) << "|" << endl;
        }
    }
    cout << "+" << string(block_width, '-') << "+" << endl; //bottom of box
    
    //monster description
    if(mnstr_index >= 0)
        cout << mnstr[mnstr_index].getDesc();
    else
        cout << string(200, ' ');
}

void Map::printPlrBlock(int y_pos, int x_pos){
    setCursorPos(x_pos, y_pos);
    cout << "+" << string(block_width, '-') << "+";
//name
    y_pos++;
    setCursorPos(x_pos, y_pos);
    cout << "|" << setw(block_width) << "JERRY" << "|";
//hp
    y_pos++;
    setCursorPos(x_pos, y_pos);
    string hp = to_string(plr.getCurHp()) + "/" + to_string(plr.getMaxHp());
    cout << "|" << setw(block_width) << hp << "|";
//atk
    y_pos++;
    setCursorPos(x_pos, y_pos);
    string attack = to_string(plr.getStr()) + " STR, " + to_string(plr.getAtk()) + " ATK";
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

void Map::printWeaponBlock(string weapon_name, string weapon_class, int atk_range, int atk, int atk_var, float acc, float acc_decay){
    clearScreen();
    setTextColor(15);
    cout << "You found a new weapon!" << endl;
    //top of box
    cout << "+" << string(2 * block_width, '-') << "++" << string(2 * block_width, '-') << "+" << endl;
    //current, new
    cout << "|" << setw(2 * block_width) << "CURRENT WEAPON" << "||" << setw(2 * block_width) << "NEW WEAPON" << "|" << endl;   
    //weapon names
    cout << "|" << setw(2 * block_width) << plr.getWeaponName() << "||" << setw(2 * block_width) << weapon_name << "|" << endl;
    //weapon class (melee/ranged)
    cout << "|" << setw(2 * block_width) << plr.getWeaponClass() << "||" << setw(2 * block_width) << weapon_class << "|" << endl;
    //attack values
    string attack = "Attack: " + to_string(plr.getAtk());
    cout << "|" << setw(2 * block_width) << attack << "||" << setw(2 * block_width) << "Attack: " + to_string(atk) << "|" << endl;
    //range
    string range = "Range: " + to_string(plr.getAtkRange());
    cout << "|" << setw(2 * block_width) << range << "||" << setw(2 * block_width) << "Range: " + to_string(atk_range) << "|" << endl;
    //attack variance
    string attack_var = "Attack Var: " + to_string(plr.getAtkVar());
    cout << "|" << setw(2 * block_width) << attack_var << "||" << setw(2 * block_width) << "Attack Var: " + to_string(atk_var) << "|" << endl;
    //accuracy
    string accuracy = "Accuracy: " + to_string(plr.getAcc()).substr(0, 4); // substr for rounding to two decimal places
    cout << "|" << setw(2 * block_width) << accuracy << "||" << setw(2 * block_width) << "Accuracy: " + to_string(acc).substr(0,4) << "|" << endl;
    //accuracy rate of decay
    string accuracy_decay = "Decay: " + to_string(plr.getAccRate()).substr(0, 4); // substr for rounding to two decimal places
    cout << "|" << setw(2 * block_width) << accuracy_decay << "||" << setw(2 * block_width) << "Decay: " + to_string(acc_decay).substr(0,4) << "|" << endl;
    cout << "+" << string(2 * block_width, '-') << "++" << string(2 * block_width, '-') << "+" << endl;
}

void Map::printStatBuffBlock(){
    clearScreen();
    setTextColor(15);
    cout << "Choose a stat to increase: " << endl;
    cout << "\t[1] - Increase maximum hp by 5" << endl;
    cout << "\t[2] - Increase strength by 2" << endl;
    cout << "\t[3] - Increase movement by 1" << endl;
    printPlrBlock(4, 0);
}

void Map::printPotionBuffBlock(){
    //clear monster description
    cout << string(200, ' ');
    //move to end of grid
    setCursorPos(0, 1 + grid.size());
    //activity line
    cout << endl << activity << endl;
    //current buffs affecting the player
    vector<string> buffs = plr.getConsumeEffects();
    cout << "+" << string(block_width, '-') << "+" << endl;
    if(!plr.isBuffed()) {
        //no buffs, print "NO BUFFS"
        cout << "|" << setw(block_width) << buffs[0] << "|";
        for(int i = 0; i < 4; i++) {
            cout << "|" << setw(block_width) << " " << "|" << endl;
        }
    }
    else{
        //print all the buffs
        cout << "|" << setw(block_width) << "BUFFS" << "|" << endl;
        for(int i = 0; i < 3; i++){
            cout << "|" << setw(block_width) << buffs[i] << "|" << endl;
        }
    }
    cout << "+" << string(block_width, '-') << "+" << endl;
}

int Map::getThemeColor() {
    if(theme == "dungeon")
    //grey
        wall_color = 8;
    else if(theme == "forest")
    //green
        wall_color = 2;
    else if(theme == "cave")
    //red
        wall_color = 4;
    else
    //purple
        wall_color = 5;

    return wall_color;
}

string Map::readTheme(fstream& data) {
    //read the theme and set the member
    theme = readEntry(data);
    return theme;
}

int Map::readNum(fstream& data) {
    //convert an entry to an int
    return stoi(readEntry(data));
}

string Map::readEntry(fstream& data) {
    //parse by `;`
    string temp;
    getline(data, temp, ';');
    return temp;
}

Monster Map::readMonster(fstream& data, int rows, int cols) {
    //read the monster stats
    string name = readEntry(data);
    string desc = readEntry(data);
    int sight = readNum(data);
    int atk = readNum(data);
    int atk_var = readNum(data);
    int atk_range = readNum(data);
    int move = readNum(data);
    float acc = stof(readEntry(data));
    float acc_decay = stof(readEntry(data));
    char token = readEntry(data)[0];
    int hp = readNum(data);
    int d_beh = readNum(data);
    int s_beh = readNum(data);
    int p_beh = readNum(data);

    //put the stats into a monster
    return Monster(name, desc, rand() % cols, rand() % rows, token, sight, p_beh, s_beh, d_beh, move, hp, atk_range, atk, atk_var, acc, acc_decay);
//file
    //name;description;sight_range;atk_strength;atk_var;atk_range;move;acc;acc_decay;symbol;max_hp;default_beh;blind_beh;provoked_beh;

//constructor
    //name, desc, rand() % cols, rand() % rows, 
    //     symbol, sight, behaveP, behaveS, behaveD, spd, max_health, range, damage, damage_var, 
    //     accuracy, accuracy_decay
}

Consumable Map::readConsumable(fstream& data) {
    //read the consumable stats
    int id = readNum(data);
    string name = readEntry(data);
    string desc = readEntry(data);
    int heal = readNum(data);
    int spd = readNum(data);
    int str = readNum(data);
    int dur = readNum(data);

    //put the consumable stats into a consumable
    return Consumable(name, desc, heal, str, spd, dur, id);
}

string Map::readCharacter(fstream& data) {
    //read the relevant stats from the file
    string character_string = readEntry(data) + ": ";
    character_string += readEntry(data) + " hp, ";
    character_string += readEntry(data) + " str, ";
    character_string += readEntry(data) + " spd. ";
    character_string += "Starts with a rusty " + readEntry(data);
    
    //return the stats as a string
    return character_string;
}

string Map::readSave(fstream& data) {
    //read the relevant stats
    //name, max_hp, cur_hp, str, spd
    string save_string = readEntry(data) + ": ";
    string max_hp = readEntry(data);
    save_string += readEntry(data) + '/' + max_hp + " hp, ";
    save_string += readEntry(data) + " str, ";
    save_string += readEntry(data) + " spd";

    //return the stats as a string
    return save_string;
}

Map::Map(){
    game_over = false;
    level = 0;
    bkgrnd = '+';
    block_width = 20;

    mainMenu();
}

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
    bool new_map_created = false;
    
    //phase 0, player move; phase 1, player attack, monsters
    if(skip){ //skip player action
        if(phase == 0){ //skip move
            phase++;
        }
        else if(phase == 1){ //skip attack
            //monsters still act
            handleMonsters();
            if(plr.getCurHp() <= 0) {
                //you lose at 0 hp
                activity = "YOU DIED! GAME OVER!";
                printGrid();
                game_over = true;
                system(("python db_interface\\db_interface.py 4 " + to_string(save_id)).c_str()); //delete save file
            }
            phase = 0;
        }
    }
    else if(phase == 0){ //player move
        if(movePlayer()){
            phase++;
        }
    }
    else if (phase == 1){ //player attack
        if(playerAttack()){
            printGrid();
            Sleep(1500);
            //monsters act
            handleMonsters();
            if(plr.getCurHp() <= 0){
                //you lose at 0 hp
                activity = "YOU DIED! GAME OVER!";
                printGrid();
                game_over = true;
                system(("python db_interface\\db_interface.py 4 " + to_string(save_id)).c_str()); //delete save file
            }
            phase = 0;
        }
        //game state
        if(mnstr.empty()) {
            //create a new room and a reward after completing a room
            activity = "ALL MONSTERS VANQUISHED! (Press Space to Continue)";
            printGrid();
            level++;

            while(!(GetAsyncKeyState(' ') & 0x8000)){
                Sleep(40);
            }

            generateReward();            
            
            plr.consumeAgain(); //clear any buffs (prevents permanent stat buffs via reloading saves)

            plr.floorHeal();
            //update save file
            //"<max_hp>,<cur_hp>,<str>,<spd>"
            system(("python db_interface\\db_interface.py 8 " + to_string(save_id) + " " + to_string(plr.getMaxHp()) + "," + to_string(plr.getCurHp()) + "," + to_string(plr.getStr()) + "," + to_string(plr.getMove())).c_str());
            
            //delete all used items from the database inventory
            for(int i = 0; i < used_items.size(); i++) {
                system(("python db_interface\\db_interface.py 10 " + to_string(save_id) + " " + to_string(used_items[i])).c_str());
            }
            used_items.clear();

            //get a new map
            system(("python db_interface\\db_interface.py 6 " + to_string(save_id) + " 0").c_str());

            createNewMap();
            new_map_created = true;
        }
    }
    //activity text
    if(phase == 0 && !new_map_created && !game_over){
        activity = "Move when ready.";
    }
    if(phase == 1 && !game_over){
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

    grid[plr.getRow()][plr.getCol()] = bkgrnd; //old location
    if(plr.updateCoords(crsr.getRow(), crsr.getCol())) { //attempt to move
        hasMoved = true;
    }
    grid[plr.getRow()][plr.getCol()] = plr.getToken(); //new location

    return hasMoved;
}

bool Map::playerAttack(){
    int mnstr_index = findMonster(crsr.getRow(), crsr.getCol()); //the monster we want to attack
    if(plr.canAttack(crsr.getRow(), crsr.getCol()) && mnstr_index >= 0){ //in range
        //roll damage and damage the monster
        int damage = plr.rollAttack(crsr.getRow(), crsr.getCol());
        mnstr[mnstr_index].receiveAttack(damage);
        if(damage < 0) { //negative damage, miss
            activity = "Your attack missed!";
        } else if(mnstr[mnstr_index].isDead()){ //killed the monster
            activity = "You killed the " + mnstr[mnstr_index].getName() + "!";
            grid[mnstr[mnstr_index].getRow()][mnstr[mnstr_index].getCol()] = bkgrnd;
            mnstr.erase(mnstr.begin() + mnstr_index);
        }
        else{ //damage, does not kill
            if(plr.getWeaponClass() == "melee"){
                activity = "You swung your ";
            }
            else if(plr.getWeaponClass() == "ranged"){
                activity = "You aimed and fired your ";
            }
            activity += (plr.getWeaponName() + " and dealt " + to_string(damage) + " damage to the " + mnstr[mnstr_index].getName() + "!"); 
        }
        return true; //attack happened
    }
    else{
        return false; //attack did not happen
    }
}

void Map::handleMonsters(){
    //tick down potion duration
    plr.consumeTimer();

    //have each monster move and attack
    bool held = false;
    for(int i = 0; i < mnstr.size(); i++){
        if(!mnstr[i].isDead()) {
            moveMonster(i);
            //highlight the current monster
            active_mons_coord[0] = mnstr[i].getRow();
            active_mons_coord[1] = mnstr[i].getCol();
            printGrid();
            Sleep(500);
            monsterAttack(i);
            printGrid();
            Sleep(600);
        }
    }
    active_mons_coord[0] = -1;
    active_mons_coord[1] = -1;
}

void Map::moveMonster(int index) {
    //determine where to move to based on behavior
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
    grid[mnstr[index].getRow()][mnstr[index].getCol()] = bkgrnd; //old location
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
    grid[mnstr[index].getRow()][mnstr[index].getCol()] = mnstr[index].getToken(); //new location
}

void Map::monsterAttack(int index) {
    //attack if the player is within the monster's range
    if(mnstr[index].canAttack(plr.getRow(), plr.getCol())) {
        //roll damage against the player
        int dmg = mnstr[index].rollAttack(plr.getRow(), plr.getCol());
        plr.receiveAttack(dmg);
        
        if(dmg < 0) { //negative, miss
            activity = mnstr[index].getName() + " tried to attack you but missed!";
        } else if(dmg == 0) { //0 damage
            activity = "You deflected " + mnstr[index].getName() + "'s attack!";
        } else { //damage dealt
            activity = mnstr[index].getName() + " attacked you for " + to_string(dmg) + " damage!";
        }
    }
}

void Map::inventorySelection(){
    clearScreen();
    //print inventory
    cout << "Select an item to use: " << endl;
    cout << plr.getInventoryList() << endl;
    cout << "[0] Return to game" << endl;
    
    //read user input
    bool input = false;
    while(!input){
        for(int i = 1; i <= plr.getInvSize(); i++){
            if(GetAsyncKeyState('0' + i) & 0x8000){
                //keep a record of used items in order to update database at end of floor
                //if we were to delete items here, then quitting out in the middle of a floor would actively harm the player
                //they would permanently lose any items used
                used_items.push_back(plr.getItemId(i));
                if(plr.consume(i)){
                    input = true;
                }
            }
        }
        if(GetAsyncKeyState('0') & 0x8000){
            input = true; //0 exits back without consuming any items
        }
    }
    clearScreen();
    printGrid();
}

void Map::gameOver() {
    game_over = true;
}

int Map::getNumRows() const {
    return grid.size();
}

int Map::getNumCols() const {
    return grid[0].size();
}

bool Map::getGameOver() const{
    return game_over;
}

void Map::printGrid() {
    setCursorPos(0, 0);

    //walls
    setTextColor(wall_color);
    for(int i = 0; i < 2 * (grid[0].size() + 2) - 1; i++) {
        cout << "#";
    }
    
    cout << endl;

    bool cursor_edge = false;
    for(int i = 0; i < grid.size(); i++){
        //wall
        setTextColor(wall_color);
        cout << "#";
        for(int j = 0; j < grid[0].size(); j++){
            changeColor(i, j);
            if(i == crsr.getRow() && j == crsr.getCol()) { //cursor
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
                cursor_edge = true; //since cursor prints after a map tile, don't print a space after this grid index
            }
            else{
                if(!cursor_edge && !(plr.canAttack(i, j) && grid[i][j] != plr.getToken() && grid[i][j] != bkgrnd)){ //monster out of range
                    cout << " ";
                }
                else if(!cursor_edge && plr.canAttack(i, j) && grid[i][j] != plr.getToken() && grid[i][j] != bkgrnd){ //monster within range
                    cout << "!";
                }
                else{
                    cursor_edge = false;
                }
                cout << grid[i][j];
            }
        }
        if(!cursor_edge){ //print a space if not immediately after the cursor
            cout << " ";
        }
        //wall
        setTextColor(wall_color);
        cout << "#\n";
        cursor_edge = false;
    }
    
    //wall
    setTextColor(wall_color);
    for(int i = 0; i < 2 * (grid[0].size() + 2) - 1; i++) {
        cout << "#";
    }
    if(crsr.getRow() == plr.getRow() && crsr.getCol() == plr.getCol()){
        printPotionBuffBlock(); //print potion if on the player
    }
    else{
        printMonstBlock(); //otherwise, try to print monster (if possible)
    }
    /*
    +__ str\n
    +__ move\n
    __ turns\n

    No current buffs\n

    substr(\n) <- put in block
    */
    int y_pos = 3 + grid.size();
    int x_pos = 2 + block_width;
    printPlrBlock(y_pos, x_pos);
}

void Map::printTitle() const{
    cout << "        | |                    ( )      / __ \\                | |" << endl;  
    cout << "        | | ___ _ __ _ __ _   _|/ ___  | |  | |_   _  ___  ___| |_" << endl; 
    cout << "    _   | |/ _ \\ '__| '__| | | | / __| | |  | | | | |/ _ \\/ __| __|" << endl;
    cout << "   | |__| |  __/ |  | |  | |_| | \\__ \\ | |__| | |_| |  __/\\__ \\ |_ "<< endl;
    cout << "    \\____/ \\___|_|  |_|   \\__, | |___/  \\___\\_ \\__,_|\\___||___/\\__|"<< endl;
    cout << "                           __/ |                                   "<< endl;
    cout << "                          |___/\n\n" << endl; 
}