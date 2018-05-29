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

#include "timer.h"

timer::timer() {
    startedAt = 0;
    pausedAt = 0;
    paused = false;
    started = false;
}

bool
timer::IsStarted() {
    return started;
}

bool
timer::IsStopped() {
    return !started;
}

bool
timer::IsPaused() {
    return paused;
}

bool
timer::IsActive() {
    return !paused & started;
}

void
timer::Pause() {
    if (paused || !started)
        return;

    paused = true;
    pausedAt = clock();
}

void
timer::Resume() {
    if (!paused)
        return;

    paused = false;
    startedAt += clock() - pausedAt;
}

void
timer::Stop() {
    started = false;
}

void
timer::Start() {
    if (started)
        return;

    started = true;
    paused = false;
    startedAt = clock();
}

void
timer::Reset() {
    paused = false;
    startedAt = clock();
}

clock_t
timer::GetTicks() {
    if (!started)
        return 0;

    if (paused)
        return pausedAt - startedAt;

    return clock() - startedAt;
}
