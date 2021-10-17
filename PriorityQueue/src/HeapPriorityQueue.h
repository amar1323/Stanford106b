/**
 * By Amar EL OUAZANI
 * 106b Stanford assignement HeapPriorityQueue
 */

#ifndef _heappriorityqueue_h
#define _heappriorityqueue_h

#include <iostream>
#include <string>

#include "PQEntry.h"
using namespace std;

/*
 * For documentation of each member, see VectorPriorityQueue.h.
 */
class HeapPriorityQueue {
   public:
    HeapPriorityQueue();
    ~HeapPriorityQueue();
    void changePriority(string value, int newPriority);
    void clear();
    string dequeue();
    void enqueue(string value, int priority);
    bool isEmpty() const;
    string peek() const;
    int peekPriority() const;
    int size() const;
    friend ostream& operator<<(ostream& out, const HeapPriorityQueue& queue);

   private:
    PQEntry* elements;
    int arraySize;
    int hQsize;
    void adjust();
    void bubbleUp(int& child);
    void bubbleDown(int& parent);
};

#endif
