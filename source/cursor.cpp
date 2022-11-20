#include "..\headers\cursor.h"
#include <iostream>

bool Cursor::isValid(int row, int col, const int max_row, int max_col) const{
    return ((row >= 0 && row < max_row) && (col >= 0 && col < max_col));
}

Cursor::Cursor(){
    row_coord = 0;
    col_coord = 0;
}

void Cursor::moveCursor(int row, int col, const int max_row, const int max_col){
    if(isValid(row, col, max_row, max_col)){
        row_coord = row;
        col_coord = col;
    }
}

int Cursor::getCol() const{
    return col_coord;
}

int Cursor::getRow() const{
    return row_coord; 
}