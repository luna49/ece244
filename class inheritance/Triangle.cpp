//
//  Triangle.cpp
//  Lab5
//
//  Created by Tarek Abdelrahman on 2020-11-25.
//  Copyright © 2020 Tarek Abdelrahman.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.

//  ECE244 Student: Write the implementation of the class Rectangle here

#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

#include "Shape.h"
#include "Triangle.h"

// Build a Triangle object
Triangle::Triangle(string n, float X1, float Y1, float X2, float Y2, float X3,
                   float Y3)
    : Shape(n, ((X1 + X2 + X3) / 3.0), ((Y1 + Y2 + Y3) / 3.0)) {
  x1 = X1;
  y1 = Y1;
  x2 = X2;
  y2 = Y2;
  x3 = X3;
  y3 = Y3;
}

Triangle::~Triangle() {
  // Nothing to do
}

// Accessor
float Triangle::getX1() const { return x1; }
float Triangle::getY1() const { return y1; }
float Triangle::getX2() const { return x2; }
float Triangle::getY2() const { return y2; }
float Triangle::getX3() const { return x3; }
float Triangle::getY3() const { return y3; }

// Mutator
void Triangle::setX1(float X1) { x1 = X1; }
void Triangle::setY1(float Y1) { y1 = Y1; }
void Triangle::setX2(float X2) { x2 = X2; }
void Triangle::setY2(float Y2) { y2 = Y2; }
void Triangle::setX3(float X3) { x3 = X3; }
void Triangle::setY3(float Y3) { y3 = Y3; }

void Triangle::draw() const {
  // Set floating point printing to fixed point with 2 decimals
  cout << std::fixed;
  cout << std::setprecision(2);

  // Print the information
  cout << "triangle: " << name << " " << x1 << " " << y1 << " " << x2 << " "
       << y2 << " " << x3 << " " << y3 << " " << computeArea() << endl;
}

float Triangle::computeArea() const {
  float area = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);
  if (area < 0) {
    return (area * -1.0) / 2.0;
  } else {
    return area / 2.0;
  }
}

Shape* Triangle::clone() const { return (new Triangle(*this)); }