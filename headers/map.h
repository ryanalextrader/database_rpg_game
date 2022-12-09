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

const string DATA_FILE = ".\\data\\runtime_data.txt";

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

        void changeColor(int row_cord, int col_cord) const;
        bool checkOverlap(int i); //returns true if the monster overlaps with a) any prior monster or b) the player
        void handleOverlap(Monster& mnstr);
        int findMonster(int row_coord, int col_coord) const; //returns the index of the monster at certain coordinates. Returns negative if it finds nothing 
        void createNewMap();
        void createPlayer();
        void createInventory(fstream& data);
        void mainMenu();
        void unlockList();
        
        bool saveList();
        void deleteSave();

        void generateReward();
        void generateWeapon();
        void generateStatBuff();
        void generateItem();
        void generateCharReward();

        void printMonstBlock();
        void printPlrBlock(int y_pos, int x_pos);
        void printWeaponBlock(string weapon_name, string weapon_class, int atk_range, int atk, int atk_var, float acc, float acc_decay);
        void printStatBuffBlock();
        void printPotionBuffBlock();

        int getThemeColor();
        string readTheme(fstream& data);
        int readNum(fstream& data);
        string readEntry(fstream& data);
        Monster readMonster(fstream& data, int rows, int cols);
        Consumable readConsumable(fstream& data);
        string readCharacter(fstream& data);
        string readSave(fstream& data);

    public:
        Map();
        // Map(int rows, int cols, char back, int num_monst, string dungeon_theme, int wall_clr);
        
        void setNumRowsCols(int row, int col);
        void moveCursor(char dir);
        void phaseAct(bool skip = false);
        bool movePlayer();
        bool playerAttack();
        void handleMonsters();
        void moveMonster(int index);
        void monsterAttack(int index);\

        void inventorySelection();

        void gameOver();

        int getNumRows() const;
        int getNumCols() const;
        bool getGameOver() const;

        void printGrid();
};
#endif