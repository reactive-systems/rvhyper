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

#ifndef SRC_NOLIVEMONITORING_H_
#define SRC_NOLIVEMONITORING_H_

#include "Formula.h"
#include "Monitoring.h"
#include "SelectionHelper.h"
#include <vector>

class NoLiveMonitoring : public Monitoring {
  private:
    void buildTrie(std::vector<Trace *> &traces);
    bool buildTrieLVL(Trie *&trie, Trace *trace);
    bool buildTrieLVL(std::vector<Trie *> &tries, std::vector<Trace *> &traces);

  public:
    NoLiveMonitoring(Formula &f);
    ~NoLiveMonitoring();
    bool monitor(std::vector<Trace *> &traces);
    bool monitor(Trace *trace);
};

#endif /* SRC_NOLIVEMONITORING_H_ */
