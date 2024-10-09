#include "test.h"
#include "Cell.h"

int main(int argc, char* argv[]) { 
    int length, width;
    int x_pos = 0, z_pos = 0; //x starting pos. x is up down (length). z starting pos. z is left right (width)
    bool TestMaze = false; //Loop for if we are doing a non randomised test of the map

    if (argc > 1) { //Check for cmd line args
        std::string argv1 = argv[1]; //Get the second command line argurement
        if (argv1 == "test") { TestMaze = true; } //Set test Maze to true, derandomise the test generation
    }

    if (argc > 3) {
        length = std::atoi(argv[2]);
        width = std::atoi(argv[3]);
    }
    else {
	    length = 15, width = 15;
    }	

    std::mt19937 rand(time(0)); //Random generator. seed w time 
    std::vector<Cell*> CellList; //All pointers to all valid empty cells
    std::vector<std::vector<char> > map(length, std::vector<char>(width, 'X')); //Set 2d vector to length outside vector(as to go up down you traverse the out vector)
    //Then inside each length is width, which to traverse left and right you go through here in the inner

    if (!TestMaze) { //If Test Maze = false
        EmptyCells(length, width, CellList, map); //Empty the cells
        int RandCellNum = PickRandomCell(x_pos, z_pos, CellList, map, rand, length, width); //Pick a random Cell
        BreakWall(x_pos, z_pos, map, rand, length, width); //Break the wall
        Cell* CurrentCell = CellList.at(RandCellNum); //Get address of starting cell
        Recursion(CurrentCell, map, rand, CellList); //Call the recursion to do DFS
    }
    else { //Else do non-random Test Maze
        EmptyCells(length, width, CellList, map); //Make the empty cells
        int TestStartCellNum = PickTestCell(x_pos, z_pos, map);
        map[1][0] = '*';//Break Test wall to the left
        Cell* CurrentCell = CellList.at(TestStartCellNum);
        //CurrentCell->testPrint();
        //CurrentCell->testPrintVisited(); //Test print its info
        TestMapMake(CurrentCell, map, CellList);
    }

    for (Cell* DeleteCell : CellList) { //Cleaning all dynamic objects
        delete DeleteCell;
    }

    testPrint(length, width, map); //test print

    return 0; //DONT FORGET TO RELEASE ALL POINTERS AT END IF NEEDED
}

//To understand this code you must understand this code
void Recursion(Cell* CurrentCell, std::vector<std::vector<char> > &map, std::mt19937 &rand, std::vector<Cell*> &CellList) { //Testing a resursive implementation. Also pass around memory address of map and rand number generator
    bool ExitLoop = false;

    //Base case
    while (!ExitLoop) {
        if (CheckUnvisited(CellList, CurrentCell)) { //If all neighbour cells are visited
            CurrentCell->SetVisited(true); //Do nothing beside setting visited, let all the values be returned
        }
        else { //else we break the problem down by visting a neighbour cell and calling the function again to check if base case then
            CurrentCell->SetVisited(true); //Set visited to current cell true Cell* NewCell = VistNeighbour(CurrentCell, map, rand, CellList); //Break neighbour cell randomly, and then assume that cell
            Cell* NewCell = VistNeighbour(CurrentCell, map, rand, CellList);
            Recursion(NewCell, map, rand, CellList);
        }

        ExitLoop = CheckUnvisited(CellList, CurrentCell); //Check neigbours. Return T if all visited, thus escaping the loop and killing this instances
    }
}

Cell* VistNeighbour(Cell* cell, std::vector<std::vector<char> > &map, std::mt19937 &rand, std::vector<Cell*> &CellList) { //Vist neighbour cell, then return the new current cell
    Cell* UpCell = new Cell(cell->GetX_pos() - 2, cell->GetZ_pos()); //Initalise them temporalily, use to compare with operator overload
    Cell* DownCell = new Cell(cell->GetX_pos() + 2, cell->GetZ_pos());
    Cell* LeftCell = new Cell(cell->GetX_pos(), cell->GetZ_pos() - 2);
    Cell* RightCell = new Cell(cell->GetX_pos(), cell->GetZ_pos() + 2);

    int RandNum;
    bool ExitLoop = false;
    Cell* ReturnCell = nullptr;
    Cell* CheckCell = nullptr;

    while (!ExitLoop) { //While ExitLoop fasle
        RandNum = rand() % CellList.size(); //Get index from range
        CheckCell = CellList.at(RandNum); //Get a random cell from the list
        
        if (!CheckCell->GetVisited()) { //If cell is not visited
            if (*CheckCell == *UpCell) { //check for equivelant coords. Werent working earlier as i was comparing pointers
                ReturnCell = CheckCell;
                map[UpCell->GetX_pos() + 1][UpCell->GetZ_pos()] = ' ';
                map[UpCell->GetX_pos()][UpCell->GetZ_pos()] = ' ';
                ExitLoop = true;
            }
            else if (*CheckCell == *DownCell) {
                ReturnCell = CheckCell;
                map[DownCell->GetX_pos() - 1][DownCell->GetZ_pos()] = ' ';
                map[DownCell->GetX_pos()][DownCell->GetZ_pos()] = ' ';
                ExitLoop = true;
            }
            else if (*CheckCell == *LeftCell) {
                ReturnCell = CheckCell;
                map[LeftCell->GetX_pos()][LeftCell->GetZ_pos() + 1] = ' ';
                map[LeftCell->GetX_pos()][LeftCell->GetZ_pos()] = ' ';
                ExitLoop = true;
            }
            else if (*CheckCell == *RightCell) {
                ReturnCell = CheckCell;
                map[RightCell->GetX_pos()][RightCell->GetZ_pos()] = ' ';
                map[RightCell->GetX_pos()][RightCell->GetZ_pos() - 1] = ' ';
                ExitLoop = true;
            }
        }
    }

    delete UpCell; //Clean up temp objects
    delete DownCell;
    delete LeftCell;
    delete RightCell;

    return ReturnCell;
}

bool CheckUnvisited(std::vector<Cell*> &CellList, Cell* cell) { //If theres any unvisited neighbours (ie a direction to go in), return false. Return true if all neighbours are visited
    bool ReturnBool = true; //Default value to return, set true

    for (Cell* CheckCell : CellList) { //For every cell in the total list
        if (ReturnBool == true) { //While Return Bool still true
            if (cell->GetX_pos() - 2 == CheckCell->GetX_pos() && cell->GetZ_pos() == CheckCell->GetZ_pos()) { //If CheckCell above
                if (!CheckCell->GetVisited()) { //If true that the cell returns false because its unvisited
                    ReturnBool = false;
                }
            }
            if (cell->GetX_pos() + 2 == CheckCell->GetX_pos() && cell->GetZ_pos() == CheckCell->GetZ_pos()) { //If cell below
                if (!CheckCell->GetVisited()) { //If true that the cell returns false because its unvisited
                    ReturnBool = false;
                }
            }
            if (cell->GetX_pos() == CheckCell->GetX_pos() && cell->GetZ_pos() + 2 == CheckCell->GetZ_pos()) { // If cell right
                if (!CheckCell->GetVisited()) { //If true that the cell returns false because its unvisited
                    ReturnBool = false;
                }
            }
            if (cell->GetX_pos() == CheckCell->GetX_pos() && cell->GetZ_pos() - 2 == CheckCell->GetZ_pos()) { //If cell left
                if (!CheckCell->GetVisited()) { //If true that the cell returns false because its unvisited
                    ReturnBool = false;
                }
            }
        }
    }

    return ReturnBool; //Return the value
}

void TestMapMake(Cell* CurrentCell, std::vector<std::vector<char> > &map, std::vector<Cell*> &CellList) { //TODO
    //This function generates a maze in a non randomised recursive manner
    bool ExitLoop = false;

    while(!ExitLoop) { //While ExitLoop still false
        if (CheckUnvisited(CellList, CurrentCell)) { //base case, returned true that no unvisited neighbours
            CurrentCell->SetVisited(true); //Do nothing beside setting visited, let all the values be returned
        }
        else { //Else we vist a cell non randomly
            CurrentCell->SetVisited(true); //Set visited to current cell true
            Cell* NewCell = TestVistNeighbour(CurrentCell, map, CellList); //Get non randomised Cell in rotated direction
            TestMapMake(NewCell, map, CellList); //Call the map maker to recursively run again
        }

        ExitLoop = CheckUnvisited(CellList, CurrentCell); //Check neigbours. Return T if all visited, thus escaping the loop and killing this instances
    }
}

Cell* TestVistNeighbour(Cell* cell, std::vector<std::vector<char> > &map, std::vector<Cell*> &CellList) { //Get Direction in a rotation up, right down left (prob use static var)
    Cell* UpCell = new Cell(cell->GetX_pos() - 2, cell->GetZ_pos()); //Initalise them temporalily, use to compare with operator overload
    Cell* DownCell = new Cell(cell->GetX_pos() + 2, cell->GetZ_pos());
    Cell* LeftCell = new Cell(cell->GetX_pos(), cell->GetZ_pos() - 2);
    Cell* RightCell = new Cell(cell->GetX_pos(), cell->GetZ_pos() + 2);

    static int Direction = 0; //Direction var, 0-3
    bool ExitLoop = false;
    Cell* ReturnCell = nullptr;
    //Cell* CheckCell = nullptr;

    while (!ExitLoop) {
        if (Direction == 0) { //Check up condition
            for (Cell* CheckCell : CellList) { //for every element in the cell list
                if (!CheckCell->GetVisited()) { //If check cell not visited
                    if (*CheckCell == *UpCell) { //If CheckCell coords match the UpCell Coords 
                        ReturnCell = CheckCell;
                        map[UpCell->GetX_pos() + 1][UpCell->GetZ_pos()] = ' ';
                        map[UpCell->GetX_pos()][UpCell->GetZ_pos()] = ' ';
                        ExitLoop = true;
                    }
                }
            }
        }
        else if (Direction == 1) { //Check Right Condition
            for (Cell* CheckCell : CellList) {
                if (!CheckCell->GetVisited()) {
                    if (*CheckCell == *RightCell) {
                        ReturnCell = CheckCell;
                        map[RightCell->GetX_pos()][RightCell->GetZ_pos()] = ' ';
                        map[RightCell->GetX_pos()][RightCell->GetZ_pos() - 1] = ' ';
                        ExitLoop = true;
                    }
                }
            }
        }
        else if (Direction == 2) { //Check down condition
            for (Cell* CheckCell : CellList) {
                if (!CheckCell->GetVisited()) {
                    if (*CheckCell == *DownCell) {
                        ReturnCell = CheckCell;
                        map[DownCell->GetX_pos() - 1][DownCell->GetZ_pos()] = ' ';
                        map[DownCell->GetX_pos()][DownCell->GetZ_pos()] = ' ';
                        ExitLoop = true;
                    }
                }
            }
        }
        else { //Check left condition
            for (Cell* CheckCell : CellList) {
                if (!CheckCell->GetVisited()) {
                    if (*CheckCell == *LeftCell) {
                        ReturnCell = CheckCell;
                        map[LeftCell->GetX_pos()][LeftCell->GetZ_pos() + 1] = ' ';
                        map[LeftCell->GetX_pos()][LeftCell->GetZ_pos()] = ' ';
                        ExitLoop = true;
                    }
                }
            }
        }

        ++Direction; //Iterate the static var
        if (Direction == 4) { //If var is over limit at 4
            Direction = 0; //reset it
        }
    }

    delete UpCell; //Clean up temp objects
    delete DownCell;
    delete LeftCell;
    delete RightCell;

    return ReturnCell; //WORK IN PROGRESSs
}

void EmptyCells(int length, int width, std::vector<Cell*> &CellList, std::vector<std::vector<char> > &map) {
    for (int i = 0; i < length; ++i) { //FUNCTION for initialisng empty cells
        if (!(i % 2 == 0)) { //if i not even
            for (int j = 0; j < width; ++j) {
                if (!(j % 2  == 0)) { //if j not even
                    map[i][j] = '.'; //Then its a valid cell. Mark it empty
                    Cell* newCell = new Cell(i, j); //i is x_pos (length/up down), j is z_pos (width/left right)
                    CellList.push_back(newCell);
                }
            }
        }
    }
}

//Function for picking a non randomised test cell (the top left)
int PickTestCell(int &x_pos, int &z_pos, std::vector<std::vector<char> > &map) {
    x_pos = 1; //Set the coords
    z_pos = 1;

    map[x_pos][z_pos] = ' '; //Mark empty cell

    return 0; //TODO: This should give the first object of the CellList? have to check if true, and then scull function call
}

//FUNCTION for getting a starting CEll
int PickRandomCell(int &x_pos, int &z_pos, std::vector<Cell*> &CellList, std::vector<std::vector<char> > &map, std::mt19937 &rand, int length, int width) {
    bool ExitLoop = false;
    int PickMe;
    int RandCellNum;

    while (!ExitLoop) { //While exitLoop not true, loop
        PickMe = rand() % 2; //get 0 or 1 randomly
        RandCellNum = rand() % CellList.size();
        Cell RandCell = *CellList.at(RandCellNum); //Get a derefernced copy of a Cell

        if (PickMe == 0) { //This is just to randomise sides (ie top and down or left and right)
            x_pos = RandCell.GetX_pos(); //Get the x_pos
            if (checkOdd(x_pos)) { //if we got a valid x_pos (ie its an odd position which is empty always). This roll can be a non edge
                z_pos = RandCell.GetZ_pos(); //now get the other value

                if (checkOdd(z_pos) && checkEdge(z_pos, width)) { //if z_pos odd AND an edge cell
                    ExitLoop = true; //exit
                }
            }
        }
        else {
            z_pos = RandCell.GetZ_pos(); //reversed order of rolls
            if (checkOdd(z_pos)) {
                x_pos = RandCell.GetX_pos();

                if (checkOdd(x_pos) && checkEdge(x_pos, length)) {
                    ExitLoop = true;
                }
            }
        }
    } 
    map[x_pos][z_pos] = ' '; //Mark change

    return RandCellNum;
}

void BreakWall(int &x_pos, int &z_pos, std::vector<std::vector<char> > &map, std::mt19937 &rand, int length, int width) { //Break the wall
    int randomNum; //random number to determine what wall to break
    bool ExitLoop = false; //reset var
    while (!ExitLoop) { //Now we break a wall adjacent. Loop until we get a valid break, then break. Wall chosen is effectively random. while wall not broke (Structurally, not financially)
        randomNum = rand() % 4; //Get number in range 0-3 randomlly
        if (randomNum == 0) { //try break up
            if (x_pos - 1 == 0) { //test breaking up valid. If not a valid break, dont do anything, and move to loop again
                map[x_pos - 1][z_pos] = '*'; //Turn to broken symbol
                ExitLoop = true; //Exit loop
            }
        }
        else if (randomNum == 1) { //try break right
            if (z_pos + 1 == width - 1) { //if valid right to break
                map[x_pos][z_pos + 1] = '*';
                ExitLoop = true;
            }
        }
        else if (randomNum == 2) { //try break down
            if (x_pos + 1 == length - 1) {
                map[x_pos + 1][z_pos] = '*';
                ExitLoop = true;
            }
        }
        else { //try break left 
            if (z_pos - 1 == 0) {
                map[x_pos][z_pos -1] = '*';
                ExitLoop = true;
            }
        }
    }
}

void testPrint(int length, int width, std::vector<std::vector<char> > &map) { //Test print the map
    for (int i = 0; i < length; ++i) { //This loop prints it all
        for (int j = 0; j < width; ++j) {
            std::cout << map[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

bool checkOdd(int value) {
    return value % 2 == 1; //Returns true if odd
}

bool checkEdge(int value, int limit) { //check if edge case
    return value - 1 == 0 || value + 1 == limit - 1; //Return true if at an empty edge cell
}
