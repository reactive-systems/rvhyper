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

#ifndef TOKEN_H_
#define TOKEN_H_

#include <cstdlib>
#include <string>

#include "TokenEnums.h"
#include "pos.h"

struct Token {

  public:
    Pos pos;
    TokenKind kind;
    std::string text;

    Token();
    Token(Pos pos, TokenKind kind);
    Token(Pos pos, TokenKind kind, std::string text);

    void print();

    std::string toString();
};

#endif /* TOKEN_H_ */
