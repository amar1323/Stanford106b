/**
 * By Amar EL OUAZANI
 * 106b Stanford assignement USLinkedPriorityQueue
 */


#ifndef _uslinkedpriorityqueue_h
#define _uslinkedpriorityqueue_h

#include <cstddef>    // for NULL
#include <iostream>
#include <string>
#include "ListNode.h"
#include "PQEntry.h"
using namespace std;

/*
 * For documentation of each member, see VectorPriorityQueue.h.
 */
class USLinkedPriorityQueue {
public:
    USLinkedPriorityQueue();
    ~USLinkedPriorityQueue();
    void changePriority(string value, int newPriority);
    void clear();
    string dequeue();
    void enqueue(string value, int priority);
    bool isEmpty() const;
    string peek() const;
    int peekPriority() const;
    int size() const;
    friend ostream& operator <<(ostream& out, const USLinkedPriorityQueue& queue);
private:
    ListNode* front;
    ListNode* getTheMostUrgent() const;

};

#endif
