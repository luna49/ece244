//
//  parser.cpp
//  lab3
//
//  Modified by Tarek Abdelrahman on 2020-10-04.
//  Created by Tarek Abdelrahman on 2018-08-25.
//  Copyright © 2018-2020 Tarek Abdelrahman.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

#include "Shape.h"
#include "globals.h"

// This is the shape array, to be dynamically allocated
Shape** shapesArray;

// The number of shapes in the database, to be incremented
int shapeCount = 0;

// The value of the argument to the maxShapes command
int max_shapes;

// ECE244 Student: you may want to add the prototype of
// helper functions you write here
void errorMessage(int type, string name);
bool nameError(string name, bool fail, bool eof, bool existError);
bool typeError(string type, bool fail, bool eof);
bool valueError(int loc, bool fail, bool eof);
bool angleError(int angle, bool fail, bool eof);

int main() {
  string line;
  string command;

  cout << "> ";        // Prompt for input
  getline(cin, line);  // Get a line from standard input

  while (!cin.eof()) {
    // Put the line in a linestream for parsing
    // Making a new sstream for each line so flags etc. are cleared
    stringstream lineStream(line);

    // Read from string stream into the command
    // The only way this can fail is if the eof is encountered
    lineStream >> command;

    // Check for the command and act accordingly
    // ECE244 Student: Insert your code here
    if (lineStream.fail()) {
      errorMessage(1, "");
    }
    // maxShapes: value
    else if (command == keyWordsList[1]) {
      int value;  // value should be >= 0
      lineStream >> value;
      // checking again for int read
      if (lineStream.fail()) {
        if (lineStream.eof()) {
          errorMessage(9, "");
        } else {
          errorMessage(2, "");
        }
      } else if (!lineStream.eof()) {
        if (lineStream.peek() != ' ') {
          errorMessage(2, "");
        } else {
          do {
            lineStream.ignore(1);
          } while (lineStream.peek() == ' ');
          if (!lineStream.eof()) {
            errorMessage(8, "");
          } else {
            if (value <= 0) {
              errorMessage(7, "");
            } else {
              // create array (no objects yet)
              max_shapes = value;
              shapesArray = new Shape*[value];
              for (int i = 0; i < value; i++) {
                shapesArray[i] = NULL;
              }
              cout << "New database: max shapes is " << value << endl;
            }
          }
        }
      } else if (value <= 0) {
        errorMessage(7, "");
      } else {
        // create array (no objects yet)
        max_shapes = value;
        shapesArray = new Shape*[value];
        for (int i = 0; i < value; i++) {
          shapesArray[i] = NULL;
        }
        cout << "New database: max shapes is " << value << endl;
      }
    }
    // create: name type loc loc size size
    else if (command == keyWordsList[2]) {
      string name, type;
      int loc_x, loc_y, size_x, size_y;
      if (shapeCount == max_shapes) {
        errorMessage(10, "");
      } else {
        lineStream >> name;
        if (!nameError(name, lineStream.fail(), lineStream.eof(), true)) {
          lineStream >> type;
          if (!typeError(type, lineStream.fail(), lineStream.eof())) {
            lineStream >> loc_x;
            if (!valueError(loc_x, lineStream.fail(), lineStream.eof())) {
              lineStream >> loc_y;
              if (!valueError(loc_y, lineStream.fail(), lineStream.eof())) {
                lineStream >> size_x;
                if (!valueError(size_x, lineStream.fail(), lineStream.eof())) {
                  lineStream >> size_y;
                  if (!valueError(size_y, lineStream.fail(),
                                  lineStream.eof())) {
                    do {
                      lineStream.ignore(1);
                    } while (lineStream.peek() == ' ');
                    if (!lineStream.eof()) {
                      errorMessage(8, "");
                    } else if (type == "circle" && (size_x != size_y)) {
                      errorMessage(7, "");
                    } else {
                      shapesArray[shapeCount] =
                          new Shape(name, type, loc_x, size_x, loc_y, size_y);
                      cout << "Created ";
                      shapesArray[shapeCount]->draw();
                      shapeCount++;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    // move: name loc1 loc2
    else if (command == keyWordsList[3]) {
      string name;
      int loc_x, loc_y;
      lineStream >> name;

      if (!nameError(name, lineStream.fail(), lineStream.eof(), false)) {
        lineStream >> loc_x;
        if (!valueError(loc_x, lineStream.fail(), lineStream.eof())) {
          lineStream >> loc_y;
          if (!valueError(loc_y, lineStream.fail(), lineStream.eof())) {
            do {
              lineStream.ignore(1);
            } while (lineStream.peek() == ' ');
            if (!lineStream.eof()) {
              errorMessage(8, "");
            } else {
              int location = 0;

              for (int i = 0; i < shapeCount; i++) {
                if (name == shapesArray[i]->getName()) {
                  location = i;
                  break;
                }
              }

              shapesArray[location]->setXlocation(loc_x);
              shapesArray[location]->setYlocation(loc_y);
              cout << "Moved " << name << " to " << loc_x << " " << loc_y
                   << endl;
            }
          }
        }
      }
    }
    // rotate: name angle
    else if (command == keyWordsList[4]) {
      string name;
      int angle;
      lineStream >> name;

      if (!nameError(name, lineStream.fail(), lineStream.eof(), false)) {
        lineStream >> angle;
        if (!angleError(angle, lineStream.fail(), lineStream.eof())) {
          do {
            lineStream.ignore(1);
          } while (lineStream.peek() == ' ');
          if (!lineStream.eof()) {
            errorMessage(8, "");
          } else {
            int location = 0;

            for (int i = 0; i < shapeCount; i++) {
              if (name == shapesArray[i]->getName()) {
                location = i;
                break;
              }
            }

            shapesArray[location]->setRotate(angle);
            cout << "Rotated " << name << " by " << angle << " degrees" << endl;
          }
        }
      }
    }
    // draw: name: type loc loc size size / all
    else if (command == keyWordsList[5]) {
      string name;
      lineStream >> name;

      if (name == "all") {
        do {
          lineStream.ignore(1);
        } while (lineStream.peek() == ' ');
        if (!lineStream.eof()) {
          errorMessage(8, "");
        } else {
          cout << "Drew all shapes" << endl;
          for (int i = 0; i < shapeCount; i++) {
            if (shapesArray[i] != NULL) {
              shapesArray[i]->draw();
            }
          }
        }
      }

      else if (!nameError(name, lineStream.fail(), lineStream.eof(), false)) {
        do {
          lineStream.ignore(1);
        } while (lineStream.peek() == ' ');
        if (!lineStream.eof()) {
          errorMessage(8, "");
        } else {
          cout << "Drew ";
          int location = 0;

          for (int i = 0; i < shapeCount; i++) {
            if (name == shapesArray[i]->getName()) {
              location = i;
              break;
            }
          }

          shapesArray[location]->draw();
        }
      }
    }
    // delete: name/all
    else if (command == keyWordsList[6]) {
      string name;
      lineStream >> name;

      if (name == "all") {
        do {
          lineStream.ignore(1);
        } while (lineStream.peek() == ' ');
        if (!lineStream.eof()) {
          errorMessage(8, "");
        } else {
          if (shapeCount > 0) {
            for (int i = 0; i < shapeCount; i++) {
              delete shapesArray[i];
              shapesArray[i] = NULL;
            }
            shapeCount = 0;
          }
          cout << "Deleted: all shapes" << endl;
        }

      } else if (!nameError(name, lineStream.fail(), lineStream.eof(), false)) {
        do {
          lineStream.ignore(1);
        } while (lineStream.peek() == ' ');
        if (!lineStream.eof()) {
          errorMessage(8, "");
        } else {
          if (shapeCount > 0) {
            int location = 0;

            for (int i = 0; i < shapeCount; i++) {
              if (name == shapesArray[i]->getName()) {
                location = i;
                break;
              }
            }

            delete shapesArray[location];
            shapesArray[location] = NULL;

            location++;

            while (location < shapeCount) {
              shapesArray[location - 1] = shapesArray[location];
              shapesArray[location] = NULL;
            }
          }
          cout << "Deleted shape " << name << endl;
        }
      }
    } else {
      errorMessage(1, "");
    }

    // Once the command has been processed, prompt for the
    // next command
    cout << "> ";  // Prompt for input
    getline(cin, line);

  }  // End input loop until EOF.

  return 0;
}

void errorMessage(int type, string name = "") {
  switch (type) {
    case 1:
      // first word entered does not match one of the valid commands
      cout << "Error: invalid command" << endl;
      break;
    case 2:
      // argument is not of correct type (ex. floating point num instead of
      // int)
      cout << "Error: invalid argument" << endl;
      break;
    case 3:
      // name used for a shape is a reserved word (ex. command name)
      cout << "Error: invalid shape name" << endl;
      break;
    case 4:
      // a shape with the name name exists in the database (ie. once created
      // and not deleted)
      cout << "Error: shape " << name << " exists" << endl;
      break;
    case 5:
      // shape with the name name specified in a command does not exist
      cout << "Error: shape " << name << " not found" << endl;
      break;
    case 6:
      // type used for a shape is not one of the allowed types
      cout << "Error: invalid shape type" << endl;
      break;
    case 7:
      // value specified in a command is invalid (ex. angle not between 0 and
      // 360)
      cout << "Error: invalid value" << endl;
      break;
    case 8:
      // more arguments were given than expected
      cout << "Error: too many arguments" << endl;
      break;
    case 9:
      // fewer arguments were given than expected
      cout << "Error: too few arguments" << endl;
      break;
    case 10:
      // attempt to create more shapes than argument given to the maxShapes
      // command
      cout << "Error: shape array is full" << endl;
      break;
  }
}

bool nameError(string name, bool fail, bool eof, bool existError) {
  if (fail) {
    if (eof) {
      errorMessage(9, "");
      return true;
    } else {
      errorMessage(2, "");
      return true;
    }
  }

  // check if name is any of the keywords in the global lists
  bool keywordCheck = false;

  for (int i = 0; i <= 6; i++) {
    if (name == keyWordsList[i]) {
      keywordCheck = true;
    }
  }

  for (int i = 0; i <= 3; i++) {
    if (name == shapeTypesList[i]) {
      keywordCheck = true;
    }
  }

  if (keywordCheck == true) {
    errorMessage(3, "");
    return true;
  }

  // check if name is already in the array
  bool existCheck = false;
  for (int i = 0; i < shapeCount; i++) {
    if (name == shapesArray[i]->getName()) {
      existCheck = true;
    }
  }

  if (existCheck == true) {
    if (existError) {
      errorMessage(4, name);
      return true;
    } else {
      return false;
    }
  }
  if (!existError) {
    errorMessage(5, name);
    return true;
  }

  return false;
}

bool typeError(string type, bool fail, bool eof) {
  if (fail) {
    if (eof) {
      errorMessage(9, "");
      return true;
    } else {
      errorMessage(2, "");
      return true;
    }
  }

  // check to make sure type is valid type
  bool keywordCheck = false;

  for (int i = 0; i <= 3; i++) {
    if (type == shapeTypesList[i]) {
      keywordCheck = true;
    }
  }

  if (keywordCheck == false) {
    errorMessage(6, "");
    return true;
  }

  return false;
}

bool valueError(int val, bool fail, bool eof) {
  if (fail) {
    if (eof) {
      errorMessage(9, "");
      return true;
    } else {
      errorMessage(2, "");
      return true;
    }
  }

  // check if loc/size/value is >=0
  if (val >= 0) {
    return false;
  } else {
    errorMessage(7, "");
    return true;
  }
}

bool angleError(int angle, bool fail, bool eof) {
  if (fail) {
    if (eof) {
      errorMessage(9, "");
      return true;
    } else {
      errorMessage(2, "");
      return true;
    }
  }

  // check if angle is in 0 to 360
  if (angle >= 0 && angle <= 360) {
    return false;
  } else {
    errorMessage(7, "");
    return true;
  }
}
