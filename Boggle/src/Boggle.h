/**
 * By Amar EL OUAZANI
 * 106b Stanford assignement Boggle
 */

#ifndef _boggle_h
#define _boggle_h

#include <cmath>
#include <iostream>
#include <string>

#include "grid.h"
#include "lexicon.h"
#include "random.h"
#include "set.h"
#include "simpio.h"
#include "vector.h"

using namespace std;
struct position {
    int i;
    int j;
};
bool operator<(const position p1, const position p2);
bool operator==(const position p1, const position p2);
bool operator>(const position p1, const position p2);

class Boggle {
   public:
    Boggle(Lexicon& dictionary, string boardText = "");
    char getLetter(int row, int col);
    int getRowSize();
    int getColSize();
    bool checkWord(string word);
    bool humanWordSearch(string word);
    Set<string> computerWordSearch();
    int getScoreHuman();
    int getScoreComputer();
    Vector<position> getPositions(char c);
    // TODO: add any other member functions/variables necessary
    friend ostream& operator<<(ostream& out, Boggle& boggle);

   private:
    Grid<char> board;
    int col, row;
    Lexicon mydict;
    Lexicon foundWords;
    Lexicon hWords;
    bool wordIsFoundH(string& word, Set<position>& usedCubes, position& currentCube);
    void findWords(string& word, Set<position>& usedCubes, position& currentCube, Set<string>& res);
    bool deadEnd(string& word, Set<position>& usedCubes, position& currentCube, Set<string>& res);
    Vector<position> neighboursPos(int icol, int irow);
    int hScore;
    int cScore;
};

#endif  // _boggle_h
