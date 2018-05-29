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

#include "RememberingSelectionHelper.h"

#include <iostream>
#include <math.h>

Remembering_Selection_Helper::Remembering_Selection_Helper(unsigned size,
                                                           bool r, bool s)
    : Selection_Helper(1, size, r, s) {}

Remembering_Selection_Helper::~Remembering_Selection_Helper() {}

bool
Remembering_Selection_Helper::hasNext() {
    if (!Selection_Helper::hasNext())
        return false;
    for (size_t i = 0; i < selection.size(); ++i)
        if (selection[i] == n_alternatives - 1)
            return true;
    selection[selection.size() - 1] = n_alternatives - 1;
    return true;
}

void
Remembering_Selection_Helper::initNextIteration(unsigned n_alternatives) {
    this->n_alternatives = n_alternatives;
    for (size_t i = 0; i < selection.size() - 1; ++i)
        selection[i] = 0;
    selection[selection.size() - 1] = n_alternatives - 1;
    initialized = (n_alternatives > 1) || !r;
}
