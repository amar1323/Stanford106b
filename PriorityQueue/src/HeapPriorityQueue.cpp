/**
 * By Amar EL OUAZANI
 * 106b Stanford assignement HeapPriorityQueue
 */

#include "HeapPriorityQueue.h"

HeapPriorityQueue::HeapPriorityQueue() {
    this->arraySize = 10;
    this->hQsize = 1;
    this->elements = new PQEntry[arraySize];
}

HeapPriorityQueue::~HeapPriorityQueue() {
    this->clear();
}

void HeapPriorityQueue::changePriority(string value, int newPriority) {
    bool done = false;
    for (int i = 1; i < hQsize; i++) {
        if (this->elements[i].value == value && this->elements[i].priority > newPriority) {
            int current = i;
            this->elements[i].priority = newPriority;
            int potentielParent = i / 2;
            if (this->elements[current] < this->elements[potentielParent]) {
                this->bubbleUp(current);
                done = true;
                break;
            }
        }
    }
    if (!done) throw string("the new priority is higher or the value doesn't exist");
}

void HeapPriorityQueue::clear() {
    delete[] this->elements;
}
void HeapPriorityQueue::adjust() {
    const int sz = 2 * this->arraySize;
    PQEntry* newElements = new PQEntry[sz];
    for (int i = 1; i < hQsize; i++) {
        newElements[i] = this->elements[i];
    }
    delete[] elements;
    this->elements = newElements;
    arraySize = sz;
}
string HeapPriorityQueue::dequeue() {
    if (!this->isEmpty()) {
        string ret = this->elements[1].value;
        this->elements[1] = this->elements[hQsize - 1];
        hQsize--;
        int parent = 1;
        this->bubbleDown(parent);
        return ret;
    } else
        throw string("this Heap priority queue is empty");
}
void HeapPriorityQueue::bubbleDown(int& parent) {
    while (true) {
        int fChild, sChild, child;
        if (parent * 2 + 1 < hQsize) {
            fChild = 2 * parent;
            sChild = 2 * parent + 1;
        } else
            break;
        if (this->elements[fChild] > this->elements[sChild])
            child = sChild;
        else
            child = fChild;
        if (this->elements[parent] > this->elements[child]) {
            PQEntry temp;
            temp = this->elements[parent];
            this->elements[parent] = this->elements[child];
            this->elements[child] = temp;
            parent = child;
        } else
            break;
    }
}

void HeapPriorityQueue::enqueue(string value, int priority) {
    //if it is the first element to add
    if (this->isEmpty()) {
        this->elements[1] = new PQEntry(value, priority);
        hQsize++;
    } else {
        //if array if full
        if (hQsize >= arraySize) this->adjust();
        //enqueue at the end
        this->elements[hQsize] = new PQEntry(value, priority);
        int child = hQsize;
        this->hQsize++;
        this->bubbleUp(child);
    }
}

void HeapPriorityQueue::bubbleUp(int& child) {
    int parent = child / 2;
    if (parent == 0) parent += 1;
    //bubble down
    while (true) {
        if (this->elements[child] < this->elements[parent]) {
            PQEntry temp = this->elements[parent];
            this->elements[parent] = this->elements[child];
            this->elements[child] = temp;
            child = parent;
            parent = child / 2;
            if (parent == 0) parent += 1;
        } else
            break;
    }
}

bool HeapPriorityQueue::isEmpty() const {
    if (this->hQsize == 1)
        return true;
    else
        return false;
}

string HeapPriorityQueue::peek() const {
    return this->elements[1].value;
}

int HeapPriorityQueue::peekPriority() const {
    return this->elements[1].priority;
}

int HeapPriorityQueue::size() const {
    return hQsize;  // remove this
}

ostream& operator<<(ostream& out, const HeapPriorityQueue& queue) {
    for (int i = 1; i < queue.hQsize; i++) {
        out << queue.elements[i].value << " ";
    }
    out << endl;
    return out;
}
