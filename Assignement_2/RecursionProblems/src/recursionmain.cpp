/**
 * By Amar EL OUAZANI
 * 106b Stanford assignement Reccursion Training
 */

#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "gbufferedimage.h"
#include "gevents.h"
#include "ginteractors.h"
#include "gobjects.h"
#include "grid.h"
#include "gwindow.h"
#include "map.h"
#include "random.h"
#include "simpio.h"
#include "strlib.h"
#include "recursionproblems.h"
#include <cmath>
using namespace std;


// constants for human pyramid
const int HUMAN_PYRAMID_MIN_WEIGHT = 50;
const int HUMAN_PYRAMID_MAX_WEIGHT = 150;
//#define HUMAN_PYRAMID_RANDOM_SEED 106   // uncomment to 'rig' random number generator

// constants for Sierpinski triangle fractal
const int SIERPINSKI_WINDOW_WIDTH = 700;
const int SIERPINSKI_WINDOW_HEIGHT = 450;
const int SIERPINSKI_FIGURE_SIZE = 400;
const int SIERPINSKI_FIGURE_Y = 20;
const string SIERPINSKI_WINDOW_TITLE = "CS 106B Sierpinski Triangle";


//// provided helpers
//int getPixelColor(int x, int y);
//void setPixelColor(int x, int y, int color);

// testing function prototype declarations
void test_humanPyramid();
void test_sierpinskiTriangle();

int main() {
    cout << "CS 106B Recursion Problems" << endl;
    while (true) {
        cout << endl;
        cout << "Choose a problem:" << endl;
        cout << "1) Human Pyramid" << endl;
        cout << "2) Sierpinski Triangle" << endl;
        int choice = getInteger("Enter your choice (or 0 to quit): ");
        cout << endl;
        if (choice == 0) {
            break;
        }
        else if (choice == 1) {
            test_humanPyramid();
        }
        else if (choice == 2) {
            test_sierpinskiTriangle();
        }
    }

    cout << "Exiting" << endl;

    return 0;
}



/*
 * Runs and tests your humanPyramid function.
 */
void test_humanPyramid() {
    int cols = getInteger("How many people are on the bottom row? ");

    // possibly rig the random generator's output
#ifdef HUMAN_PYRAMID_RANDOM_SEED
    setRandomSeed(HUMAN_PYRAMID_RANDOM_SEED);
#endif // HUMAN_PYRAMID_RANDOM_SEED

    // populate vector of weights
    Vector<Vector<double> > weights;
    for (int row = 0; row < cols; row++) {
        Vector<double> currentRow;
        for (int col = 0; col <= row; col++) {
            double weight = randomReal(HUMAN_PYRAMID_MIN_WEIGHT, HUMAN_PYRAMID_MAX_WEIGHT);
            currentRow.add(weight);
        }
        weights.add(currentRow);
    }

    // print weights
    cout << "Each person's own weight:" << endl;
    cout << fixed << setprecision(2);
    for (int row = 0; row < weights.size(); row++) {
        for (int col = 0; col < weights[row].size(); col++) {
            cout << weights[row][col] << " ";
        }
        cout << endl;
    }
    cout << endl;

    // print weight on knees for each person in pyramid
    cout << "Weight on each person's knees:" << endl;
    for (int row = 0; row < weights.size(); row++) {
        for (int col = 0; col < weights[row].size(); col++) {
            double result;
            result = weightOnKnees(row, col, weights);
            cout << result << " ";
        }
        cout << endl;
    }
    cout << resetiosflags(ios::fixed | ios::floatfield);
}

/*
 * Runs and tests your drawSierpinskiTriangle function.
 * Prompts the user for the fractal's order, then displays it.
 */
void test_sierpinskiTriangle() {
    int order = getInteger("Fractal order? ");
    GWindow gw(SIERPINSKI_WINDOW_WIDTH, SIERPINSKI_WINDOW_HEIGHT);
    gw.setWindowTitle(SIERPINSKI_WINDOW_TITLE);
    int x0 = (SIERPINSKI_WINDOW_WIDTH - SIERPINSKI_FIGURE_SIZE) / 2;  // center
    int y0 = SIERPINSKI_FIGURE_Y;

    try {
        drawSierpinskiTriangle(gw, x0, y0, SIERPINSKI_FIGURE_SIZE, order);
    } catch (string) {
        cout << "Invalid parameters passed! (threw an exception)" << endl;
        gw.close();
    } catch (const char*) {
        cout << "Invalid parameters passed! (threw an exception)" << endl;
        gw.close();
    } catch (ErrorException) {
        cout << "Invalid parameters passed! (threw an exception)" << endl;
        gw.close();
    }
}

