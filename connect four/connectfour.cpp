//
//  connectfour.cpp
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
//  ECE244 Student: Complete the skeletal code of the main function in this file

#include <iostream>
#include <string>
using namespace std;

#include "GameState.h"
#include "globals.h"

// Function prototypes

// The main function
int main() {
  /**********************************************************************************/
  /* Create three initialized game state objects in an array */
  /**********************************************************************************/
  const int numOfRounds = 3;
  GameState game_state[numOfRounds];

  // Read one integer from the user that represents the column
  // the player would like to place their piece (R or Y) in
  // You can assume there will be no formatting errors in the input

  int col;
  int round = 0;

  const int RC = 1;
  const int YC = -1;
  int rWinCount = 0;
  int yWinCount = 0;

  cout << "Game 1" << endl;

  while (!game_state[round].get_gameOver()) {
    cout << "Enter column to place piece: ";
    cin >> col;

    if (cin.eof()) {
      cerr << endl << "Game ended by user." << endl;
      exit(0);
    }
    if (cin.fail()) {
      cin.clear();
      cin.ignore(1000, '\n');
      col = -1;  // giving col invalid value so it will be handled as invalid
                 // input below
    }

    // Check validity of input and if not valid, handle accordingly
    if (!(col >= 0 && col < boardSize) ||
        game_state[round].get_gameBoard(boardSize - 1, col) != Empty) {
      cout << "Invalid column!" << endl;
      game_state[round].set_moveValid(!game_state[round].get_turn());
      continue;
    }
    // The coordinates are valid; set the selectedRow and selectedColumn
    // members of the game state to the read values
    game_state[round].set_turn(true);
    cout << "column chosen: " << col << endl;
    game_state[round].set_selectedColumn(col);
    game_state[round].set_moveValid(true);
    // Note that the corresponding mutators of GameState must be first
    // implemented before this works

    // Call playMove
    GameState &gsRef = game_state[round];
    playMove(gsRef);
    // Print the GameState object, as prescribed in the handout
    for (int i = boardSize - 1; i >= 0; i--) {
      for (int j = 0; j < boardSize; j++) {
        if (game_state[round].get_gameBoard(i, j) == RC) {
          cout << "R";
          break;
        } else if (game_state[round].get_gameBoard(i, j) == YC) {
          cout << "Y";
          break;
        }
      }
      cout << endl;
    }
    // Check if a player won this round and if so handle accordingly
    if (game_state[round].get_winner() == R) {
      cout << "R won this round!" << endl;
      rWinCount++;
    } else if (game_state[round].get_winner() == Y) {
      cout << "Y won this round!" << endl;
      yWinCount++;
    }

    if (rWinCount == (numOfRounds / 2 + 1) ||
        yWinCount == (numOfRounds / 2 + 1)) {
      if (rWinCount == (numOfRounds / 2 + 1)) {
        cout << "R won the match!";
      } else if (yWinCount == (numOfRounds / 2 + 1)) {
        cout << "Y won the match!";
      }
      break;
    }

    if (game_state[round].get_gameOver()) {
      round++;
      game_state[round].set_turn(true);
      cout << "Game " << round + 1 << endl;
    }
  }
}
