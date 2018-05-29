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

#ifndef SRC_VECTORTRACE_H_
#define SRC_VECTORTRACE_H_

#include "Trace.h"

class VectorTrace : public Trace {

  protected:
    std::vector<Event> events;
    unsigned current_position;

  public:
    VectorTrace(unsigned id);
    VectorTrace(unsigned id, std::vector<Event> &events);
    virtual ~VectorTrace();

    bool next();
    Event &currentEvent();
    void reset();
};

#endif /* SRC_VECTORTRACE_H_ */
