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
        vector<vector<int>> color_grid;
        vector<vector<int>> color_bkgrnd;
        char bkgrnd;
        Player plr;
        vector<Monster> mnstr;
        Cursor crsr;
        void changeColor(int row_cord, int col_cord) const;
        bool checkOverlap(int i); //returns true if the monster overlaps with a) any prior monster or b) the player
        void handleOverlap(Monster& mnstr);
    public:
        Map();
        Map(int rows, int cols, char back, int num_monst);
        
        void setNumRowsCols(int row, int col);
        void moveCursor(char dir);
        bool movePlayer();
        void moveMonsters();

        int getNumRows() const;
        int getNumCols() const;

        void printGrid();
};
#endif