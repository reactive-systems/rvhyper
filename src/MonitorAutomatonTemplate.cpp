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

#include "MonitorAutomatonTemplate.h"

#include "flags.h"
#include <algorithm>
#include <bddx.h>
#include <iostream>
#include <spot/twa/bddprint.hh>
#include <spot/twa/fwd.hh>

MonitorAutomatonTemplate::MonitorAutomatonTemplate(Formula &f)
    : trace_vars(f.tracevars()), aut(f.translate2monitor()) {
    // initialize aps sets
    for (const spot::formula &ap : aut->ap()) {
        std::string ap_name = ap.ap_name();
        ltlaps.push_back(ap_name);
        ltlaps_set.insert(ap_name);
        size_t found = ap_name.find_last_of('_');
        std::string id = ap_name.substr(0, found);
        aps_set.insert(id);
    }
    for (auto &ap : aps_set) {
        aps.push_back(ap);
    }
    std::sort(ltlaps.begin(), ltlaps.end());
    std::sort(aps.begin(), aps.end());
};

MonitorAutomatonTemplate::~MonitorAutomatonTemplate() {
    aut->get_dict()->unregister_all_my_variables(nullptr);
}

void
custom_print(std::ostream &out, spot::const_twa_graph_ptr &aut) {
    // We need the dictionary to print the BDDs that label the edges
    const spot::bdd_dict_ptr &dict = aut->get_dict();

    // Some meta-data...
    out << "Acceptance: " << aut->get_acceptance() << std::endl;
    out << "Number of sets: " << aut->num_sets() << std::endl;
    out << "Number of states: " << aut->num_states() << std::endl;
    out << "Number of edges: " << aut->num_edges() << std::endl;
    out << "Initial state: " << aut->get_init_state_number() << std::endl;
    out << "Atomic propositions:";
    for (spot::formula ap : aut->ap())
        out << ' ' << ap << " (=" << dict->varnum(ap) << ')';
    out << std::endl;

    // Arbitrary data can be attached to automata, by giving them
    // a type and a name.  The HOA parser and printer both use the
    // "automaton-name" to name the automaton.
    if (auto name = aut->get_named_prop<std::string>("automaton-name"))
        out << "Name: " << *name << std::endl;

    // For the following prop_*() methods, the return value is an
    // instance of the spot::trival class that can represent
    // yes/maybe/no.  These properties correspond to bits stored in the
    // automaton, so they can be queried in constant time.  They are
    // only set whenever they can be determined at a cheap cost: for
    // instance an algorithm that always produces deterministic automata
    // would set the deterministic property on its output.  In this
    // example, the properties that are set come from the "properties:"
    // line of the input file.
    out << "Deterministic: " << aut->prop_deterministic() << std::endl;
    out << "Unambiguous: " << aut->prop_unambiguous() << std::endl;
    out << "State-Based Acc: " << aut->prop_state_acc() << std::endl;
    out << "Terminal: " << aut->prop_terminal() << std::endl;
    out << "Weak: " << aut->prop_weak() << std::endl;
    out << "Inherently Weak: " << aut->prop_inherently_weak() << std::endl;
    out << "Stutter Invariant: " << aut->prop_stutter_invariant() << std::endl;

    if (FLAG_VERBOSE) {
        // States are numbered from 0 to n-1
        unsigned n = aut->num_states();
        for (unsigned s = 0; s < n; ++s) {
            out << "State " << s << ":\n";

            // The out(s) method returns a fake container that can be
            // iterated over as if the contents was the edges going
            // out of s.  Each of these edges is a quadruplet
            // (src,dst,cond,acc).  Note that because this returns
            // a reference, the edge can also be modified.
            for (auto &t : aut->out(s)) {
                out << "  edge(" << t.src << " -> " << t.dst
                    << ")\n    label = ";
                spot::bdd_print_formula(out, dict, t.cond);
                out << "\n    acc sets = " << t.acc << std::endl;
            }
        }
    }
}

void
MonitorAutomatonTemplate::print(std::ostream &out) {
    out << "trace variables:" << std::endl;
    bool first = true;
    for (auto var : trace_vars) {
        if (first) {
            out << var;
            first = false;
        } else {
            out << ", " << var;
        }
    }
    out << std::endl;
    custom_print(out, aut);
}

MonitorAutomaton *
MonitorAutomatonTemplate::instanciate(std::vector<Trie *> &tries) {
    assert(trace_vars.size() == tries.size());
    return new MonitorAutomaton(tries, aut->get_init_state_number());
}

bool
MonitorAutomatonTemplate::representsFalse() {
    return !aut->state_is_accepting(aut->get_init_state_number());
}
