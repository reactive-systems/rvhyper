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

#include "NoLiveMonitoring.h"

#include "MonitorAutomaton.h"
#include "flags.h"

NoLiveMonitoring::NoLiveMonitoring(Formula &f) : Monitoring(f) {}

NoLiveMonitoring::~NoLiveMonitoring() {}

bool
NoLiveMonitoring::monitor(std::vector<Trace *> &traces) {
    auto tries = std::vector<Trie *>(traces.size(), eventTrie->root());

    std::list<MonitorAutomaton *> mas;
    instantiateAutomata(mas);
    int position = 0;

    while (!mas.empty()) {
        if (FLAG_VERBOSE) {
            position++;
            std::cout << "check position " << position
                      << " (monitor instances left: " << mas.size() << ")"
                      << std::endl;
        }
        buildTrieLVL(tries, traces);
        forkAutomata(mas, false);
        if (!monitorstep(mas))
            return false;
    }
    return true;
}

bool
NoLiveMonitoring::monitor(Trace *trace) {
    eventTrie = eventTrie->root();

    std::list<MonitorAutomaton *> mas;
    instantiateAutomata(mas);
    buildTrieLVL(eventTrie, trace);
    forkAutomata(mas, true);
    int position = 0;

    while (!mas.empty()) {
        if (FLAG_VERBOSE) {
            position++;
            std::cout << "check position " << position
                      << " (monitor instances left: " << mas.size() << ")"
                      << std::endl;
        }
        if (!monitorstep(mas))
            return false;
        buildTrieLVL(eventTrie, trace);
        forkAutomata(mas, true);
    }
    bool added = false;
    do {
        added = buildTrieLVL(eventTrie, trace);
    } while (added);
    return true;
}

bool
NoLiveMonitoring::buildTrieLVL(Trie *&trie, Trace *trace) {
    if (!trace->next()) {
        return false;
    }
    auto &e = trace->currentEvent();
    std::cout << trace->name() << " Event: " << e << std::endl;
    e.restrictProperties(mat.aps);
    trie = trie->addValue(trace->getId(), e);
    return true;
}

bool
NoLiveMonitoring::buildTrieLVL(std::vector<Trie *> &tries,
                               std::vector<Trace *> &traces) {
    bool added = false;
    for (size_t i = 0; i < traces.size(); i++) {
        added = buildTrieLVL(tries[i], traces[i]);
    }
    return added;
}

void
NoLiveMonitoring::buildTrie(std::vector<Trace *> &traces) {
    eventTrie = eventTrie->root();
    auto root = eventTrie;
    for (size_t i = 0; i < traces.size(); i++) {
        Trace *t = traces[i];
        while (t->next()) {
            auto &e = t->currentEvent();
            e.restrictProperties(mat.aps);
            eventTrie = eventTrie->addValue(t->getId(), e);
        }
        eventTrie = root;
    }
}
