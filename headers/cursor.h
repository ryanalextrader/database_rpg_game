#ifndef CURSOR_H
#define CURSOR_H

class Cursor{
    private:
        int row_coord;
        int col_coord;
        bool isValid(int row, int col, const int max_rows, const int max_cols) const;
    public:
        Cursor();
        void moveCursor(int row, int col, const int max_row, const int max_col);
        int getCol() const;
        int getRow() const;
};
#endif