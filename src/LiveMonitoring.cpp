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

#include "LiveMonitoring.h"
#include "CachedTrace.h"
#include "flags.h"
#include <sstream>

LiveMonitoring::LiveMonitoring(Formula &f)
    : Monitoring(f), needToInstantiate(false), session_c(0) {}

LiveMonitoring::~LiveMonitoring() {}

void
LiveMonitoring::initSession() {
    session_c++;
    if (FLAG_VERBOSE) {
        std::cout << "Session " << session_c << ":" << std::endl;
    }

    eventTrie = eventTrie->root();
    needToInstantiate = true;
}

bool
LiveMonitoring::monitorEvent(Event &event) {
    if (needToInstantiate) {
        instantiateAutomata(mas);
        needToInstantiate = false;
    }
    if (FLAG_VERBOSE) {
        std::cout << "Check position " << (eventTrie->depth() + 1)
                  << " (monitor instances left: " << mas.size() << ")"
                  << std::endl;
        std::cout << "Event: " << event << std::endl;
    }
    event.restrictProperties(mat.aps);
    eventTrie = eventTrie->addValue(session_c, event);
    assert(eventTrie);
    if (mas.empty())
        return true;

    forkAutomata(mas, true);

    return monitorstep(mas);
}

void
LiveMonitoring::terminateSession() {
    for (auto ma : mas)
        delete ma;
    mas.clear();
}

void
LiveMonitoring::printStats() {
    std::cout << "seen traces: " << session_c << std::endl;
    Monitoring::printStats();
}

void
LiveMonitoring::printAPs() {
    bool first = true;
    for (auto &ap : mat.aps_set) {
        if (first) {
            first = false;
            std::cout << ap;
        } else {
            std::cout << "," << ap;
        }
    }
    std::cout << std::endl;
}
