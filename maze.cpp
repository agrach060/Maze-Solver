//
// Created by Yusuf Pisan on 4/18/18.
//

#include "maze.h"
#include <algorithm>
#include <cassert>
#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

// print maze object
ostream &operator<<(ostream &out, const Maze &maze) {
  for (int row = 0; row < maze.height; ++row) {
    for (int col = 0; col < maze.width; ++col) {
      out << maze.field[row][col];
    }
    out << endl;
  }
  out << endl;
  return out;
}

// default constructor
Maze::Maze() = default;

// Load maze file from current directory
bool Maze::load(const string &fileName) {
  ifstream inFile;
  inFile.open(fileName);
  if (!inFile.is_open()) {
    cerr << "Unable to open file: " << fileName << endl;
    // next line is just to satisfy clang-tidy
    // otherwise it complains saying function can be const
    width = height = 0;
    return false;
  }
  inFile >> width >> height;
  inFile >> exitRow >> exitColumn;
  inFile >> startRow >> startColumn;
  // ignore leftover charcaters up to newline
  inFile.ignore(INT_MAX, '\n');
  string line;
  for (int row = 0; row < height; ++row) {
    getline(inFile, line);
    field.push_back(line);
  }
  return true;
}

// path to exit
string Maze::getPath() const { return path; }

// true if row, column is the exit
bool Maze::atExit(int row, int column) const {
  return row == exitRow && column == exitColumn;
}

// true if maze can be solved
bool Maze::solve() {
  // clear the path and mark the starting location as visited
  path = "";
  markAsPath(startRow, startColumn);
  // try to find a path to the exit
  if (solveRecursive(startRow, startColumn)) {
    // if a path is found, mark the exit as part of the path and return true
    markAsPath(exitRow, exitColumn);
    path = getPathRecursive();
    return true;
  }
  // if no path is found, return false
  return false;
}

// true if row, column is inside the maze
bool Maze::isInside(int row, int col) const {
  return row >= 0 && col >= 0 && row < field.size() && col < field[row].size();
}

// true if row, column is clear to move
bool Maze::isClear(int row, int col) const {
  assert(row >= 0 && col >= 0 && row < field.size() && col < field[row].size());
  return field[row][col] == CLEAR;
}

// mark location as part of the path to exit
void Maze::markAsPath(int row, int col) {
  assert(row >= 0 && col >= 0 && row < field.size() && col < field[row].size());
  field[row][col] = PATH;
}

// mark location as visited, not part of the path to exit
void Maze::markAsVisited(int row, int col) {
  assert(row >= 0 && col >= 0 && row < field.size() && col < field[row].size());
  field[row][col] = VISITED;
}

// recursive function tring to move in each direction
bool Maze::solveRecursive(int row, int col) {
  // base case: check if we have reached the exit
  if (atExit(row, col)) {
    return true;
  }

  // recursive case: try to move in each direction
  if (isInside(row - 1, col) && isClear(row - 1, col)) {
    markAsVisited(row - 1, col);
    if (solveRecursive(row - 1, col)) {
      markAsPath(row - 1, col);
      path += "N";
      return true;
    }
  }
  if (isInside(row, col + 1) && isClear(row, col + 1)) {
    markAsVisited(row, col + 1);
    if (solveRecursive(row, col + 1)) {
      markAsPath(row, col + 1);
      path += "E";
      return true;
    }
  }
  if (isInside(row + 1, col) && isClear(row + 1, col)) {
    markAsVisited(row + 1, col);
    if (solveRecursive(row + 1, col)) {
      markAsPath(row + 1, col);
      path += "S";
      return true;
    }
  }
  if (isInside(row, col - 1) && isClear(row, col - 1)) {
    markAsVisited(row, col - 1);
    if (solveRecursive(row, col - 1)) {
      markAsPath(row, col - 1);
      path += "W";
      return true;
    }
  }

  // if no path is found in any direction, backtrack
  return false;
}

// reverse the path for output
string Maze::getPathRecursive() const {
  string reversedPath = path;
  reverse(reversedPath.begin(), reversedPath.end());
  return reversedPath;
}