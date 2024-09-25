/*
Make a sudoku then solve it

solver comes first, then the generator.
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
bool firstSolve(int[][9], bool[][9][9]);
bool secondSolve(int[][9], bool[][9][9]);
bool backtrackSolve(int[][9], int);
bool isSafe(int[][9], int, int, int);

int main()
{
    //first two brackets are tied to a grid coordinate, last one is if candidate 1-9 is true
    //if seven is a possible candidate of the middle grid space, candidates[4][4][6] would be true
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
    }; //i literally got this from the new york times lol

    initializeCandidates(grid, candidates);

    //menu front end display
    while (true)
    {
        if(updated)
            displayGrid(grid);
        else
            std::cout << "No changes made!\n";
        updated = true;
        std::cout << std::endl;
        std::cout 
            << "1) end it all\n"
            << "2) scream\n"
            << "3) debug solver functions \n"
            << "4) cout a value at a location\n" 
            << "5) list candidates of a grid space \n"
            << "6) list row candidates \n"
            << "7) list collumn candidates \n"
            << "8) backtrack solve \n"
            << std::endl
            << "pick an option: ";
        std::cin >> choice;

        //runs functions based on your option (listen im trying to comment everything that means this too)
        switch (choice)
        {
            case 1:
                return 0;
            case 2:
                while (true)
                    std::cout << 'A';
            case 3:
                std::cout << "pick a solve (1-2): ";
                std::cin >> choice;
                switch (choice)
                {
                    case 1:
                        updated = firstSolve(grid, candidates);
                        break;
                    case 2:
                        updated = secondSolve(grid, candidates);
                }
                break;
            case 4: 
                std::cout << "y and x coordinates: ";
                std::cin >> yCoord >> xCoord;
                std::cout << grid[yCoord][xCoord];
                break;
            case 5:
                std::cout << "y and x coordinates: ";
                std::cin >> yCoord >> xCoord;
                displayCandidates(candidates, yCoord, xCoord);
                break;
            case 6:
                std::cout << "y coordinate: ";
                std::cin >> yCoord;
                displayRowCandidates(candidates, yCoord);
                break;
            case 7:
                std::cout << "x coordinate: ";
                std::cin >> xCoord;
                displayColCandidates(candidates, xCoord);
                break;
            case 8:
                backtrackSolve(grid, 0);
        }
        std::cout << "done" << std::endl << std::endl;
        choice = 0;
    }
    //solve using candidates

    /*if two grid spaces in a row, collumn, or block share the same two candidates, the rest
    of the grid spaces in the row, column, or block will not have those as candidates, and
    we can remove them. may be smart to do this earlier than later.
    this works for anything from 2 to 8 as well. replace the word "two" with the new
    number and it will still hold true.
    
    doing this method */

    //could this theoretically lead to an optimized version of this that involves recursively
    //checking rows, collumns, and blockes after a change is made for any new possible values?
    //that would be so sick if that worked :O
    //I'm not really looking to code that rn tho lol

    //AUTOMATICALLY IF THERE ARE NO CANDIDATES ON A BLANK SPACE SOMETHING WENT WRONG
    //same thing goes for if there are no candidates or valued block for a value in a 
    //row, collumn, or block
    //...I've said that SOOO MUCH already good grief

    //all of these different methods are going to need to be repeated several times until
    //the puzzle is solved...

    /*
    brute force backtracking because i do not care

    place down 1 on the first open grid space
    check to see if it works
    if it does, move to the next OPEN tile and repeat of that tile
    if not, repeat, increasing the number by one
    
    if the number hits 9 and fails, clear the tile an move to the previous tile.
    the previous tile will be incremented by one and checked once again

    theoretically this should work
    */



    return 0;
}

bool backtrackSolve(int grid[][9], int coord)
{
    /*
    brute force backtracking because i do not care

    place down 1 on the first open grid space
    check to see if it works
    if it does, move to the next OPEN tile and repeat of that tile
    if not, repeat, increasing the number by one
    
    if the number hits 9 and fails, clear the tile an move to the previous tile.
    the previous tile will be incremented by one and checked once again

    theoretically this should work
    */

        if(grid[coord / 9][coord % 9] == 0)
        {
            for (int count = 1; count <= 9; ++count)
            {
                grid[coord / 9][coord % 9] = count;
                std::cout << coord << std::endl;

                if(isSafe(grid, coord / 9, coord % 9, count))
                    if (coord >= 80 || backtrackSolve(grid, coord + 1))
                        return true;
            }
        }
        else
            if (backtrackSolve(grid, coord + 1))
                return true;


    grid[coord / 9][coord % 9] = 0;
    return false;
}

bool isSafe(int grid[][9], int y, int x, int value)
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


//loads a grid from the sudoku.txt file 
void readFile(int grid[][9])
{
    std::fstream file;
}


//initializes possible candidates for values based on the values already in the grid
void initializeCandidates(int grid[][9], bool candidates[][9][9])
{
    //method 1
    //give every grid space every candidate
    for (int i = 0; i < 81; ++i)
        if (grid[i / 9][i % 9] == 0)
            for (int j = 0; j < 9; ++j)
                candidates[i / 9][i % 9][j] = true;
    
    //if there is a value in a grid space, run the update candidates function to remove
    //any candidate that would relate to the value in the grid space.
    for (int i = 0; i < 81; ++i)
        if (grid[i / 9][i % 9] != 0)
            updateCandidates(candidates, grid[i / 9][i % 9] - 1, i / 9, i % 9);
}


//called after a grid space is given a new value, updates candidates in the same row,
//collumn, and block as the updated grid space.
//also called in initializing for the same reason.
void updateCandidates(bool candidates[][9][9], int value, int y, int x)
{
    //after a grid space is given a value, remove all of its candidates and remove
    //all of the candidates that match that value in the row, collumn, AND block.
    for (int i = 0; i < 9; ++i) //all grid space candidates
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


//displays the candidates of a specific space
void displayCandidates(bool candidates[][9][9], int y, int x)
{
    //to do: make a check for if the grid space has a value
    //do this once you have checked that updateCandidates works properly
    //the display should only list candidates of blank spaces
    //theoretically, valued grid spaces should have no candidates
    for (int i = 0; i < 9; ++i)   
        if (candidates[y][x][i])
            std::cout << i + 1 << ' ';
    
    std::cout << std::endl;
}

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


//if there is only one candidate in the grid space it must be that one
//returns true if a grid space was given a value, false if nothing happened.
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
//then that candidate must be the value in the grid space.
//returns true if a grid space was given a value, false if nothing happened.
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
            //top left grid space in each, and the l / 3 l % 3 cycles through the spaces in the block
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
