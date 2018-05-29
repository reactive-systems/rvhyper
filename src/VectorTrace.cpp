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

#include "VectorTrace.h"

VectorTrace::VectorTrace(unsigned id)
    : Trace(id), events(), current_position(0) {
    // TODO Auto-generated constructor stub
}

VectorTrace::VectorTrace(unsigned id, std::vector<Event> &events)
    : Trace(id), events(events), current_position(0) {
    // TODO Auto-generated constructor stub
}

VectorTrace::~VectorTrace() {
    // TODO Auto-generated destructor stub
}

bool
VectorTrace::next() {
    Trace::next();
    if (current_position == events.size()) {
        return false;
    }
    current_position++;
    return true;
}

Event &
VectorTrace::currentEvent() {
    return events[current_position - 1];
}

void
VectorTrace::reset() {
    Trace::reset();
    current_position = 0;
}
