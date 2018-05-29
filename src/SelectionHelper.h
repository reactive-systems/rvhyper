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

#ifndef SRC_SELECTIONHELPER_H_
#define SRC_SELECTIONHELPER_H_

#include <stdlib.h>
#include <vector>

class Selection_Helper {
  protected:
    std::vector<unsigned> selection;
    unsigned n_alternatives;
    bool initialized;
    bool r, s;

  public:
    Selection_Helper(unsigned n_alternatives, unsigned size, bool r, bool s);
    virtual ~Selection_Helper();

    virtual bool hasNext();
    std::vector<unsigned> &
    next() {
        return selection;
    };
    size_t
    width() {
        return selection.size();
    };
};

#endif /* SRC_SELECTIONHELPER_H_ */
