#ifndef CELL_H
#define CELL_H

#include <vector>

class Cell {
    private:
        int x_pos; //up down, first bracket
        int z_pos; //left right, second bracket
        bool visited; //might not use this in place of a 
    public:
        Cell(int, int);
        void SetVisited(bool);
        bool GetVisited() const;
        bool operator== (const Cell& RightSide) const; //Take a rightside Cell object as a const. Take its memory address. Only check x_pos & z_pos
        int GetX_pos() const;
        int GetZ_pos() const;
        void testPrint() const; //test print function
        void testPrintVisited() const;
};

#endif //CELL_H