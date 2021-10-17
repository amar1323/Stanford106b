/**
 * By Amar EL OUAZANI
 * 106b Stanford assignement HuffmanEncoding
 */

#include "encoding.h"

#include "filelib.h"
#include "lexicon.h"
#include "pqueue.h"
Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> freqTable;
    while (!input.fail()) {
        int singleBit = input.get();
        if (singleBit == -1)
            freqTable[PSEUDO_EOF]++;
        else
            freqTable[singleBit]++;
    }

    rewindStream(input);
    return freqTable;
}

HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    PriorityQueue<HuffmanNode*> pq;
    HuffmanNode* ret;

    for (int charac : freqTable.keys()) {
        HuffmanNode* toAdd = new HuffmanNode(charac, freqTable[charac]);
        pq.enqueue(toAdd, toAdd->count);
    }
    while (pq.size() > 1) {
        HuffmanNode* zero = pq.dequeue();
        HuffmanNode* one = pq.dequeue();
        HuffmanNode* toAdd = new HuffmanNode(NOT_A_CHAR, zero->count + one->count, zero, one);
        pq.enqueue(toAdd, toAdd->count);
    }
    ret = pq.dequeue();
    return ret;
}

Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;
    HuffmanNode* current = encodingTree;
    string soFar;
    transversalWalk(current, soFar, encodingMap);
    return encodingMap;
}

void transversalWalk(HuffmanNode* current, string soFar, Map<int, string>& encodingMap) {
    if (current->one != NULL || current->zero != NULL) {
        soFar = soFar + "0";
        transversalWalk(current->zero, soFar, encodingMap);
        soFar.erase(soFar.length() - 1);
        soFar = soFar + "1";
        transversalWalk(current->one, soFar, encodingMap);
    } else {
        encodingMap[current->character] = soFar;
    }
}
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    string currentEncod;
    while (!input.fail()) {
        currentEncod = encodingMap[input.get()];
        for (int ichar = 0; ichar < currentEncod.length(); ichar++) {
            if (currentEncod[ichar] == '0')
                output.writeBit(0);
            else if (currentEncod[ichar] == '1')
                output.writeBit(1);
        }
    }
    currentEncod.clear();
    currentEncod = encodingMap[PSEUDO_EOF];
    for (int ichar = 0; ichar < currentEncod.length(); ichar++) {
        if (currentEncod[ichar] == '0')
            output.writeBit(0);
        else if (currentEncod[ichar] == '1')
            output.writeBit(1);
    }
    rewindStream(input);
}

void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    int dir;
    HuffmanNode* root = encodingTree;
    while (!input.fail()) {
        dir = input.readBit();
        if (dir == 0) {
            if (encodingTree->zero == NULL) {
                if (encodingTree->character == PSEUDO_EOF) break;
                char* toAdd = new char(encodingTree->character);
                output.write(toAdd, 1);
                encodingTree = root->zero;
            } else {
                encodingTree = encodingTree->zero;
            }
        } else if (dir == 1) {
            if (encodingTree->one == NULL) {
                if (encodingTree->character == PSEUDO_EOF) break;
                char* toAdd = new char(encodingTree->character);
                output.write(toAdd, 1);
                encodingTree = root->one;
            } else {
                encodingTree = encodingTree->one;
            }
        }
    }
}

void compress(istream& input, obitstream& output) {
    Map<int, int> freqTable = buildFrequencyTable(input);
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    HuffmanNode* trash = encodingTree;
    Map<int, string> encodingMap = buildEncodingMap(encodingTree);
    output << freqTable;
    encodeData(input, encodingMap, output);
    freeTree(trash);
}

void decompress(ibitstream& input, ostream& output) {
    Map<int, int> freqTable;
    input >> freqTable;
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    HuffmanNode* trash = encodingTree;
    decodeData(input, encodingTree, output);
    freeTree(trash);
}

void freeTree(HuffmanNode* node) {
    if (node) {
        HuffmanNode* leftTail = node->zero;
        HuffmanNode* rightTail = node->one;
        delete node;
        freeTree(leftTail);
        freeTree(rightTail);
    }
}
