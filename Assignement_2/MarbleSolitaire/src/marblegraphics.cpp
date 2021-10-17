#include "marblegraphics.h"
#include "random.h"
#include "simpio.h"
#include "filelib.h"
#include <iostream>
using namespace std;


static const string kTitle = "Marble Solitaire";
static const int kMarbleDimension = 100;
static const int kMarbleSpacingWidth = 10;
static const double kWindowWidth = (8*kMarbleSpacingWidth) + (7*kMarbleDimension);
static const double kWindowHeight = (8*kMarbleSpacingWidth) + (7*kMarbleDimension);
static const int kSolveButtonOffset = 20;
static const int kSolveWordBoxPadding = 10;



int main() {
    cout << "Welcome to Marble Solitaire!" << endl;
    MarbleGraphics mg;
    Grid<MarbleType> board(7,7);

    do {
        int marblesRemaining = initializeBoard(board);
        mg.drawBoard(board);
        marblesRemaining = humanPlay(board, marblesRemaining, mg);
        //Only activate computer's turn if human hasn't won
        if (marblesRemaining == 1){
            cout << "Congrats! You have won. :-)" << endl;
        }
        else {
            computerPlay(board, marblesRemaining, mg);
        }
    } while (getLine("Game over! Play again? [y/n] ") == "y");

    return 0;
}


/* Performs human's turn. Makes appropriate graphics calls to
 * animate the user's movements on the board. When the human's
 * turn is over (either they win or they give up and ask computer
 * to solve the board), it will return the number of marbles remaining.
 */
int humanPlay(Grid<MarbleType>& board, int marblesRemaining, MarbleGraphics& mg) {
    Move move;
    // loop ends when user clicks button asking computer
    // to solve the puzzle or if only one marble left
    while (mg.getNextUserMove(move)){
        if (isValidMove(move, board)) {
            makeMove(move, board);
            mg.makeMove(move);
            marblesRemaining--;
            if(marblesRemaining == 1) break;
        }
        else {
            mg.errorNotification();
        }
    }
    return marblesRemaining;
}



/* Performs computer play which will call the recursive exhaustive
 * search function in order to try to find a valid path. If a
 * valid path is found, it will allow the user to step through
 * the solution.
 */
void computerPlay(const Grid<MarbleType>& board, int marblesRemaining, MarbleGraphics& mg){
    Set<uint32_t> exploredBoards;
    Vector<Move> pathToWin;
    cout << "Starting computer solver" << endl;
    Grid<MarbleType> boardCopy = board;
    bool won = solvePuzzle(boardCopy, exploredBoards, pathToWin);
    if (!won) {
        cout << "Sorry, no solution found!" << endl;
    }
    else {
        cout << "Solution found!!!" << endl;
        for (Move m : pathToWin){
            cout << "Move " << m;
            mg.makeMove(m);
            getLine("  Press ENTER to continue.");
        }
    }
}

/* Checks whether the given move is valid on this board.
 * A move is valid if
 * 1) A marble occupies the start location.
 * 2) The end location is empty.
 * 3) A marble occupies the space in between the start and end locations.
 * 4) The start and end locations are in an orthogonal direction and
 *    exactly 2 spaces away (i.e. no diagonal moves)
 * If all 3 of these properties are true, then this method will
 * return that the move is valid.
 */
bool isValidMove(Move move, Grid<MarbleType>& board){
    if (board[move.endRow][move.endCol] != MARBLE_EMPTY) return false;
    if (board[move.startRow][move.startCol] != MARBLE_OCCUPIED) return false;
    int rowDelta = abs(move.endRow - move.startRow);
    int colDelta = abs(move.endCol - move.startCol);
    if (!((rowDelta == 2 && colDelta == 0) || (rowDelta == 0 && colDelta == 2))) {
        return false;
    }
    int jumpedRow = move.startRow + (move.endRow - move.startRow) / 2;
    int jumpedCol = move.startCol + (move.endCol - move.startCol) / 2;
    if (board[jumpedRow][jumpedCol] != MARBLE_OCCUPIED) return false;
    return true;
}



/* Initializes the game board by either reading a file or setting
 * up the default board.
 * Parameter validEnds: a set of valid end locations on the board
 * where the last marble must finish in order to have won the game.
 * If the last marble ends in a location not in this set, then the
 * game is lost.
 * Returns the number of marbles on the gameboard.
 */
int initializeBoard(Grid<MarbleType>& board){
    if(getLine("Do you want to read the board configuration from a file? [y/n] ") == "y"){
        ifstream file;
        promptUserForFile(file, "Enter filename (ie. 'boards/7-Step.txt'): ");
        int marblesRemaining = readBoardFromFile(board, file);
        file.close();
        return marblesRemaining;
    } else { //Do default configuration (Fill in the marbles, leaving four corners blank)
        setUpDefaultBoard(board);
        return N_MARBLES_START;
    }
}

/* Reads in the board from the file and populates the board along with
 * the set of valid end points.
 * Precondition: ifstream file parameter is already open with a correctly
 * formatted file.
 * Returns the number of marbles on the gameboard.
 */
int readBoardFromFile(Grid<MarbleType>& board, ifstream& file){
    //Skip comments at top of file
    string line = "";
    do{
        getline(file, line);
    }
    while(!file.fail() && !line.empty() && line[0] == '#');

    //Read in dimensions
    int row = stringToInteger(line);
    getline(file, line);
    int col = stringToInteger(line);

    //Read board
    int marbleCount = 0;
    for(int r = 0; r < row; r++){
        getline(file, line);
        for(int c = 0; c < col; c++){
            if(line[c] == 'X') board[r][c] = MARBLE_INVALID;
            else if(line[c] == '-') board[r][c] = MARBLE_EMPTY;
            else if(line[c] == 'O') {
                marbleCount++;
                board[r][c] = MARBLE_OCCUPIED;
            }
        }
    }
    return marbleCount;
}

/* Populates the given board with the default board configuration
 * and only has the position (3,3) as a valid end point for the game.
 */
void setUpDefaultBoard(Grid<MarbleType>& board){
    for (int row = 0; row < board.numRows(); row++){
        for (int col = 0; col < board.numCols(); col++){
            if ((row < 2 && col < 2) || (row < 2 && col > 4)
             || (row > 4 && col < 2) || (row > 4 && col > 4)){
                board[row][col] = MARBLE_INVALID;
            }
            else{
                board[row][col] = MARBLE_OCCUPIED;
            }
        }
    }
    //Remove the very center marble
    board[3][3] = MARBLE_EMPTY;
}


MarbleGraphics::MarbleGraphics() : GWindow(kWindowWidth, kWindowHeight) {
    marbles.resize(7,7);
    spaces.resize(7,7);
    setWindowTitle(kTitle);
    setVisible(true);

    //words on solve button
    solveWords = new GLabel("Solve it for me!");
    solveWords->setColor("WHITE");
    solveWords->setFont("Arial-Bold-18");
    solveWords->setVisible(true);

    //solve button rectangle
    solveButton = new GRoundRect(solveWords->getWidth()+kSolveWordBoxPadding,solveWords->getHeight()+10);
    solveButton->setFilled(true);
    solveButton->setFillColor("RED");
    solveButton->setVisible(true);

    //a "flash" to display quickly when user makes an error
    flash = new GRect(kWindowWidth+10,kWindowHeight+10);
    flash->setColor("YELLOW");
    flash->setFilled(true);
    flash->setVisible(true);

    // Wood Board background
    boardBackground = new GImage("images/woodBoard.jpg");
    boardBackground->scale((kWindowWidth+10)/boardBackground->getWidth(), (kWindowHeight+10)/boardBackground->getHeight());
    boardBackground->setVisible(true);
}

MarbleGraphics::~MarbleGraphics() {
    for (GObject * obj : marbles){
        if (obj != NULL) delete obj;
    }
    for (GObject * obj : spaces){
        if (obj != NULL) delete obj;
    }
    delete solveWords;
    delete solveButton;
    delete flash;
    delete boardBackground;
}

void MarbleGraphics::clearBoard(){
    this->clear();
    for (GObject * obj : marbles){
        if (obj != NULL) {
            delete obj;
            obj = NULL;
        }
    }
    for (GObject * obj : spaces){
        if (obj != NULL) {
            delete obj;
            obj = NULL;
        }
    }
    marbleCoords.clear();
    spaceCoords.clear();

    GMouseEvent me;
    do{
        me = getNextEvent(MOUSE_EVENT);
    }
    while(me.getEventClass() != NULL_EVENT);
}

template<typename ValueType>
void clearGrid(Grid<ValueType *>& grid){
    for(int r = 0; r < grid.numRows(); r++){
        for(int c = 0; c < grid.numCols(); c++){
            grid[r][c] = NULL;
        }
    }
}

void MarbleGraphics::drawBoard(Grid<MarbleType>& board){
    clearBoard();
    marbles.resize(board.numRows(), board.numCols());
    spaces.resize(board.numRows(), board.numCols());
    clearGrid(marbles); clearGrid(spaces);
    add(boardBackground, 0, 0);
    add(solveButton,kSolveButtonOffset,kSolveButtonOffset);
    add(solveWords,kSolveButtonOffset+kSolveWordBoxPadding/2,kSolveButtonOffset+(solveWords->getHeight()));

    for (int row=0; row<board.numRows(); row++){
        for (int col=0; col<board.numCols(); col++){
            if (board[row][col] == MARBLE_OCCUPIED || board[row][col] == MARBLE_EMPTY){
                if(board[row][col] == MARBLE_EMPTY){
                    GOval* emptySpace = new GOval(kMarbleDimension, kMarbleDimension);
                    spaces[row][col] = emptySpace;
                    this->add(emptySpace, col*(kMarbleDimension+kMarbleSpacingWidth)+kMarbleSpacingWidth,
                              row*(kMarbleDimension+kMarbleSpacingWidth)+kMarbleSpacingWidth);
                    spaceCoords[emptySpace] = Coord(row, col);
                } else{
                    GImage * marble = new GImage(pickRandomImage());
                    marbles[row][col] = marble;
                    this->add(marble, col*(kMarbleDimension+kMarbleSpacingWidth)+kMarbleSpacingWidth,
                              row*(kMarbleDimension+kMarbleSpacingWidth)+kMarbleSpacingWidth);
                    marbleCoords[marble] = Coord(row,col);
                }
            }
        }
    }
}

bool MarbleGraphics::makeMove(const Move move){
    GImage * movingMarble = marbles[move.startRow][move.startCol];
    int jumpedRow = move.startRow+(move.endRow-move.startRow)/2;
    int jumpedCol = move.startCol+(move.endCol-move.startCol)/2;
    GImage * jumpedMarble = marbles[jumpedRow][jumpedCol];
    if (movingMarble == NULL || jumpedMarble == NULL){
        return false;
    }

    //Move empty spot from end to start of move
    GOval* endSpot = spaces[move.endRow][move.endCol];
    double tempX = endSpot->getX();
    double tempY = endSpot->getY();
    endSpot->setLocation(movingMarble->getX(), movingMarble->getY());
    spaces[move.startRow][move.startCol] = endSpot;
    spaces[move.endRow][move.endCol] = NULL;
    spaceCoords[endSpot] = Coord(move.startRow, move.startCol);

    //Move image at start location to image at end location
    movingMarble->setLocation(tempX, tempY);
    marbles[move.endRow][move.endCol] = movingMarble;
    marbles[move.startRow][move.startCol] = NULL;
    marbleCoords[movingMarble] = Coord(move.endRow, move.endCol);

    //Delete marble for jumped space and create empty space there instead
    marbles[jumpedRow][jumpedCol] = NULL;
    marbleCoords.remove(jumpedMarble);
    remove(jumpedMarble);
    delete jumpedMarble;
    GOval* jumpedSpace = new GOval(kMarbleDimension, kMarbleDimension);
    spaces[jumpedRow][jumpedCol] = jumpedSpace;
    spaceCoords[jumpedSpace] = Coord(jumpedRow, jumpedCol);
    add(jumpedSpace, jumpedCol*(kMarbleDimension+kMarbleSpacingWidth)+kMarbleSpacingWidth,
        jumpedRow*(kMarbleDimension+kMarbleSpacingWidth)+kMarbleSpacingWidth);

    return true;
}

bool MarbleGraphics::getNextUserMove(Move& move) {
    GMouseEvent me;
    bool startClick = true;
    while (true) {
        me = getNextEvent(MOUSE_EVENT);
        if (me.getEventType() == MOUSE_CLICKED) {
            GObject* obj = getGObjectAt(me.getX(),me.getY());
            if (obj == NULL) {
                startClick = true;
                errorNotification();
            }
            // User clicked in SOLVE button
            else if (obj->getType() == "GRoundRect" || obj->getType() == "GLabel"){
                return false;
            }
            // User clicked in MARBLE
            else if (obj->getType() == "GImage") {
                // First click = starting location
                if (startClick) {
                    move.startRow = marbleCoords[(GImage*)obj].row;
                    move.startCol = marbleCoords[(GImage*)obj].col;
                    //If click on visible marble (Flash it to show selection detected)
                    if(((GImage*)obj)->isVisible()){
                        ((GImage*) obj)->setVisible(false);
                        pause(50);
                        ((GImage*) obj)->setVisible(true);
                    }
                    startClick = false;
                }
            }
            //User clicked in empty valid space on playing board
            else if (obj->getType() == "GOval"){
                // Second click = ending location
                if (!startClick) {
                    move.endRow = spaceCoords[(GOval*)obj].row;
                    move.endCol = spaceCoords[(GOval*)obj].col;
                    return true;
                }
            }
        }
    }
    return false;
}


void MarbleGraphics::errorNotification(){
    add(flash,0,0);
    pause(50);
    remove(flash);
}

string MarbleGraphics::pickRandomImage(){
    Vector<string> images;
    images += "images/m1.png", "images/m2.png", "images/m3.png", "images/m4.png", "images/m5.png";
    return images[randomInteger(0, images.size()-1)];
}

bool operator<(const Coord& c1, const Coord& c2){
    if (c1.row < c2.row) return true;
    if (c1.row > c2.row) return false;
    if (c1.col < c2.col) return true;
    if (c1.col > c2.col) return false;
    return false;
}
