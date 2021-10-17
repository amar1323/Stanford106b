#ifndef MARBLEGRAPHICS_H
#define MARBLEGRAPHICS_H

#include "marbletypes.h"
#include "map.h"
#include "gobjects.h"
#include "gwindow.h"
#include "grid.h"
#include "gevents.h"
#include "set.h"
#include "grid.h"
#include "vector.h"
#include <string>
#include <fstream>
using namespace std;


const bool RANDOMIZED_POSSIBLE_MOVES = true;
const int PAUSE_DURATION     = 30;
const int N_MARBLES_START    = 32;



struct Coord {
    Coord(){
        row = 0;
        col = 0;
    }

    Coord(int r, int c) {
        row = r;
        col = c;
    }

    int row;
    int col;
};

class MarbleGraphics : private GWindow
{
public:

    MarbleGraphics();
    virtual ~MarbleGraphics();

    /*
     * Initializes board graphics with marbles placed in locations
     * indicated by board parameter
     */
    void drawBoard(Grid<MarbleType>& board);

    /*
     * Moves marble graphic according to move parameter
     *
     * Returns FALSE if move is not valid according to current
     * graphics state, otherwise returns TRUE.
     */
    bool makeMove(const Move move);

    /*
     * Removes all marbles from window
     */
    void clearBoard();

    /*
     * Processes two mouse clicks to move a marble and saves
     * the move in the provided move struct and RETURNS true.
     *
     * If the user clicks the button indicating the computer
     * should solve the puzzle, RETURNS false.
     */
    bool getNextUserMove(Move& move);

    /*
     * Makes a noise and flash to notify user of user error.
     */
    void errorNotification();

private:
    string pickRandomImage();

    GImage* boardBackground;
    Grid<GImage*> marbles;
    Grid<GOval*> spaces;
    Map<GImage*,Coord> marbleCoords;
    Map<GOval*,Coord> spaceCoords;
    GRect* solveButton;
    GLabel* solveWords;
    GRect * flash;
};

/* static helpers */
bool isValidMove(Move move, Grid<MarbleType>& board);
int  humanPlay(Grid<MarbleType>& board, int marblesRemaining, MarbleGraphics& mg);
void computerPlay(const Grid<MarbleType>& board, int marblesRemaining, MarbleGraphics& mg);
int  initializeBoard(Grid<MarbleType>& board);
int  readBoardFromFile(Grid<MarbleType>& board, ifstream& file);
void setUpDefaultBoard(Grid<MarbleType>& board);
void makeMove(Move move, Grid<MarbleType>& board);
void undoMove(Move move, Grid<MarbleType>& board);
bool solvePuzzle(Grid<MarbleType>& board, Set<uint32_t>& exploredBoards,
                 Vector<Move>& moveHistory);

#endif // MARBLEGRAPHICS_H
