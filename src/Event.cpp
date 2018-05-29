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

#include "Event.h"

#include <algorithm>

std::ostream &
operator<<(std::ostream &os, const Event &e) {
    bool first = true;
    for (auto i : e.input) {
        if (first) {
            os << i;
            first = false;
        } else {
            os << ',' << i;
        }
    }
    if (!e.input.empty() && !e.output.empty())
        os << ';';
    first = true;
    for (auto o : e.output) {
        if (first) {
            os << o;
            first = false;
        } else {
            os << ',' << o;
        }
    }
    return os;
}

void
Event::restrictProperties(std::vector<std::string> &aps) {
    std::vector<std::string> tmp;
    std::set_intersection(input.begin(), input.end(), aps.begin(), aps.end(),
                          std::back_inserter(tmp));
    input = tmp;
    tmp.clear();
    std::set_intersection(output.begin(), output.end(), aps.begin(), aps.end(),
                          std::back_inserter(tmp));
    output = tmp;
}
