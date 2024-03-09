#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>

#include "Customer.h"
#include "QueueList.h"
#include "Register.h"
#include "RegisterList.h"

using namespace std;

// Function Declarations:

// Set mode of the simulation
string getMode();

// Register
void parseRegisterAction(stringstream &lineStream, string mode);
void openRegister(
    stringstream &lineStream,
    string mode);  // register opens (it is upto customers to join)
void closeRegister(stringstream &lineStream,
                   string mode);  // register closes

// Customer
void addCustomer(stringstream &lineStream,
                 string mode);  // customer wants to join

// Helper functions
bool getInt(stringstream &lineStream, int &iValue);
bool getDouble(stringstream &lineStream, double &dValue);
bool foundMoreArgs(stringstream &lineStream);
void updateSystem(double timeElapsed, string mode);
void finalStats();

// Global variables
RegisterList *registerList;  // holding the list of registers
QueueList *doneList;         // holding the list of customers served
QueueList *singleQueue;      // holding customers in a single virtual queue
double expTimeElapsed;  // time elapsed since the beginning of the simulation

// List of commands:
// To open a register
// register open <ID> <secPerItem> <setupTime> <timeElapsed>
// To close register
// register close <ID> <timeElapsed>
// To add a customer
// customer <items> <timeElapsed>

int main() {
  registerList = new RegisterList();
  doneList = new QueueList();
  singleQueue = new QueueList();
  expTimeElapsed = 0;

  // Set mode by the user
  string mode = getMode();

  string line;
  string command;

  cout << "> ";  // Prompt for input
  getline(cin, line);

  while (!cin.eof()) {
    stringstream lineStream(line);
    lineStream >> command;
    if (command == "register") {
      parseRegisterAction(lineStream, mode);
    } else if (command == "customer") {
      addCustomer(lineStream, mode);
    } else {
      cout << "Invalid operation" << endl;
    }
    cout << "> ";  // Prompt for input
    getline(cin, line);
  }

  // You have to make sure all dynamically allocated memory is freed
  // before return 0
  finalStats();
  return 0;
}

string getMode() {
  string mode;
  cout << "Welcome to ECE 244 Grocery Store Queue Simulation!" << endl;
  cout << "Enter \"single\" if you want to simulate a single queue or "
          "\"multiple\" to "
          "simulate multiple queues: \n> ";
  getline(cin, mode);

  if (mode == "single") {
    cout << "Simulating a single queue ..." << endl;
  } else if (mode == "multiple") {
    cout << "Simulating multiple queues ..." << endl;
  }

  return mode;
}

void addCustomer(stringstream &lineStream, string mode) {
  int items;
  double timeElapsed;
  if (!getInt(lineStream, items) || !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments." << endl;
    return;
  }
  // Depending on the mode of the simulation (single or multiple),
  // add the customer to the single queue or to the register with
  // fewest items
  Customer *newCustomer = new Customer(expTimeElapsed + timeElapsed, items);
  if (mode == "single") {
    updateSystem(timeElapsed, "single");
    // add the customer to the single queue
    singleQueue->enqueue(newCustomer);
    cout << "A customer entered" << endl;
    if (registerList->get_free_register() != nullptr) {
      Register *free = registerList->get_free_register();
      Customer *customer = singleQueue->dequeue();
      if (customer != nullptr) {
        free->get_queue_list()->enqueue(customer);
        customer->set_departureTime(free->calculateDepartTime());
        free->set_availableTime(customer->get_departureTime());
        cout << "Queued a customer with free register " << free->get_ID()
             << endl;
      }
    } else {
      cout << "No free registers" << endl;
    }
  } else if (mode == "multiple") {
    updateSystem(timeElapsed, "multiple");
    // add the customer to the register with fewest items
    Register *min = registerList->get_min_items_register();
    min->get_queue_list()->enqueue(newCustomer);
    cout << "A customer entered" << endl;
    // if the register has no customers, assign the customer to it
    if (min->get_queue_list()->get_head()->get_departureTime() == -1) {
      min->get_queue_list()->get_head()->set_departureTime(
          min->calculateDepartTime());
      min->set_availableTime(
          min->get_queue_list()->get_head()->get_departureTime());
      cout << "Queued a customer with quickest register " << min->get_ID()
           << endl;
    }
    // if the register has customers, add customer to the end of the shortest
    // queue
    else {
      Customer *temp = min->get_queue_list()->get_head();
      while (temp->get_next() != nullptr) {
        temp = temp->get_next();
      }
      // temp->set_departureTime(min->calculateDepartTime());
      // min->set_availableTime(temp->get_departureTime());
      cout << "Queued a customer with quickest register " << min->get_ID()
           << endl;
    }
  }
}

void parseRegisterAction(stringstream &lineStream, string mode) {
  string operation;
  lineStream >> operation;
  if (operation == "open") {
    openRegister(lineStream, mode);
  } else if (operation == "close") {
    closeRegister(lineStream, mode);
  } else {
    cout << "Invalid operation" << endl;
  }
}

void openRegister(stringstream &lineStream, string mode) {
  int ID;
  double secPerItem, setupTime, timeElapsed;
  // convert strings to int and double
  if (!getInt(lineStream, ID) || !getDouble(lineStream, secPerItem) ||
      !getDouble(lineStream, setupTime) ||
      !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments" << endl;
    return;
  }

  // Check if the register is already open
  // If it's open, print an error message
  // Otherwise, open the register
  // If we were simulating a single queue,
  // and there were customers in line, then
  // assign a customer to the new register
  if (registerList->foundRegister(ID)) {
    cout << "Error: register " << ID << " is already open" << endl;
  } else {
    if (mode == "single") {
      updateSystem(timeElapsed, "single");
      Register *newRegister =
          new Register(ID, secPerItem, setupTime, expTimeElapsed + timeElapsed);
      // if the register is already open, print an error message
      registerList->enqueue(newRegister);
      newRegister->set_availableTime(expTimeElapsed);
      cout << "Opened register " << ID << endl;
      // if there were customers in line, assign a customer to the new register
      if (singleQueue->get_head() != nullptr) {
        Customer *temp = singleQueue->dequeue();
        newRegister->get_queue_list()->enqueue(temp);
        temp->set_departureTime(newRegister->calculateDepartTime());
        newRegister->set_availableTime(temp->get_departureTime());
        cout << "Queued a customer with free register " << ID << endl;
      }
    } else if (mode == "multiple") {
      updateSystem(timeElapsed, "multiple");
      Register *newRegister =
          new Register(ID, secPerItem, setupTime, expTimeElapsed + timeElapsed);
      // if the register is already open, print an error message
      // open the register
      registerList->enqueue(newRegister);
      newRegister->set_availableTime(expTimeElapsed);
      cout << "Opened register " << ID << endl;
    }
  }
}

void closeRegister(stringstream &lineStream, string mode) {
  int ID;
  double timeElapsed;
  // convert string to int
  if (!getInt(lineStream, ID) || !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments" << endl;
    return;
  }
  // Check if the register is open
  // If it is open dequeue it and free it's memory
  // Otherwise, print an error message
  if (mode == "single") {
    updateSystem(timeElapsed, "single");
    // if the register is open, dequeue it and free it's memory
    if (registerList->foundRegister(ID)) {
      Register *temp = registerList->dequeue(ID);
      delete temp;
      cout << "Closed register " << ID << endl;
    } else {
      cout << "Error: register " << ID << " is not open" << endl;
    }
  } else if (mode == "multiple") {
    updateSystem(timeElapsed, "multiple");
    // if the register is open, dequeue it and free it's memory
    if (registerList->foundRegister(ID)) {
      Register *temp = registerList->dequeue(ID);
      delete temp;
      cout << "Closed register " << ID << endl;
    } else {
      cout << "Error: register " << ID << " is not open" << endl;
    }
  }
}

bool getInt(stringstream &lineStream, int &iValue) {
  // Reads an int from the command line
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  }
  iValue = stoi(command);
  return true;
}

bool getDouble(stringstream &lineStream, double &dvalue) {
  // Reads a double from the command line
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  }
  dvalue = stod(command);
  return true;
}

bool foundMoreArgs(stringstream &lineStream) {
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  } else {
    return true;
  }
}

void updateSystem(double timeElapsed, string mode) {
  // depart customers from earliest to latest
  // Iterate through registers and find earliest departure
  // current time
  expTimeElapsed += timeElapsed;
  // getting register with least departure time
  Register *min = registerList->calculateMinDepartTimeRegister(expTimeElapsed);
  // while there is customers in register and they leave before current time
  // cout << "TIME: " << expTimeElapsed << endl;
  // if (min != nullptr && min->calculateDepartTime() != -1) {
  //   cout << "DEPART: " << min->calculateDepartTime() << endl;
  //   cout << "MIN " << min->get_ID() << endl;
  // }
  while (min != nullptr && min->calculateDepartTime() != -1 &&
         min->calculateDepartTime() <= expTimeElapsed) {
    // get the customer depart time
    double time = min->calculateDepartTime();
    // depart the customer and add to doneList
    min->departCustomer(doneList);
    cout << "Departed a customer at register ID " << min->get_ID() << " at "
         << time << endl;
    // cout << "TIME: " << expTimeElapsed << endl;
    // check if the queue has a customer, if it does then add to register
    if (mode == "single") {
      if (singleQueue->get_head() != nullptr) {
        Customer *temp = singleQueue->dequeue();
        min->get_queue_list()->enqueue(temp);
        temp->set_departureTime(min->calculateDepartTime());
        min->set_availableTime(temp->get_departureTime());
        cout << "Queued a customer with free register " << min->get_ID()
             << endl;
        // cout << "DEPART TIME: " << min->calculateDepartTime() << endl;
      }
    } else if (mode == "multiple") {
    }
    // get the next register with least departure time
    min = registerList->calculateMinDepartTimeRegister(expTimeElapsed);
  }

  //   while (temp != nullptr) {
  //     if (temp->calculateDepartTime() != -1) {
  //       if (temp->calculateDepartTime() <= expTimeElapsed + timeElapsed) {
  //         double time = temp->calculateDepartTime();
  //         temp->departCustomer(doneList);
  //         cout << "Departed a customer at register ID " << temp->get_ID()
  //              << " at " << time << endl;
  //       }
  //     }
  //     temp = temp->get_next();
  //   }
  // expTimeElapsed += timeElapsed;
}

void finalStats() {
  // calculate and print finished time, max and avg wait time, and standard
  // dev of wait
  cout << "\nFinished at time " << expTimeElapsed << endl;
  cout << "Statistics: " << endl;
  Customer *temp = doneList->get_head();
  Customer *max = doneList->get_head();
  int count = 0;
  double sum = 0;
  double avg = 0;
  while (temp != nullptr) {
    // max
    if (temp->get_departureTime() - temp->get_arrivalTime() >
        max->get_departureTime() - max->get_arrivalTime()) {
      max = temp;
    }
    // avg
    sum += temp->get_departureTime() - temp->get_arrivalTime();
    count += 1;
    avg = sum / count;
    temp = temp->get_next();
  }
  // standard dev
  temp = doneList->get_head();
  double stdsum = 0;
  double final = 0;
  while (temp != nullptr) {
    stdsum += (temp->get_departureTime() - temp->get_arrivalTime() - avg) *
              (temp->get_departureTime() - temp->get_arrivalTime() - avg);
    final = sqrt(stdsum / count);
    temp = temp->get_next();
  }
  cout << "Maximum wait time: "
       << max->get_departureTime() - max->get_arrivalTime() << endl;
  cout << "Average wait time: " << avg << endl;
  cout << "Standard Deviation of wait time: " << final << endl;
}