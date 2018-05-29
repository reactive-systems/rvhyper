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

#ifndef SRC_MONITORING_H_
#define SRC_MONITORING_H_

#include "Formula.h"
#include "MonitorAutomatonTemplate.h"
#include "SelectionHelper.h"
#include "Trie.h"
#include <vector>

class Monitoring {
  private:
    void printCEX(MonitorAutomaton *ma);

  protected:
    Formula &formula;
    MonitorAutomatonTemplate mat;
    Trie *eventTrie;
    unsigned nInstances;
    long nTransitions;

    void instantiateAutomata(std::list<MonitorAutomaton *> &mas);
    void forkAutomata(std::list<MonitorAutomaton *> &mas,
                      bool enforce_eventTrie);
    bool monitorstep(std::list<MonitorAutomaton *> &mas);
    spot::formula event2formula(Event &event);

  public:
    Monitoring(Formula &f);
    virtual ~Monitoring();
    void printAutomatonCharacteristics(std::ostream &out);
    void printStats();
};

#endif /* SRC_MONITORING_H_ */
