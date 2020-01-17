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

#include "Monitoring.h"

#include "GeneralSelectionHelper.h"
#include "MonitorAutomaton.h"
#include "flags.h"

Monitoring::Monitoring(Formula &f)
    : formula(f), mat(f), eventTrie(new Trie(0)), nInstances(0),
      nTransitions(0) {
    if (mat.representsFalse()) {
        throw std::runtime_error("formula evaluates to FALSE");
    }
}

Monitoring::~Monitoring() { delete eventTrie; }

void
Monitoring::printAutomatonCharacteristics(std::ostream &out) {
    out << "Monitor automaton template characteristics:" << std::endl;
    mat.print(out);
    out << std::endl;
    out << std::endl;
}

spot::formula
Monitoring::event2formula(Event &event) {
    std::set<std::string> aps_copy = mat.aps_set; // copy
    std::vector<spot::formula> conjunct;
    for (std::string ap : event.input) {
        if (aps_copy.find(ap) != aps_copy.end()) {
            aps_copy.erase(ap);
            conjunct.push_back(spot::formula::ap(ap));
        }
    }
    for (std::string ap : event.output) {
        if (aps_copy.find(ap) != aps_copy.end()) {
            aps_copy.erase(ap);
            conjunct.push_back(spot::formula::ap(ap));
        }
    }
    for (std::string ap : aps_copy) {
        conjunct.push_back(spot::formula::Not(spot::formula::ap(ap)));
    }
    return spot::formula::And(conjunct);
}

void
Monitoring::instantiateAutomata(std::list<MonitorAutomaton *> &mas) {
    assert(mas.empty());
    eventTrie = eventTrie->root();
    std::vector<Trie *> tries = std::vector<Trie *>();
    tries.resize(formula.tracevarsSize(), eventTrie);
    MonitorAutomaton *ma;
    ma = mat.instanciate(tries);
    mas.push_back(ma);
}

void
Monitoring::forkAutomata(std::list<MonitorAutomaton *> &mas,
                         bool enforce_eventTrie) {
    auto new_mas = std::list<MonitorAutomaton *>();
    auto n_alternatives = std::vector<unsigned>();
    auto tmp_tries = std::vector<Trie *>();
    tmp_tries.resize(formula.tracevarsSize());
    General_Selection_Helper sh(n_alternatives);
    for (auto mas_it = mas.begin(); mas_it != mas.end();) {
        auto ma = *mas_it;
        n_alternatives.clear();
        auto &tries = ma->tries;
        bool equal = tries.size() == 2 && tries[0] == tries[1];
        bool one = true;
        for (auto t : tries) {
            if (t->isLeaf()) {
                // continue outer loop
                goto outer;
            }
            n_alternatives.push_back(t->children.size());
            if (t->children.size() != 1) {
                one = false;
            }
        }
        if (one) {
            if (equal && formula.reflexive()) {
                if (tries[0]->cTraces > 1) {
                    goto advance;
                }
                mas_it++;
                continue;
            }
        advance:
            ma->advanceTries();
            if (enforce_eventTrie) {
                for (auto t : tries) {
                    if (t == eventTrie) {
                        goto push;
                    }
                }
                // no new trie contained
                mas_it++;
                continue;
            }
        push:
            mas_it = mas.erase(mas_it);
            new_mas.push_back(ma);
            continue;
        }
        sh = General_Selection_Helper(n_alternatives);
        while (sh.hasNext()) {
            auto &s = sh.next();
            for (size_t i = 0; i < n_alternatives.size(); i++) {
                // TODO short circuit selection array
                tmp_tries[i] = tries[i]->children[s[i]];
            }
            if (enforce_eventTrie) {
                for (auto t : tmp_tries) {
                    if (t == eventTrie) {
                        goto symm;
                    }
                }
                // no new trie contained
                continue;
            }
        symm:
            if (equal && formula.symmetric())
                if (tmp_tries[0]->sessionID > tmp_tries[1]->sessionID) {
                    // redundant by symmetry
                    continue;
                }
            MonitorAutomaton *new_ma;
            new_ma = ma->fork(tmp_tries);
            new_mas.push_back(new_ma);
        }
    outer:
        ++mas_it;
    }
    for (auto ma : mas)
        delete ma;
    mas.clear();
    mas.swap(new_mas);

    nInstances = mas.size() > nInstances ? mas.size() : nInstances;
}

bool
Monitoring::monitorstep(std::list<MonitorAutomaton *> &mas) {
    for (auto mas_it = mas.begin(); mas_it != mas.end();) {
        MonitorAutomaton *ma = *mas_it;
        ma->step(mat.aut, mat.trace_vars, mat.ltlaps_set, nTransitions);
        if (ma->rejecting(mat.aut)) {
            printCEX(ma);
            delete ma;
            mas_it = mas.erase(mas_it);
            return false;
        }
        if (ma->accepting(mat.aut)) {
            if (FLAG_VERBOSE)
                std::cout << ma->name() << " accepting" << std::endl;
            delete ma;
            mas_it = mas.erase(mas_it);
            continue;
        }
        ++mas_it;
    }
    return true;
}

void
Monitoring::printCEX(MonitorAutomaton *ma) {
    std::cerr << " --> " << ma->name() << " detected a violation at position "
              << ma->getSteps() << std::endl;
    if (!FLAG_QUIET) {
        auto traces = ma->getTraces();
        std::cout << "Counter Example:" << std::endl;
        bool first = true;
        for (auto t : traces) {
            if (first) {
                first = false;
                std::cout << t->name();
            } else {
                std::cout << " | " << t->name();
            }
            t->reset();
        }
        std::cout << std::endl;
        bool cond = true;
        while (cond) {
            first = true;
            for (auto t : traces) {
                bool available = t->next();
                assert(first | available);
                if (!available) {
                    cond = false;
                    break;
                }
                if (first) {
                    first = false;
                    std::cout << t->currentEvent();
                } else {
                    std::cout << " | " << t->currentEvent();
                }
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}

void
Monitoring::printStats() {
    std::cout << "trie size: " << eventTrie->root()->size() << std::endl;
    std::cout << "created instances: " << nInstances << std::endl;
    std::cout << "done transitions: " << nTransitions << std::endl;
}
