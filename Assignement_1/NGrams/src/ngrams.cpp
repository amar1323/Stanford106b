/**
 * By Amar EL OUAZANI
 * 106b Stanford assignement Ngrams
 */

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

#include "console.h"
#include "map.h"
#include "queue.h"
#include "random.h"
#include "set.h"
#include "simpio.h"
using namespace std;
void userInterface(ifstream& input, int& nGram);
void constructNgram(Map<Vector<string>, Vector<string> >& mapNGrams, int nGram, ifstream& input);
void outPut(Vector<string>& myText);
int main() {
    ifstream input;
    int nGram, nText;
    bool ok = true;
    Map<Vector<string>, Vector<string> > mapNGrams;
    Vector<string> myText;
    userInterface(input, nGram);
    constructNgram(mapNGrams, nGram, input);
    while (ok) {
        nText = getInteger("# of random words to generate (0 to quit)? ");
        if (nText) {
            if (nText < nGram) {
                myText += randomKey(mapNGrams);
                myText = myText.subList(0, nText);
            } else {
                myText += randomKey(mapNGrams);
                while (myText.size() < nText) {
                    Vector<string> base = myText.subList((myText.size()) - (nGram - 1), nGram - 1);
                    string nextWord = randomElement(mapNGrams[base]);
                    myText += nextWord;
                }
            }
            outPut(myText);
            myText.clear();
        } else
            ok = false;
    }
    return 0;
}
void userInterface(ifstream& input, int& nGram) {
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl;
    string fileName;
    while (true) {
        fileName = getLine("Input file name?");
        input.open(fileName);
        if (!input.fail()) break;
    }
    nGram = getInteger("Value of N?");
}

void constructNgram(Map<Vector<string>, Vector<string> >& mapNGrams, int nGram, ifstream& input) {
    Vector<string> window;
    string word;
    for (int i = 0; i < nGram - 1; i++) {
        input >> word;
        window += word;
    }
    while (input >> word) {
        mapNGrams[window] += word;
        window.remove(0);
        window.add(word);
    }
}

void outPut(Vector<string>& myText) {
    cout << "... ";
    for (int i = 0; i < myText.size(); i++) {
        cout << myText[i] << " ";
    }
    cout << "..." << endl;
}
