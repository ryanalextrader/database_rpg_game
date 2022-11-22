#ifndef map_h
#define map_h

#include <vector>
#include <string>
#include ".\player.h"
#include ".\monster.h"
#include ".\cursor.h"

using namespace std;

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

        Player plr;
        vector<Monster> mnstr;
        Cursor crsr;

        void changeColor(int row_cord, int col_cord) const;
        bool checkOverlap(int i); //returns true if the monster overlaps with a) any prior monster or b) the player
        void handleOverlap(Monster& mnstr);
        int findMonster(int row_coord, int col_coord) const; //returns the index of the monster at certain coordinates. Returns negative if it finds nothing 
        void createNewMap();
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
        void monsterAttack(int index);

        void completeFloor();
        void gameOver();

        int getNumRows() const;
        int getNumCols() const;

        void printGrid();
        void printMonstBlock();
        void printPlrBlock();
};
#endif