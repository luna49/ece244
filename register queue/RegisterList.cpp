#include "RegisterList.h"

#include <iostream>

#include "Register.h"
using namespace std;

RegisterList::RegisterList() {
  head = nullptr;
  size = 0;
}

RegisterList::~RegisterList() {
  // Delete all registers in the list
  if (head != nullptr) {
    Register* temp = head;
    while (temp != nullptr) {
      Register* temp2 = temp;
      temp = temp->get_next();
      delete temp2;
    }
  }
}

Register* RegisterList::get_head() { return head; }

int RegisterList::get_size() {
  // return number of registers
  return size;
}

Register* RegisterList::get_min_items_register() {
  // loop all registers to find the register with least number of items
  if (head == nullptr) {
    return nullptr;
  } else {
    Register* temp = head;
    Register* min = head;
    while (temp != nullptr) {
      if (temp->get_queue_list()->get_items() <
          min->get_queue_list()->get_items()) {
        min = temp;
      }
      temp = temp->get_next();
    }
    return min;
  }
}

Register* RegisterList::get_free_register() {
  // return the register with no customers
  // if all registers are occupied, return nullptr
  if (head == nullptr) {
    return nullptr;
  } else {
    Register* temp = head;
    while (temp != nullptr) {
      if (temp->get_queue_list()->get_head() == nullptr) {
        return temp;
      }
      temp = temp->get_next();
    }
    return nullptr;
  }
}

void RegisterList::enqueue(Register* newRegister) {
  // a register is placed at the end of the queue
  // if the register's list is empty, the register becomes the head
  // Assume the next of the newRegister is set to null
  // You will have to increment size
  if (head == nullptr) {
    head = newRegister;
    newRegister->set_next(nullptr);
  } else {
    Register* temp = head;
    while (temp->get_next() != nullptr) {
      temp = temp->get_next();
    }
    temp->set_next(newRegister);
    size++;
    newRegister->set_next(nullptr);
  }
}

bool RegisterList::foundRegister(int ID) {
  // look for a register with the given ID
  // return true if found, false otherwise
  if (head == nullptr) {
    return false;
  } else {
    Register* temp = head;
    while (temp != nullptr) {
      if (temp->get_ID() == ID) {
        return true;
      }
      temp = temp->get_next();
    }
    return false;
  }
}

Register* RegisterList::dequeue(int ID) {
  // dequeue the register with given ID

  // return the dequeued register
  // return nullptr if register was not found
  if (head == nullptr) {
    return nullptr;
  } else {
    Register* temp = head;
    Register* prev = nullptr;
    while (temp != nullptr) {
      if (temp->get_ID() == ID) {
        if (prev == nullptr) {
          head = temp->get_next();
          size--;
          return temp;
        } else {
          prev->set_next(temp->get_next());
          size--;
          return temp;
        }
      }
      prev = temp;
      temp = temp->get_next();
    }
    return nullptr;
  }
}

Register* RegisterList::calculateMinDepartTimeRegister(double expTimeElapsed) {
  // return the register with minimum time of departure of its customer
  // if all registers are free, return nullptr
  if (head == nullptr) {
    return nullptr;
  } else {
    Register* temp = head;
    Register* min = nullptr;

    double departTime = 10000000000000;

    while (temp != nullptr) {
      if (temp->calculateDepartTime() < departTime &&
          temp->calculateDepartTime() != -1 &&
          temp->calculateDepartTime() <= expTimeElapsed) {
        min = temp;
        departTime = temp->calculateDepartTime();
      }
      temp = temp->get_next();
    }
    return min;
  }
}

void RegisterList::print() {
  Register* temp = head;
  while (temp != nullptr) {
    temp->print();
    temp = temp->get_next();
  }
}
