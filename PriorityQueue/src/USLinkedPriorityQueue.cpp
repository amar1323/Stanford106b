/**
 * By Amar EL OUAZANI
 * 106b Stanford assignement USLinkedPriorityQueue
 */

#include "USLinkedPriorityQueue.h"
USLinkedPriorityQueue::USLinkedPriorityQueue() {
    this->front = NULL;
}

USLinkedPriorityQueue::~USLinkedPriorityQueue() {
    this->clear();
}

void USLinkedPriorityQueue::changePriority(string value, int newPriority) {
    bool done = false;
    ListNode* current = this->front;
    if(current->value == value && current->priority > newPriority){
        current->priority = newPriority;
        done = true;
    }
    else {
        while(current->next){
            if(current->next->value == value && current->priority > newPriority){
                current->next->priority = newPriority;
                done = true;
                break;
            }
            else {
                current = current->next;
            }
        }
    }
    if(!done) throw string("either priority " + to_string(newPriority)+ " is bigger than it should or the element is not in the PQ");

}

void USLinkedPriorityQueue::clear() {
    while(this->front){
        ListNode* next = this->front->next;
        delete this->front;
        this->front = next;
    }
}

string USLinkedPriorityQueue::dequeue() {
    if(this->isEmpty()) throw string("The queue is empty");
    else{
        ListNode* toDeq = this->getTheMostUrgent();
        ListNode* current = this->front;
        string ret = toDeq->value;
        if(this->front == toDeq){
            ListNode* tail = this->front->next;
            delete current;
            this->front =tail;
        }
        else{
            while(current->next){
                if(current->next == toDeq){
                    ListNode* tail = current->next->next;
                    delete current->next;
                    current->next = tail;
                    break;
                }
                else current = current->next;
            }
        }
        return ret;
    }
}

void USLinkedPriorityQueue::enqueue(string value, int priority) {
    ListNode* toAdd = new ListNode(value, priority, NULL);
    //if toAdd is the first one
    if(this->front == NULL) this->front = new ListNode(value, priority);
    else {
        ListNode* tail = this->front;
        this->front = toAdd;
        toAdd->next = tail;
    }
}

bool USLinkedPriorityQueue::isEmpty() const {
    if(this->front) return false;
    else return true;
}
ListNode* USLinkedPriorityQueue::getTheMostUrgent() const{
    ListNode* current = this->front;
    ListNode* theUrgent = this->front;
    while(current){
        if(current->priority < theUrgent->priority) theUrgent = current;
        current = current->next;
    }
    return theUrgent;
}

int USLinkedPriorityQueue::peekPriority() const {
    return this->getTheMostUrgent()->priority;
}
string USLinkedPriorityQueue::peek() const {
    return getTheMostUrgent()->value;
}

int USLinkedPriorityQueue::size() const {
    int ret = 0;
    ListNode* next = this->front;
    while(next){
        ret++;
        next = next->next;
    }
    return ret;
}

ostream& operator<<(ostream& out, const USLinkedPriorityQueue& queue) {
    ListNode* current = queue.front;
    while(current){
        out<<current->value<<", ";
        current = current->next;
    }
    out<<endl;
    return out;
}
