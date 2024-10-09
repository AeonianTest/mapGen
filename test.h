#ifndef TEST_H
#define TEST_H

#include <vector>
#include <iostream>
#include <cmath>
#include <random>
#include "Cell.h"

void testPrint(int length, int width, std::vector<std::vector<char> > &map);
bool checkOdd(int value);
bool checkEdge(int value, int limit);

void EmptyCells(int length, int width, std::vector<Cell*> &CellList, std::vector<std::vector<char> > &map); //Adjusting program to account for test mode
int PickRandomCell(int &x_pos, int &z_pos, std::vector<Cell*> &CellList, std::vector<std::vector<char> > &map, std::mt19937 &rand, int length, int width);
void BreakWall(int &x_pos, int &z_pos, std::vector<std::vector<char> > &map, std::mt19937 &rand, int length, int width);

//Functions for testing non-randomised maze
int PickTestCell(int &x_pos, int &z_pos, std::vector<std::vector<char> > &map);
void TestMapMake(Cell* CurrentCell, std::vector<std::vector<char> > &map, std::vector<Cell*> &CellList); 
Cell* TestVistNeighbour(Cell* cell, std::vector<std::vector<char> > &map, std::vector<Cell*> &CellList); //Up right down left rotation

void Recursion(Cell* CurrentCell, std::vector<std::vector<char> > &map, std::mt19937 &rand, std::vector<Cell*> &CellList); //Testing recursion implementation
Cell* VistNeighbour(Cell* cell, std::vector<std::vector<char> > &map, std::mt19937 &rand, std::vector<Cell*> &CellList);
bool CheckUnvisited(std::vector<Cell*> &CellList, Cell* cell);

#endif // TEST_H