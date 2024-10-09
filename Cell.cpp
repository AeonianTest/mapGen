#include <iostream>
#include "Cell.h" 

Cell::Cell(int x_pos, int z_pos) { //constructer
    this->x_pos = x_pos; //Set what row we are in
    this->z_pos = z_pos; //Set what column we are in
    visited = false; //Initialise visited to false
}

int Cell::GetX_pos() const { return x_pos; }
int Cell::GetZ_pos() const { return z_pos; }

void Cell::SetVisited(bool value) { visited = value; }
bool Cell::GetVisited() const { return visited; }

bool Cell::operator== (const Cell& RightSide) const {
    return (x_pos == RightSide.x_pos) && (z_pos == RightSide.z_pos); //Compare the positions
}

void Cell::testPrint() const { //Test print function for evaluating everythings working
    std::cout << "Cell positions" << std::endl;
    std::cout << "x_pos: " << x_pos << std::endl;
    std::cout << "z_pos: " << z_pos << std::endl; 
}

void Cell::testPrintVisited() const { // print visited value
    if (visited == true) {
        std::cout << std::endl << "visited" << std::endl;
    }
    else {
        std::cout << std::endl << "Not" << std::endl;
    }
}