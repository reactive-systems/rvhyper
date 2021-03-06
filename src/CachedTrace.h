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

#ifndef SRC_CACHEDTRACE_H_
#define SRC_CACHEDTRACE_H_

#include "VectorTrace.h"

class CachedTrace : public VectorTrace {
  private:
    Trace *t;

  public:
    CachedTrace(Trace *t);
    virtual ~CachedTrace();

    bool next();
    std::string name();
};

#endif /* SRC_CACHEDTRACE_H_ */
