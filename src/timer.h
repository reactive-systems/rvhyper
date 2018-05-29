// Copyright (C) 2018  Marvin Stenger (Reactive Systems Group @ Saarland University)
// Copyright (C) 2013  daniweb.com
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

// https://www.daniweb.com/programming/software-development/code/428040/simple-timer-class-c

#ifndef TIMER_H
#define TIMER_H

#include <ctime>

class timer {
    clock_t startedAt;
    clock_t pausedAt;
    bool started;
    bool paused;

  public:
    timer();
    bool IsStarted();
    bool IsStopped();
    bool IsPaused();
    bool IsActive();

    void Pause();
    void Resume();
    void Stop();
    void Start();
    void Reset();

    clock_t GetTicks();
};
#endif
