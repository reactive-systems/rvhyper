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

#ifndef DIAGNOSTIC_H
#define DIAGNOSTIC_H

#include "pos.h"

#define PANIC(msg) panic(__FILE__, __LINE__, (msg))

void panic(char const *const file, int const line, char const *const msg);

void errorf(Pos const &, char const *fmt, ...);

void errorf(char const *fmt, ...);

void errorErrno(Pos const &pos);

bool hasNewErrors();

int printDiagnosticSummary();

#endif
