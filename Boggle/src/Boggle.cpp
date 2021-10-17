/**
 * By Amar EL OUAZANI
 * 106b Stanford assignement Boggle
 */

#include "Boggle.h"

#include "bogglegui.h"

// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"};

// letters on every cube in 5x5 "Big Boggle" version (extension)
static string BIG_BOGGLE_CUBES[25] = {
    "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
    "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
    "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
    "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
    "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"};
static int boardSize = 5;

Boggle::Boggle(Lexicon &dictionary, string boardText) {
    this->mydict = dictionary;
    this->col = this->row = boardSize;
    this->board.resize(this->col, this->row);
    if (!BoggleGUI::isInitialized()) BoggleGUI::initialize(this->board.nRows, this->board.nCols);
    int l = 0;
    if (!boardText.empty()) {
        boardText = toUpperCase(boardText);
        for (int irow = 0; irow < this->row; irow++) {
            for (int icol = 0; icol < this->col; icol++) {
                this->board[irow][icol] = boardText[l++];
                BoggleGUI::labelCube(irow, icol, this->board[irow][icol]);
            }
        }
    } else {
        for (int irow = 0; irow < this->row; irow++) {
            for (int icol = 0; icol < this->col; icol++) {
                int r = randomInteger(0, BIG_BOGGLE_CUBES->length() - 1);
                this->board[irow][icol] = BIG_BOGGLE_CUBES[l++][r];
                BoggleGUI::labelCube(irow, icol, this->board[irow][icol]);
            }
        }
        shuffle(this->board);
        for (int irow = 0; irow < this->row; irow++) {
            for (int icol = 0; icol < this->col; icol++) {
                BoggleGUI::labelCube(irow, icol, this->board[irow][icol]);
            }
        }
    }

    this->hScore = 0;
    this->cScore = 0;
}

Vector<position> Boggle::getPositions(char c) {
    Vector<position> ret;
    for (int irow = 0; irow < this->board.nRows; irow++) {
        for (int icol = 0; icol < this->board.nCols; icol++) {
            if (getLetter(irow, icol) == c) {
                position p;
                p.i = irow;
                p.j = icol;
                ret.add(p);
            }
        }
    }
    return ret;
}
char Boggle::getLetter(int row, int col) {
    return this->board[row][col];  // return value (row,col)
}

int Boggle::getRowSize() {
    return this->row;  // return value (row,col)
}
int Boggle::getColSize() {
    return this->col;  // return value (row,col)
}
bool Boggle::checkWord(string word) {
    word = toLowerCase(word);
    if (!mydict.contains(word) || word.length() < 4 || word.length() > 16 || foundWords.contains(word))
        return false;
    else
        return true;  // checks if this word is valid
}

bool Boggle::humanWordSearch(string word) {
    const string theWord = word;
    if (this->foundWords.contains(theWord)) return false;
    for (int irow = 0; irow < this->board.nRows; irow++) {
        for (int icol = 0; icol < this->board.nCols; icol++) {
            BoggleGUI::clearHighlighting();
            Set<position> used;
            position currentCube;
            currentCube.i = irow;
            currentCube.j = icol;
            if (getLetter(currentCube.i, currentCube.j) == word[0]) {
                char currentLetter = word[0];
                used.add(currentCube);
                word = word.substr(1, word.length() - 1);
                if (wordIsFoundH(word, used, currentCube)) {
                    this->foundWords.add(theWord);
                    BoggleGUI::recordWord(theWord, BoggleGUI::HUMAN);
                    BoggleGUI::setStatusMessage("You have found a new word");
                    this->hScore += (theWord.length() - 3);
                    BoggleGUI::setScore(this->hScore, BoggleGUI::HUMAN);
                    BoggleGUI::clearHighlighting();
                    return true;
                }
                word = currentLetter + word;
                used.remove(currentCube);
            }
        }
    }

    return false;
}

bool Boggle::wordIsFoundH(string &word, Set<position> &usedCubes, position &currentCube) {
    BoggleGUI::setHighlighted(currentCube.i, currentCube.j);
    BoggleGUI::setAnimationDelay(500);
    if (word == "") return true;
    Vector<position> neighs = neighboursPos(currentCube.i, currentCube.j);
    for (position p : neighs) {
        if (usedCubes.contains(p)) {
            continue;
        }

        char nextLetter = getLetter(p.i, p.j);
        if (nextLetter == word[0]) {
            word = word.substr(1, word.length() - 1);
            usedCubes.add(p);
            currentCube.i = p.i;
            currentCube.j = p.j;
            if (wordIsFoundH(word, usedCubes, currentCube)) {
                return true;
            }
            word = nextLetter + word;
            currentCube.i = p.i;
            currentCube.j = p.j;
            usedCubes.remove(p);
            BoggleGUI::setHighlighted(currentCube.i, currentCube.j, false);
            BoggleGUI::setAnimationDelay(500);
        }
    }

    return false;
}

int Boggle::getScoreHuman() {
    return this->hScore;
}

Set<string> Boggle::computerWordSearch() {
    Set<string> result;
    for (int irow = 0; irow < this->board.nRows; irow++) {
        for (int icol = 0; icol < this->board.nCols; icol++) {
            BoggleGUI::clearHighlighting();
            string wordSoFar;
            Set<position> used;
            position currentCube;
            currentCube.i = irow;
            currentCube.j = icol;
            char currentLetter = getLetter(currentCube.i, currentCube.j);
            wordSoFar += currentLetter;
            used.add(currentCube);
            findWords(wordSoFar, used, currentCube, result);
        }
    }
    return result;
}

void Boggle::findWords(string &word, Set<position> &usedCubes, position &currentCube, Set<string> &res) {
    //base case
    bool ok = deadEnd(word, usedCubes, currentCube, res);
}

bool Boggle::deadEnd(string &word, Set<position> &usedCubes, position &currentCube, Set<string> &res) {
    //base case
    BoggleGUI::setHighlighted(currentCube.i, currentCube.j);
    BoggleGUI::setAnimationDelay(500);

    if (!this->mydict.containsPrefix(word) || this->foundWords.contains(word)) return true;
    position origin = currentCube;
    Vector<position> neighs = neighboursPos(currentCube.i, currentCube.j);
    for (position p : neighs) {
        if (usedCubes.contains(p)) continue;
        usedCubes.add(p);
        char nextLetter = this->getLetter(p.i, p.j);
        word = word + nextLetter;
        currentCube = p;
        //found a word
        if (this->mydict.contains(word) && !this->foundWords.contains(word) && this->checkWord(word)) {
            res.add(word);
            this->cScore += word.length() - 3;
            BoggleGUI::setScore(this->cScore, BoggleGUI::COMPUTER);
        }
        if (deadEnd(word, usedCubes, currentCube, res)) {
            usedCubes.remove(p);
            word.erase(word.length() - 1);
            BoggleGUI::setHighlighted(currentCube.i, currentCube.j, false);
            BoggleGUI::setAnimationDelay(500);
            currentCube = origin;
        }
    }
    return true;
}

int Boggle::getScoreComputer() {
    return this->cScore;
}
Vector<position> Boggle::neighboursPos(int irow, int icol) {
    Vector<position> ret;
    position p;
    for (int i = irow - 1; i < irow + 2; i++) {
        for (int j = icol - 1; j < icol + 2; j++) {
            if (this->board.inBounds(i, j) && ((i != irow) || (j != icol))) {
                p.i = i;
                p.j = j;
                ret.add(p);
            }
        }
    }
    return ret;
}
ostream &operator<<(ostream &out, Boggle &boggle) {
    for (int irow = 0; irow < boggle.getRowSize(); irow++) {
        for (int icol = 0; icol < boggle.getColSize(); icol++) {
            out << boggle.getLetter(irow, icol);
        }
        out << endl;
    }
    return out;
}

bool operator<(const position p1, const position p2) {
    if (p1.i * boardSize + p1.j < p2.i * boardSize + p2.j)
        return true;
    else
        return false;
}
bool operator==(const position p1, const position p2) {
    if ((p1.i == p2.i) && (p1.j == p2.j))
        return true;
    else
        return false;
}
bool operator>(const position p1, const position p2) {
    if (p1 < p2)
        return false;
    else
        return true;
}
