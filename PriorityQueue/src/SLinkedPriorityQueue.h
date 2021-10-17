/**
 * By Amar EL OUAZANI
 * 106b Stanford assignement SLinkedPriorityQueue
 */

#ifndef _slinkedpriorityqueue_h
#define _slinkedpriorityqueue_h

#include <cstddef>    // for NULL
#include <iostream>
#include <string>
#include "ListNode.h"
#include "PQEntry.h"
using namespace std;

/*
 * For documentation of each member, see VectorPriorityQueue.h.
 */
class SLinkedPriorityQueue {
public:
    SLinkedPriorityQueue();
    ~SLinkedPriorityQueue();
    void changePriority(string value, int newPriority);
    void clear();
    string dequeue();
    void enqueue(string value, int priority);
    bool isEmpty() const;
    string peek() const;
    int peekPriority() const;
    int size() const;
    friend ostream& operator <<(ostream& out, const SLinkedPriorityQueue& queue);

private:
    ListNode* front;

};

#endif
