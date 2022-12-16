#ifndef CURSOR_H
#define CURSOR_H

class Cursor{
    private:
        int row_coord; //row coordinate of cursor
        int col_coord; //column coordinate of cursor
        //check if a specific cursor position is valid
        //input: row - int, the row to move the cursor to
        //       col - int, the col to move the cursor to
        //       max_rows - the maximum number of rows in the map
        //       max_cols - the maximum number of columns in the map
        //output: <true> - if the position marked by row and col is a valid position
        //          or
        //        <false> - if the position marked by row and col is not a valid position 
        bool isValid(int row, int col, const int max_rows, const int max_cols) const;
    public:
        Cursor();
        //move the cursor to the position marked by row and col if it is a valid position
        //input: row - int, the row to move the cursor to
        //       col - int, the col to move the cursor to
        //       max_row - the maximum number of rows in the map
        //       max_col - the maximum number of columns in the map
        void moveCursor(int row, int col, const int max_row, const int max_col);
        //getters
        int getCol() const;
        int getRow() const;
};
#endif