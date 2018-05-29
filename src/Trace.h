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

#ifndef TRACE_H_
#define TRACE_H_
#include "Event.h"
#include <string>
#include <vector>

class Trace {
  protected:
    unsigned id;
    unsigned idx;
    bool bDone;

  public:
    Trace(unsigned id);
    virtual ~Trace();

    virtual bool
    next() {
        idx++;
        return false;
    }
    void
    done() {
        bDone = true;
    }
    bool
    isDone() const {
        return bDone;
    }
    virtual Event &currentEvent() = 0;
    virtual void
    reset() {
        idx = 0;
        bDone = false;
    }
    virtual std::string name();

    unsigned
    getId() const {
        return id;
    }
    unsigned
    getIndex() const {
        return idx;
    }
};

#endif /* TRACE_H_ */
