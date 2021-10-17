// This is the .cpp file you will edit and turn in.
//
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

// Please feel free to add any other #includes you need!
#include "recursionproblems.h"
#include <cmath>
#include <iostream>
#include "hashmap.h"
#include "map.h"
#include "random.h"
using namespace std;


double weightOnKnees(int row, int col, const Vector<Vector<double> >& weights) {
    if(row<0 || col < 0 || col >row) return 0.;
    else return weights[row][col] + 0.5* weightOnKnees(row-1,col,weights) + 0.5*weightOnKnees(row-1,col-1,weights);
}


void drawSierpinskiTriangle(GWindow& gw, double x, double y, int size, int order) {
    if(order ==1 ){
        gw.drawLine(x, y, x+size, y);
        gw.drawLine(x+size, y, 0.5*size+x, y + sin(60*M_PI/180)*size );
        gw.drawLine(0.5*size+x, y + sin(60*M_PI/180)*size, x, y);
    }
    else{
        drawSierpinskiTriangle(gw, x, y, 0.5*size, order-1);
        drawSierpinskiTriangle(gw, x + size*0.5, y, 0.5*size, order-1);
        drawSierpinskiTriangle(gw, x + 0.5*size*cos(60*M_PI/180), y + 0.5*size*sin(60*M_PI/180), 0.5*size, order-1);
    }

}

