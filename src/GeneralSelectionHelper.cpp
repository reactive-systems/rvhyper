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

#include "GeneralSelectionHelper.h"

#include <iostream>

General_Selection_Helper::General_Selection_Helper(
    std::vector<unsigned> n_alternatives)
    : n_alternatives(n_alternatives), initialized(true) {
    for (auto n : n_alternatives) {
        if (n < 1) {
            throw std::runtime_error(
                "General_Selection_Helper input out of bounds");
        }
    }
    selection.resize(n_alternatives.size());
}

General_Selection_Helper::~General_Selection_Helper() {}

bool
General_Selection_Helper::hasNext() {
    if (initialized) {
        initialized = false;
        return true;
    }
    size_t i = selection.size() - 1;
    while (selection[i] == n_alternatives[i] - 1) {
        selection[i] = 0;
        if (i-- == 0)
            return false;
    }
    selection[i]++;
    return true;
}
