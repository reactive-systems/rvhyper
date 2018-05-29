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

#ifndef MONITORAUTOMATON_H_
#define MONITORAUTOMATON_H_

#include "Trace.h"
#include "Trie.h"
#include <bddx.h>
#include <spot/tl/formula.hh>
#include <spot/twa/bdddict.hh>
#include <spot/twa/twagraph.hh>
#include <string>
#include <vector>

class MonitorAutomaton {
  public:
    std::vector<Trie *> tries;
    unsigned state;

    std::vector<unsigned> getConfig();

    MonitorAutomaton(std::vector<Trie *> &tries, unsigned state);
    ~MonitorAutomaton();

    std::string name();
    void step(spot::const_twa_graph_ptr &aut,
              std::vector<std::string> &trace_vars,
              std::set<std::string> &aps_set, long &nTransitions);
    bool accepting(spot::const_twa_graph_ptr &aut);
    bool rejecting(spot::const_twa_graph_ptr &aut);
    MonitorAutomaton *fork(std::vector<Trie *> &tries);
    void advanceTries();

    std::vector<Trace *> getTraces();

    unsigned getSteps();
};

#endif /* MONITORAUTOMATON_H_ */
