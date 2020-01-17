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

#include "MonitorAutomaton.h"

#include "VectorTrace.h"
#include "flags.h"
#include <iostream>
#include <spot/tl/print.hh>
#include <spot/twa/acc.hh>
#include <spot/twa/bddprint.hh>
#include <spot/twa/formula2bdd.hh>
#include <sstream>
#include <stdio.h>
#include <string>

MonitorAutomaton::MonitorAutomaton(std::vector<Trie *> &tries, unsigned state)
    : tries(tries), state(state) {}

MonitorAutomaton::~MonitorAutomaton() {}

std::vector<unsigned>
MonitorAutomaton::getConfig() {
    std::vector<unsigned> config;
    for (auto t : tries)
        config.push_back(t->sessionID);
    return config;
}

std::string
MonitorAutomaton::name() {
    auto cfg = getConfig();
    std::stringstream ss;
    ss << "Monitor(";
    for (size_t i = 0; i < cfg.size(); ++i) {
        if (i != 0)
            ss << ',';
        ss << cfg[i];
    }
    ss << ")";
    return ss.str();
}

void
MonitorAutomaton::step(spot::const_twa_graph_ptr &aut,
                       std::vector<std::string> &trace_vars,
                       std::set<std::string> &aps_set, long &nTransitions) {
    bool equal = tries.size() == 2 && tries[0] == tries[1];
    if (equal && tries[0]->state != -1) {
        state = tries[0]->state;
        return;
    }
    nTransitions++;

    std::set<std::string> aps_graph = aps_set; // copy
    std::vector<spot::formula> conjunct;
    for (size_t i = 0; i < tries.size(); ++i) {
        for (std::string &ap : tries[i]->value.input) {
            std::string tmp = ap + "_" + trace_vars[i];
            if (aps_graph.erase(tmp) > 0)
                conjunct.push_back(spot::formula::ap(tmp));
        }
        for (std::string &ap : tries[i]->value.output) {
            std::string tmp = ap + "_" + trace_vars[i];
            if (aps_graph.erase(tmp) > 0)
                conjunct.push_back(spot::formula::ap(tmp));
        }
    }
    for (std::string ap : aps_graph) {
        conjunct.push_back(spot::formula::Not(spot::formula::ap(ap)));
    }
    spot::formula curLabel = spot::formula::And(conjunct);
    if (FLAG_VERBOSE) {
        std::cout << name() << ": combined label:\n\t" << curLabel << std::endl;
    }
    bdd curLabelBDD = spot::formula_to_bdd(curLabel, aut->get_dict(), nullptr);

    for (auto &edge : aut->out(state)) {
        if (bdd_implies(curLabelBDD, edge.cond)) {
            if (FLAG_VERBOSE) {
                std::cout << name() << ": transition:\n\t" << state << " --> "
                          << edge.dst << std::endl;
            }
            state = edge.dst;
            if (equal) {
                tries[0]->state = state;
            }
            return;
        }
    }
    throw std::runtime_error("unmatched label");
}

bool
MonitorAutomaton::accepting(spot::const_twa_graph_ptr &aut) {
    for (auto &edge : aut->out(state)) {
        if (edge.dst == state && bdd_implies(bddtrue, edge.cond)) {
            // self-loop with True as guard
            return !rejecting(aut);
        }
    }
    return false;
}

bool
MonitorAutomaton::rejecting(spot::const_twa_graph_ptr &aut) {
    return !aut->state_is_accepting(state);
}

std::vector<Trace *>
MonitorAutomaton::getTraces() {
    auto traces = std::vector<Trace *>();
    for (auto t : tries) {
        auto path = t->getPath();
        traces.push_back(new VectorTrace(t->sessionID, path));
    }
    return traces;
}

void
MonitorAutomaton::advanceTries() {
    for (size_t i = 0; i < tries.size(); i++) {
        assert(tries[i]->children.size() == 1);
        tries[i] = tries[i]->children.front();
    }
}

unsigned
MonitorAutomaton::getSteps() {
    assert(!tries.empty());
    return tries.front()->getPath().size();
}

MonitorAutomaton *
MonitorAutomaton::fork(std::vector<Trie *> &tries) {
    return new MonitorAutomaton(tries, state);
}
