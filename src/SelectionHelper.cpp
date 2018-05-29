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

#include "SelectionHelper.h"

#include <iostream>
#include <math.h>

Selection_Helper::Selection_Helper(unsigned n_alternatives, unsigned size,
                                   bool r, bool s)
    : n_alternatives(n_alternatives), initialized(!r), r(r), s(s) {
    if (size < 1) {
        std::cerr << "Selection_Helper input out of bounds, size=" << size
                  << std::endl;
        throw std::runtime_error("Selection_Helper input out of bounds");
    }
    selection.resize(size);
}

Selection_Helper::~Selection_Helper() {}

bool
Selection_Helper::hasNext() {
    if (n_alternatives == 0)
        return false;
    if (initialized) {
        initialized = false;
        return true;
    }
    size_t i = selection.size() - 1;
    while (selection[i] == n_alternatives - 1) {
        selection[i] = 0;
        if (i-- == 0)
            return false;
    }
    selection[i]++;
    if (s && i < selection.size() - 1) {
        for (size_t j = i + 1; j < selection.size(); ++j)
            selection[j] = selection[i];
    }
    if (r) {
        for (size_t j = 1; j < selection.size(); ++j) {
            if (selection[j] != selection[0])
                return true;
        }
        return selection[selection.size() - 1]++ != n_alternatives - 1;
    }
    return true;
}
