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

#ifndef SRC_LIVEMONITORING_H_
#define SRC_LIVEMONITORING_H_

#include <list>
#include <string>

#include "Event.h"
#include "LiveTrace.h"
#include "MonitorAutomaton.h"
#include "Monitoring.h"
#include "RememberingSelectionHelper.h"
#include "Trace.h"

class LiveMonitoring : public Monitoring {
  private:
    std::list<MonitorAutomaton *> mas;
    bool needToInstantiate;
    unsigned session_c;

  public:
    LiveMonitoring(Formula &f);
    ~LiveMonitoring();

    void initSession();
    bool monitorEvent(Event &event);
    void terminateSession();

    void printStats();
    void printAPs();
};

#endif /* SRC_LIVEMONITORING_H_ */
