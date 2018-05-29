// Copyright (C) 2018  Marvin Stenger (Reactive Systems Group @ Saarland University)
//
// This file is part of RVHyper, a runtime verification tool for
// temporal hyperproperties.
//
// RVHyper is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// RVHyper is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
// License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef SRC_ROW_H_
#define SRC_ROW_H_

#include "Event.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Inspired by:
// http://stackoverflow.com/questions/1120140/how-can-i-read-and-parse-csv-files-in-c
class Row {
  public:
    void
    readLine(std::string &line) {
        std::istringstream lineStream(line);
        std::string input;
        // std::string output;

        std::getline(lineStream, input, ';');
        // std::getline(lineStream, output, ';');

        std::istringstream inputStream(input);
        // std::istringstream outputStream(output);
        std::string cell;

        event.input.clear();
        while (std::getline(inputStream, cell, ',')) {
            event.input.push_back(cell);
        }
        std::sort(event.input.begin(), event.input.end());
        event.output.clear();
        // while (std::getline(outputStream, cell, ',')) {
        while (std::getline(lineStream, cell, ',')) {
            event.output.push_back(cell);
        }
        std::sort(event.output.begin(), event.output.end());
    }
    Event event;
};

#endif /* SRC_ROW_H_ */
