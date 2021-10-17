/**
 * By Amar EL OUAZANI
 * 106b Stanford assignement GameOfLife
 */

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "lifegui.h"
#include "random.h"
#include "simpio.h"

using namespace std;

const int ms = 5;

void inputFileReeader(ifstream& in, Grid<char>& myGrid, LifeGUI& myGui);
void oneCycle(Grid<char>& myGrid);
int countNeighbours(Grid<char>& myGrid, int i, int j);
void outOneCycle(Grid<char>& myGrid, LifeGUI& myGui);
void interfacePop(string& rep, int& nanim);

int main() {
    ifstream in;
    Grid<char> myGrid;
    LifeGUI myGui;
    inputFileReeader(in, myGrid, myGui);
    outOneCycle(myGrid, myGui);
    string rep;
    int nanim = 1;
    interfacePop(rep, nanim);
    while (true) {
        if (rep[0] == 'a') {
            for (int icyc = 0; icyc < nanim; icyc++) {
                oneCycle(myGrid);
                outOneCycle(myGrid, myGui);
                pause(ms);
            }
            interfacePop(rep, nanim);
        } else if (rep[0] == 't') {
            oneCycle(myGrid);
            outOneCycle(myGrid, myGui);
            pause(ms);
            interfacePop(rep, nanim);
        } else if (rep[0] == 'q')
            break;
    }
    return 0;
}

void inputFileReeader(ifstream& in, Grid<char>& myGrid, LifeGUI& myGui) {
    string line;
    int row;
    int col;

    cout << "Welcome to Amar game of life" << endl;
    cout << "Simulation of cells colonny" << endl;
    cout << "A location that has zero or one neighbors will be empty in the next generation. If a cell was there, it dies." << endl;
    cout << "location with two neighbors is stable. If it had a cell, it still contains a cell. If it was empty, it s still empty." << endl;
    cout << "A location with three neighbors will contain a cell in the next generation. If it was unoccupied before, a new cell is born. If it currently contains a cell, the cell remains." << endl;
    cout << "A location with four or more neighbors will be empty in the next generation. If there was a cell in that location, it dies of overcrowding." << endl;

    cout << "----Grid colonny file----" << endl;

    string namefile = getLine("name of file colony: ");
    if (namefile == "random") {
        //        row = randomInteger(1,200);
        //        col = randomInteger(1,200);
        row = 100;
        col = 100;
        myGrid.resize(row, col);
        myGui.resize(row, col);
        for (int irow = 0; irow < myGrid.numRows(); irow++) {
            for (int icol = 0; icol < myGrid.numCols(); icol++) {
                if (randomBool()) {
                    myGrid.set(irow, icol, 'X');
                    myGui.drawCell(irow, icol, true);
                } else {
                    myGrid.set(irow, icol, '-');
                    myGui.drawCell(irow, icol, false);
                }
            }
        }
    } else {
        in.open(namefile);
        getline(in, line);
        row = stringToInteger(line);
        getline(in, line);
        col = stringToInteger(line);
        myGrid.resize(row, col);
        for (int irow = 0; irow < myGrid.numRows(); irow++) {
            getline(in, line);
            for (int icol = 0; icol < myGrid.numCols(); icol++) {
                myGrid.set(irow, icol, line[icol]);
                if (line[icol] == 'X')
                    myGui.drawCell(irow, icol, true);
                else
                    myGui.drawCell(irow, icol, false);
            }
        }
    }
}

void oneCycle(Grid<char>& myGrid) {
    Grid<char> nextGrid = myGrid;
    for (int irow = 0; irow < myGrid.numRows(); irow++) {
        for (int icol = 0; icol < myGrid.numCols(); icol++) {
            int e = countNeighbours(myGrid, icol, irow);
            switch (e) {
                case 0:
                    nextGrid.set(irow, icol, '-');
                    break;
                case 1:
                    nextGrid.set(irow, icol, '-');
                    break;
                case 2:
                    nextGrid.set(irow, icol, myGrid.get(irow, icol));
                    break;
                case 3:
                    nextGrid.set(irow, icol, 'X');
                    break;
                case 4:
                    nextGrid.set(irow, icol, '-');
                    break;
            }
        }
    }

    myGrid = nextGrid;
}

int countNeighbours(Grid<char>& myGrid, int i, int j) {
    int ret = 0;
    for (int ir = i - 1; ir < i + 2; ir++) {
        for (int ic = j - 1; ic < j + 2; ic++) {
            if ((ic == j) && (ir == i)) continue;
            if (myGrid.inBounds(ic, ir) && myGrid.get(ic, ir) == 'X') ret++;
            if (!myGrid.inBounds(ic, ir)) {
                int ict, irt;
                if (ir == -1)
                    irt = myGrid.numRows() - 1;
                else if (ir == myGrid.numRows())
                    irt = 0;
                if (ic == -1)
                    ict = myGrid.numCols() - 1;
                else if (ic == myGrid.numCols())
                    ict = 0;

                if (myGrid.get(irt, ict) == 'X') ret++;
            }
        }
    }
    if (ret > 4) ret = 4;
    return ret;
}

void outOneCycle(Grid<char>& myGrid, LifeGUI& myGui) {
    for (int irow = 0; irow < myGrid.numRows(); irow++) {
        for (int icol = 0; icol < myGrid.numCols(); icol++) {
            if (myGrid[irow][icol] == 'X')
                myGui.drawCell(irow, icol, true);
            else if (myGrid[irow][icol] == '-')
                myGui.drawCell(irow, icol, false);
        }
    }
}
void interfacePop(string& rep, int& nanim) {
    while (true) {
        rep = getLine("a)nimate, t)ick, q)uit?");
        if ((rep[0] == 'a') && rep.length() == 1) {
            nanim = getInteger("How many frames?");
            break;
        } else if ((rep[0] == 't' || rep[0] == 'q') && rep.length() == 1)
            break;
    }
}
