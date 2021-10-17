/**
 * By Amar EL OUAZANI
 * 106b Stanford assignement SLinkedPriorityQueue
 */

#include "SLinkedPriorityQueue.h"

SLinkedPriorityQueue::SLinkedPriorityQueue() {
    this->front = NULL;

}

SLinkedPriorityQueue::~SLinkedPriorityQueue() {
    this->clear();

}

void SLinkedPriorityQueue::changePriority(string value, int newPriority) {
    ListNode* current = this->front;
    bool done = false;
    if(this->front->value == value && this->front->priority > newPriority){
        this->dequeue();
        this->enqueue(value, newPriority);
        done = true;
    }
    else{
        while(current->next){
            if(current->next->value == value && this->front->priority > newPriority){
                ListNode* tail = current->next->next;
                delete current->next;
                current->next = tail;
                enqueue(value, newPriority);
                done= true;
                break;
            }
            else {
                current = current->next;
            }
        }
    }
    if(!done) throw string("either priority " + to_string(newPriority)+ " is bigger than it should or the element is not in the PQ");
}

void SLinkedPriorityQueue::clear() {
    while(this->front){
        ListNode* next = this->front->next;
        delete this->front;
        this->front = next;
    }
}

string SLinkedPriorityQueue::dequeue() {
    if(!this->isEmpty()){
        string ret = this->front->value;
        ListNode* newfront;
        newfront = this->front->next;
        delete this->front;
        this->front = newfront;

        return ret;
    }
    else throw string("this PQ is empty, you cannot dequeue it");
}

void SLinkedPriorityQueue::enqueue(string value, int priority) {
    ListNode* toAdd = new ListNode(value, priority, NULL);
    bool added = false;
    ListNode* current = this->front;
    //if toAdd is the first one
    if(this->front == NULL) this->front = new ListNode(value, priority);
    //if the toAdd is the most urgent
    else if(*toAdd < *this->front){
        ListNode* tail = this->front;
        this->front = toAdd;
        toAdd->next = tail;
        added =true;
    }
    else {
        //if toAdd is in the middle
        while(current->next){
            if( *toAdd <*current->next){
                ListNode* tail = current->next;
                current->next = toAdd;
                toAdd->next = tail;
                added = true;
                break;
            }
            else current = current->next;
        }
        //if to add is at the end
        if(!added){
            current->next = toAdd;
        }
    }
}


bool SLinkedPriorityQueue::isEmpty() const {
    if(front) return false;
    else return true;
}

string SLinkedPriorityQueue::peek() const {
    string ret = this->front->value;
    return ret;   // remove this
}

int SLinkedPriorityQueue::peekPriority() const {
    int ret = this->front->priority;
    return ret;
}

int SLinkedPriorityQueue::size() const {
    int ret = 0;
    ListNode* next = this->front;
    while(next){
        ret++;
        next = next->next;
    }
    return ret;
}

ostream& operator<<(ostream& out, const SLinkedPriorityQueue& queue) {
    ListNode* current = queue.front;
    while(current){
        out<<current->value<<", ";
        current = current->next;
    }
    out<<endl;
    return out;
}
