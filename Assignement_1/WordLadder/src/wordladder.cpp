/**
 * By Amar EL OUAZANI
 * 106b Stanford assignement wordLadder
 */

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

#include "console.h"
#include "queue.h"
#include "set.h"
#include "simpio.h"
#include "stack.h"
using namespace std;

void userInterface(string& word1, string& word2, Set<string>& myDict, bool& ok);
Stack<string> neighboors(string w, Set<string>& myDict);
void outPutSol(string& currneigh, Stack<string>& ladder, bool& ladderexist);

int main() {
    cout << "Welcome to Amar word ladder" << endl;
    cout << "Give me two english words and i'll change" << endl;
    cout << "the first into the seconde by changing one letter at a time" << endl;
    string w1;
    string w2;
    Set<string> dictionary;
    Queue<Stack<string> > wordLadder;
    Stack<string> tempLadder;
    string dicName;
    ifstream inputDic;
    string dicWord;
    Set<string> wordUsed;
    bool ok = true;
    bool ladderexist;
    while (true) {
        dicName = getLine("Dictionary file name? ");
        inputDic.open(dicName);
        if (inputDic.fail()) {
            cout << "file doesn't exist" << endl;
        } else {
            break;
        }
    }
    while (inputDic >> dicWord) {
        dictionary.add(dicWord);
    }
    while (ok) {
        ladderexist = false;
        userInterface(w1, w2, dictionary, ok);
        tempLadder.add(w1);
        wordLadder.add(tempLadder);
        wordUsed.add(w1);
        while (!wordLadder.isEmpty()) {
            Stack<string> partielLadder = wordLadder.dequeue();
            string word = partielLadder.peek();
            Stack<string> neigh = neighboors(word, dictionary);
            for (int ineigh = neigh.size() - 1; ineigh >= 0; ineigh--) {
                string currentNeigh = neigh.pop();
                if (!wordUsed.contains(currentNeigh) && currentNeigh != w1) {
                    if (currentNeigh == w2) {
                        outPutSol(currentNeigh, partielLadder, ladderexist);
                        wordLadder.clear();
                        break;
                    } else {
                        Stack<string> copy = partielLadder;
                        copy.add(currentNeigh);
                        wordLadder.enqueue(copy);
                        wordUsed.add(currentNeigh);
                    }
                }
            }
        }
        if (!ladderexist && ok) cout << "word ladder doesn't existe" << endl;
        tempLadder.clear();
        wordUsed.clear();
    }

    cout << "have a nice day" << endl;
    return 0;
}

void userInterface(string& word1, string& word2, Set<string>& myDict, bool& ok) {
    while (true) {
        word1 = getLine("Word #1 (or q to quit): ");
        word1 = toLowerCase(word1);
        if (word1 == "q") {
            ok = false;
            break;
        } else if (!myDict.contains(word1)) {
            cout << "Word #1 isn't in the dictionary!" << endl;
            continue;
        }
        word2 = getLine("Word #2 (or q to quit): ");
        word2 = toLowerCase(word2);
        if (word2 == "q") {
            ok = false;
            break;
        } else if (!myDict.contains(word2)) {
            cout << "Word #2 isn't in the dictionary!" << endl;
        } else if (word1 == word2) {
            cout << "word #1 is exactly word #2" << endl;
        } else
            break;
    }
}
Stack<string> neighboors(string w, Set<string>& myDict) {
    string alphabet = "abcdefghijklmnopqrstuvwxyz";
    Stack<string> ret;
    for (char c : alphabet) {
        for (int ic = 0; ic < w.length(); ic++) {
            string newW = w;
            newW[ic] = c;
            if (myDict.contains(newW) && newW != w) ret.add(newW);
        }
    }
    return ret;
}

void outPutSol(string& currneigh, Stack<string>& ladder, bool& ladderexist) {
    cout << "your word ladder" << endl;
    cout << currneigh;
    for (int i = ladder.size() - 1; i >= 0; i--) {
        cout << "->" << ladder.pop();
    }
    cout << endl;
    ladderexist = true;
}
