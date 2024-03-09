//
//  playMove.cpp
//  Connect Four Game
//
//  Created by Tarek Abdelrahman on 2019-06-07.
//  Modified by Tarek Abdelrahman on 2020-09-17.
//  Modified by Salma Emara and Ethan Hugh on 2023-09-06
//  Copyright © 2019-2020 Tarek Abdelrahman. All rights reserved.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.
//
//  ECE244 Student: write your code for playMove in this file

#include <iostream>

#include "GameState.h"
#include "globals.h"
using namespace std;
// Function prototypes (if you need)
bool checkFour(GameState, int, int, int, int, int);
bool checkWin(GameState, int);

void playMove(GameState& game_state) {
  // Change turn if move is valid,
  if (game_state.get_moveValid()) {  // play move if valid
    // start from bottom to search for first empty spot
    for (int i = boardSize - 1; i >= 0; i--) {
      if (game_state.get_gameBoard(i, game_state.get_selectedColumn()) ==
          Empty) {
        game_state.set_selectedRow(i);
        game_state.set_gameBoard(game_state.get_selectedRow(),
                                 game_state.get_selectedColumn(),
                                 game_state.get_turn() ? R : Y);

        break;
      }
      if (i == 0) {
        game_state.set_moveValid(false);
        return;
      }
    }
  }
  // Change gameOver
  game_state.set_gameOver(true);
  for (int i = 0; i < boardSize; i++) {
    for (int j = 0; j < boardSize; j++) {
      if (game_state.get_gameBoard(i, j) != Empty) {
        game_state.set_gameOver(false);
        break;
      }
    }
  }
  // Change winner
  if (checkWin(game_state, R)) {
    game_state.set_winner(R);
    game_state.set_gameOver(true);
  } else if (checkWin(game_state, Y)) {
    game_state.set_winner(Y);
    game_state.set_gameOver(true);
  }
  // set turn from true (R) <-> false (Y)
  if (game_state.get_turn()) {
    game_state.set_turn(false);
  } else {
    game_state.set_turn(true);
  }
}

bool checkFour(GameState game_state, int x, int y, int dirx, int diry,
               int color) {
  for (int i = 0; i < 4; i++) {
    if (x < 0 || x >= boardSize || y < 0 || y >= boardSize ||
        game_state.get_gameBoard(x, y) != color) {
      return false;
    }
    x += dirx;
    y += diry;
  }
  return true;
}

bool checkWin(GameState game_state, int player) {
  for (int i = 0; i < boardSize; i++) {
    for (int j = 0; j < boardSize; j++) {
      if (checkFour(game_state, i, j, 1, 0, player) ||  // Row
          checkFour(game_state, i, j, 0, 1, player) ||  // Column
          checkFour(game_state, i, j, 1, 1,
                    player) ||  // Diagonal left-to-right
          checkFour(game_state, i, j, 1, -1,
                    player)) {  // Diagonal right-to-left
        return true;
      }
    }
  }
  return false;
}