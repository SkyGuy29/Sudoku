/*
Make a sudoku then solve it

solver comes first, then the generator.

sudoku rules:
numbers placed may only go 1-9
there cannot be two of the same number in a row, collumn, or 3x3 block
the grid must be fully filled in and adhere to these other two rules.
*/

#include <iostream>
#include <fstream>


void readFile(int[][9]);
void initializeCandidates(int[][9], bool[][9][9]);
void updateCandidates(bool[][9][9], int, int, int);
void displayGrid(int[][9]);
void displayCandidates(bool[][9][9], int, int);
void displayRowCandidates(bool[][9][9], int);
void displayColCandidates(bool[][9][9], int);
bool backtrackSolve(int[][9], unsigned = 0);
bool isSafe(int[][9], int, int, int);

bool firstSolve(int[][9], bool[][9][9]);
bool secondSolve(int[][9], bool[][9][9]);


int main()
{
    //first two brackets are tied to a grid coordinate, last one is if candidate 1-9 is true
    //if seven is a possible candidate of the middle tile, candidates[4][4][6] would be true
    bool candidates[9][9][9]{}, updated = true;
    int choice, xCoord, yCoord, grid[9][9]
    {
        0, 9, 1, 0, 0, 7, 0, 0, 0,
        0, 3, 0, 5, 8, 0, 9, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 3, 0,
        4, 0, 6, 0, 0, 0, 0, 0, 5,
        0, 0, 0, 2, 0, 3, 0, 0, 7,
        0, 0, 7, 0, 0, 1, 0, 0, 0,
        0, 0, 8, 7, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 8, 9, 0,
        5, 0, 0, 8, 9, 0, 0, 0, 0
    };

    //   readFile(grid); //WHY DOESNT PROGRAMIZ LET ME ADD A TEXT FILE RRRRRRRRRRRRGGGGGGGGHHHHHHHHH
    initializeCandidates(grid, candidates);

    //menu front end display
    while (true)
    {
        if(updated)
            displayGrid(grid);
        else
            std::cout << "No changes made! \n";
        updated = true;
        std::cout << std::endl;
        std::cout 
            << "0) exit \n"
            << "1) scream \n"
            << "2) set a tile's value \n"
            << "5) list candidates of a tile \n"
            << "6) list row candidates \n"
            << "7) list collumn candidates \n"
            << "8) solve \n"
            << std::endl
            << "pick an option: ";
        std::cin >> choice;

        //runs functions based on your option (listen im trying to comment everything that means this too)
        switch (choice)
        {
            case 0: //exit
                return 0;
            case 1: //scream
                while (true)
                    std::cout << 'A';
            case 2: //set a tile's value
                std::cout << "y and x coordinates: ";
                std::cin >> yCoord >> xCoord;
                std::cout << "value: ";
                std::cin >> choice;
                if (grid[yCoord][xCoord] == 0)
                {
                    grid[yCoord][xCoord] = choice;
                    updateCandidates(candidates, choice, yCoord, xCoord);
                }
                break;
            case 5: //list candidates of a tile
                std::cout << "y and x coordinates: ";
                std::cin >> yCoord >> xCoord;
                displayCandidates(candidates, yCoord, xCoord);
                break;
            case 6: //list candidates of a row
                std::cout << "y coordinate: ";
                std::cin >> yCoord;
                displayRowCandidates(candidates, yCoord);
                break;
            case 7: //list candidates of a collumn
                std::cout << "x coordinate: ";
                std::cin >> xCoord;
                displayColCandidates(candidates, xCoord);
                break;
            case 8: //solve
                backtrackSolve(grid);
        }
        std::cout << "done" << std::endl << std::endl;
        choice = 0;
    }

    //its a while(true) loop, the only exit is if you choose to exit.
}


//loads a grid from the sudoku.txt file 
void readFile(int grid[][9])
{
    std::fstream file;
    file.open("sudoku.txt", std::ios::in);
    for (int i = 0; i < 81; ++i)
        file >> grid[i / 9][i % 9];
    file.close();
}


//initializes possible candidates for values based on the values already in the grid
void initializeCandidates(int grid[][9], bool candidates[][9][9])
{
    //method 1
    //give every tile every candidate
    for (int i = 0; i < 81; ++i)
        if (grid[i / 9][i % 9] == 0)
            for (int j = 0; j < 9; ++j)
                candidates[i / 9][i % 9][j] = true;
    
    //if there is a value in a tile, run the update candidates function to remove
    //any candidate that would relate to the value in the tile.
    for (int i = 0; i < 81; ++i)
        if (grid[i / 9][i % 9] != 0)
            updateCandidates(candidates, grid[i / 9][i % 9] - 1, i / 9, i % 9);
}


//called after a tile is given a new value, updates candidates in the same row,
//collumn, and block as the updated tile.
//also called in initializing for the same reason.
void updateCandidates(bool candidates[][9][9], int value, int y, int x)
{
    //after a tile is given a value, remove all of its candidates and remove
    //all of the candidates that match that value in the row, collumn, AND block.
    for (int i = 0; i < 9; ++i) //all tile candidates
        candidates[y][x][i] = false;

    for (int i = 0; i < 9; ++i) //row
        candidates[y][i][value] = false;

    for (int i = 0; i < 9; ++i) //collumn
        candidates[i][x][value] = false;

    for (int i = 0; i < 9; ++i) //block
        candidates[y / 3 * 3 + i / 3][x / 3 * 3 + i % 3][value] = false;
}


//displays the grid to the console
void displayGrid(int grid[][9])
{
    for (int i = 0; i < 81; ++i)
    {
        if (grid[i / 9][i % 9] == 0)
            std::cout << ". ";
        else
            std::cout << grid[i / 9][i % 9] << ' ';
        if ((i + 1) % 9 == 0)
            std::cout << std::endl;
    }
}


//displays the candidates of a specific tile
void displayCandidates(bool candidates[][9][9], int y, int x)
{
    for (int i = 0; i < 9; ++i)   
        if (candidates[y][x][i])
            std::cout << i + 1 << ' ';
    
    std::cout << std::endl;
}


//displays the candidates of a whole row
void displayRowCandidates(bool candidates[][9][9], int y)
{
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            if(candidates[y][i][j])
                std::cout << j + 1;
            else
                std::cout << ' ';

            std::cout << ' ';
        }

        std::cout << std::endl;
    }
}


//displays the candidates of a whole collumn
void displayColCandidates(bool candidates[][9][9], int x)
{
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            if(candidates[i][x][j])
                std::cout << j + 1;
            else
                std::cout << ' ';

            std::cout << ' ';
        }

        std::cout << std::endl;
    }
}


//solves sudoku via a recursive backtracking algorithm. yep. brute force.
//you CAN manually give a 
bool backtrackSolve(int grid[][9], unsigned coord)
{
    if(grid[coord / 9][coord % 9] == 0) //if the tile is blank, we skip it. see the else statement below.
    {
        for (int count = 1; count <= 9; ++count) //repeat next steps for each value 1-9.
        {
            grid[coord / 9][coord % 9] = count; //first step. give tile a value.

            if(isSafe(grid, count, coord / 9, coord % 9) //check to see if the value is good.
                && (coord >= 80 || backtrackSolve(grid, coord + 1))) //backtrackSolve moves to the next tile.
                //if the value is good and coord >= 80, the grid is solves, and this returns true.
                //backtrackSolve will then chain return true to terminate the recursion.
                return true;
        }
        
        //if none of the nine values were successful, we reset the tile to blank
        //and return false to recusrively move back to the previous tile.
        grid[coord / 9][coord % 9] = 0;
        return false;
    }
    else
        //returns the same thing backtrackSolve does, unless coord >= 80, in which the grid is solved.
        //calling backtrackSolve here effectively just skips over the filled in tile.
        if (coord >= 80 || backtrackSolve(grid, coord + 1)) 
            return true;
        else
            return false;
}


//checks to see if a value in a tile follows the sudoku rules.
bool isSafe(int grid[][9], int value, int y, int x)
{
    int count = 0;

    for (int i = 0; i < 9; ++i)
        if (grid[y][i] == value)
            ++count;

    if (count > 1)
        return false;

    count = 0;

    for (int i = 0; i < 9; ++i)
        if (grid[i][x] == value)
            ++count;

    if (count > 1)
        return false;

    count = 0;

    for (int i = 0; i < 9; ++i)
        if(grid[y / 3 * 3 + i / 3][x / 3 * 3 + i % 3] == value)
            ++count;

    if (count > 1)
        return false;

    return true;
}


//the next two functions are obsolete, i put in so much work into them tho i dont want to delete them.


//if there is only one candidate in the tile it must be that one
//returns true if a tile was given a value, false if nothing happened.
bool firstSolve(int grid[][9], bool candidates[][9][9])
{
    bool updated = false;
    for (int i = 0 ; i < 81; ++i)
    {
        if (grid[i / 9][i % 9] == 0)
        {
            //using the grid value itself as a temporary value, I'm a bit proud of that idea
            //it will be set properly later.
            for (int j = 0; j < 9; ++j)
                grid[i / 9][i % 9] += candidates[i / 9][i % 9][j]; //count how many candidates there are here

            //if there is only one candidate...
            if (grid[i / 9][i % 9] == 1)
            {
                for (int j = 0; j < 9; ++j) //start counting again to find the candidate
                    if (candidates[i / 9][i % 9][j]) 
                    {
                        grid[i / 9][i % 9] = j + 1; //j goes from 0-8, candidates go from 1-9
                        updateCandidates(candidates, grid[i / 9][i % 9] - 1, i / 9, i % 9);
                        updated = true;
                    }
            }
            else
                grid[i / 9][i % 9] = 0; //reset.
        }
    }

    return updated;
}


//if there is only one of a specific candidate in a row, collumn, or block
//then that candidate must be the value in the tile.
//returns true if a tile was given a value, false if nothing happened.
//this doesnt seem optimized but im not sure how to fix it :/
bool secondSolve(int grid[][9], bool candidates[][9][9])
{
    bool updated = false;
    int count = 0;
    for (int i = 0; i < 9; ++i) //check each value
    {
        for (int j = 0; j < 81; ++j) //rows
        {
            count += candidates[j / 9][j % 9][i];

            if ((j + 1) % 9 == 0) //at the end of the row...
            {   
                if (count == 1)
                {
                    for (int k = 0; k < 9; ++k) //check the row again for the single value
                    {
                        if (candidates[j / 9][k][i])
                        {
                            grid[j / 9][k] = i + 1;
                            updateCandidates(candidates, grid[j / 9][k] - 1, j / 9, k);
                            updated = true;
                        }
                    }
                }
                count = 0; //reset count at the end of the row
            }
        }

        for (int j = 0; j < 81; ++j) //collumns
        {
            count += candidates[j % 9][j / 9][i];

            if ((j + 1) % 9 == 0) //at the end of the collumn...
            {   
                if (count == 1)
                {
                    for (int k = 0; k < 9; ++k) //check the collumn again for the single value
                    {
                        if (candidates[k][j / 9][i])
                        {
                            grid[k][j / 9] = i + 1;
                            updateCandidates(candidates, grid[k][j / 9] - 1, k, j / 9);
                            updated = true;
                        }
                    }
                }
                count = 0; //reset count at the end of the collumn
            }
        }

        for (int j = 0; j < 9; ++j) //blockes
        {
            //there is a reason for j / 3 * 3 + l / 3 and j % 3 * 3 + l % 3
            //j / 3 * 3 and j % 3 * 3 cycles through the blockes, and starts the search at the 
            //top left tile in each, and the l / 3 l % 3 cycles through the tiles in the block
            for (int l = 0; l < 9; ++l)
                count += candidates[j / 3 * 3 + l / 3][j % 3 * 3 + l % 3][i];

            //at the end of the block...
            if (count == 1)
            {
                for (int k = 0; k < 9; ++k) //check the block again for the single value
                {
                    if (candidates[j / 3 * 3 + k / 3][j % 3 * 3 + k % 3][i])
                    {
                        //this is such a mess if i have to debug this i may explode
                        grid[j / 3 * 3 + k / 3][j % 3 * 3 + k % 3] = i + 1;
                        updateCandidates(candidates, grid[j / 3 * 3 + k / 3][j % 3 * 3 + k % 3] - 1,
                            j / 3 * 3 + k / 3, j % 3 * 3 + k % 3);
                        updated = true;
                    }
                }
            }
            count = 0; //reset count at the end of the block
        }
    }

    return updated;
}

void stackOverflow()
{
    int i;
    stackOverflow();
}