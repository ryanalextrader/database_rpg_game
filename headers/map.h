#ifndef map_h
#define map_h

#include <vector>
#include <string>
#include <fstream>
#include ".\player.h"
#include ".\monster.h"
#include ".\cursor.h"
#include ".\consumable.h"

using namespace std;

const string DATA_FILE = ".\\data\\runtime_data.txt"; //entries in file should be separated by semicolons for parsing
                                                      //strings do not include square braces 

class Map{
    private:
        vector<vector<char>> grid;
        char bkgrnd;
        string theme;
        int wall_color;
        int phase;
        int block_width;
        string activity;
        int active_mons_coord[2];
        int level;
        bool game_over;

        vector<int> used_items;
        int reward_type;
        int save_id;

        Player plr;
        vector<Monster> mnstr;
        Cursor crsr;

        //changes the color of the tiles on the map based on what character occupies the tile
        //player, phase 0 -> blue
        //player, phase 1 -> yellow
        //monster that is moving -> purple/magenta
        //other monsters -> red
        //background tile within move range -> green
        //background -> white
        void changeColor(int row_cord, int col_cord) const;
        //ensures that monster[i] is not overlapping with the player or any other monster
        //returns false if no overlaps are found
        bool checkOverlap(int i);
        //moves a monster in a random direction to try and shove it out of an overlap
        void handleOverlap(Monster& mnstr);
        //checks the given point for a monster
        //returns the index if a monster is found
        //returns negative otherwise
        int findMonster(int row_coord, int col_coord) const;
        //reads the data file for map and monster information, then generates and populates the map
        //!!DOES NOT UPDATE THE FILE, NEITHER BEFORE NOR AFTER!!
        //EXPECTED MAP FORMAT:
        //reward_type;height;width;theme;num_monsters;
        //[monster information lines]...
        void createNewMap();
        //reads the data file for player information and fills the data into the plr member
        //!!DOES NOT UPDATE THE FILE, NEITHER BEFORE NOR AFTER!!
        void createPlayer();
        //reads the given file to create and fill and inventory for the player
        //!!DOES NOT UPDATE THE FILE!!
        //EXPECTS:
        //num_items;
        //[item information lines]...
        void createInventory(fstream& data);
        //displays the title and the list of available saves
        //allows the player to select a save, delete a save, or create a save
        void mainMenu();
        //displays a list of unlocked classes
        //creates a save based off of player selection
        //!!DOES NOT CHANGE FILE BEFORE!!
        //EXPECTS:
        //num_unlocked_characters;
        //[character information lines]...
        void unlockList();
        
        //displays a list of save files
        //loads a save based on player selection
        //returns whether or not a save was actually loaded
        //!!DOES NOT CHANGE FILE BEFORE!! 
        //EXPECTS:
        //num_saves;
        //[save info lines]...
        bool saveList();
        //displays a list of save files
        //deletes a save based on player selection
        //!!DOES NOT CHANGE FILE BEFORE!!
        //EXPECTS:
        //num_saves;
        //[save info lines]...
        void deleteSave();

        //uses reward_type to determine what reward to give the player
        void generateReward();
        //reads the file for weapon info
        //prompts the user to swap weapons or not
        //!!DOES NOT UPDATE FILE!!
        void generateWeapon();
        //prompts the user to increase a stat
        void generateStatBuff();
        //reads the file for item info
        //checks to see if the item can be added
        //either adds or item or prompts for an item swap
        //!!DOES NOT UPDATE FILE!!
        void generateItem();
        //displays that a character was unlocked
        void generateCharReward();

        //print the stats of the monster at the player's cursor
        void printMonstBlock();
        //prints the stats of the player
        void printPlrBlock(int y_pos, int x_pos);
        //prints the stats of the given weapon and the player's current weapon
        void printWeaponBlock(string weapon_name, string weapon_class, int atk_range, int atk, int atk_var, float acc, float acc_decay);
        //prints the options for a stat increase
        void printStatBuffBlock();
        //prints the current potion effects on the player, if any
        void printPotionBuffBlock();

        //gets the color for the background walls based on the room's theme
        int getThemeColor();
        //reads a string from the given file, and sets it equal to theme
        //returns theme
        string readTheme(fstream& data);
        //reads the given file for a semicolon and returns the string preceding it
        string readEntry(fstream& data);
        //reads an entry from the given file and returns it as an int
        int readNum(fstream& data);
        //reads a monster stat block from the given file and returns the generated monster
        //EXPECTED MONSTER FORMAT:
        //name;description;sight_range;atk_strength;atk_var;atk_range;move;acc;acc_decay;symbol;max_hp;default_beh;blind_beh;provoked_beh;
        Monster readMonster(fstream& data, int rows, int cols);
        //reads an item stat block from the given file and returns the generated item
        //EXPECTED ITEM FORMAT:
        //id;name;desc;heal;spd;str;dur;
        Consumable readConsumable(fstream& data);
        //reads the given file for a starting class (WARRIOR, RANGER,...) and returns it as a string
        //EXPECTED CHARACTER FORMAT:
        //class_name;hp;str;spd;weapon_name
        //RETURN FORMAT:
        //"[class_name]: [] hp, [] str, [] spd. Starts with a rusty [weapon_name]"
        string readCharacter(fstream& data);
        //reads the given file for save information and returns it as a string
        //EXPECTED SAVE FORMAT:
        //class_name;max_hp;cur_hp;str;spd;wep_id;ench_id;level;
        //RETURN FORMAT:
        //"[class_name]: [cur]/[max] hp, [] str, [] spd"
        string readSave(fstream& data);

    public:
        Map();

        //size of the map
        void setNumRowsCols(int row, int col);
        //moves the player cursor based on input
        //'W' up
        //'A' left
        //'S' down
        //'D' right
        void moveCursor(char dir);
        //handles player actions
        //phase 0: movement
        //phase 1: attack
        //skips current phase if skip is true
        //after phase 1, handle monsters
        //handle monsters even if phase 1 is skipped
        void phaseAct(bool skip = false);
        //check if the player can move to the player cursor, and move them
        //returns whether or not the player succesfully moved
        bool movePlayer();
        //check if the player can attack to the player cursor and if there is something there to attack
        //attack at the player cursor
        bool playerAttack();
        //run through the monster vector and call each monster to both move and attack
        void handleMonsters();
        //have the monster at index move based on behavior
        void moveMonster(int index);
        //have the monster at index attack if the player is within its range
        void monsterAttack(int index);

        //print the inventory list and prompt the user to select an item to use
        void inventorySelection();

        //set game_over to true
        void gameOver();

        int getNumRows() const;
        int getNumCols() const;
        bool getGameOver() const;

        //print the map and all relevant info to the console
        //relevant info: monster/stat and player blocks, activity line
        void printGrid();
        //print the game title in big funny letters
        //big funny letters from: https://patorjk.com/software/taag/#p=display&f=Graffiti&t=Type%20Something%20
        void printTitle() const;
};
#endif