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

#include "FileTrace.h"

std::istream &
operator>>(std::istream &str, Row &data) {
    std::string line;
    std::getline(str, line);
    data.readLine(line);
    return str;
}

FileTrace::FileTrace(unsigned id, std::string &filename)
    : Trace(id), filename(filename) {
    in.open(filename, std::ifstream::in);
    if (!in.is_open()) {
        throw std::runtime_error("could not open trace file " + filename);
    }
}

FileTrace::~FileTrace() { in.close(); }

bool
FileTrace::next() {
    Trace::next();
    return (bool)(in >> row);
}

Event &
FileTrace::currentEvent() {
    return row.event;
}

void
FileTrace::reset() {
    Trace::reset();
    in.close();
    in.open(filename, std::ifstream::in);
}

std::string
FileTrace::name() {
    std::stringstream ss;
    ss << Trace::name() << " (" << filename << ")";
    return ss.str();
}
