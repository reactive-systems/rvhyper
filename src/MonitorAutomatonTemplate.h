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

#ifndef MONITORAUTOMATONTEMPLATE_H_
#define MONITORAUTOMATONTEMPLATE_H_

#include "Formula.h"
#include "MonitorAutomaton.h"
#include "Trace.h"
#include <spot/twa/bdddict.hh>
#include <spot/twa/twagraph.hh>
#include <string>
#include <vector>

class MonitorAutomatonTemplate {
  public:
    std::vector<std::string> trace_vars;
    std::vector<std::string> aps;
    std::set<std::string> aps_set;
    std::vector<std::string> ltlaps;
    std::set<std::string> ltlaps_set;
    spot::const_twa_graph_ptr aut;

    MonitorAutomatonTemplate(Formula &f);
    ~MonitorAutomatonTemplate();

    void print(std::ostream &out);
    MonitorAutomaton *instanciate(std::vector<Trie *> &tries);
    bool representsFalse();
};

#endif /* MONITORAUTOMATONTEMPLATE_H_ */
