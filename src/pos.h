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

#ifndef POS_H
#define POS_H

struct Pos {
    explicit Pos() : Pos("", 0, 0) {}
    explicit Pos(char const *const name) : Pos(name, 0, 0) {}

    Pos(char const *const name, unsigned int const line,
        unsigned int const column)
        : name(name), line(line), column(column) {}

    char const *name;
    unsigned int line;
    unsigned int column;
};

#endif
