#include "QueueList.h"

#include "Customer.h"

QueueList::QueueList() { head = nullptr; }

QueueList::QueueList(Customer* customer) { head = customer; }

QueueList::~QueueList() { empty_queue(); }

Customer* QueueList::get_head() { return head; }

void QueueList::enqueue(Customer* customer) {
  // a customer is placed at the end of the queue
  // if the queue is empty, the customer becomes the head
  if (head == nullptr) {
    head = customer;
  } else {
    Customer* temp = head;
    while (temp->get_next() != nullptr) {
      temp = temp->get_next();
    }
    temp->set_next(customer);
    customer->set_next(nullptr);
  }
}

Customer* QueueList::dequeue() {
  // remove a customer from the head of the queue
  // and return a pointer to it
  if (head == nullptr) {
    return nullptr;
  } else {
    Customer* temp = head;
    head = head->get_next();
    temp->set_next(nullptr);
    // pointer to temp is the removed customer; used to add to doneList
    return temp;
  }
}

int QueueList::get_items() {
  // count total number of items each customer in the queue has
  Customer* temp = head;
  int items = 0;
  while (temp != nullptr) {
    items += temp->get_numOfItems();
    temp = temp->get_next();
  }
  return items;
}

bool QueueList::empty_queue() {
  // if the queue is empty, return false
  // if the queue is not empty, delete all customers
  // and return true
  if (head == nullptr) {
    return false;
  } else {
    Customer* temp = head;
    while (temp != nullptr) {
      Customer* temp2 = temp->get_next();
      delete temp;
      temp = temp2;
    }
    head = nullptr;
    return true;
  }
}

void QueueList::print() {
  // print customers in a queue
  Customer* temp = head;
  while (temp != nullptr) {
    temp->print();
    temp = temp->get_next();
  }
}
