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

#ifndef LEXER_H_
#define LEXER_H_

#include "Token.h"
#include <stdint.h>
#include <string>

class Lexer {
  public:
    Lexer(const char *formula);
    Lexer(std::string filename);
    virtual ~Lexer();

    Token nextToken();

  private:
    std::string filename;
    char *buf;
    size_t size;
    uint32_t index;
    uint32_t after_dot_index;
    bool after_dot;
    char c;
    unsigned int line;
    unsigned int column;
    Pos posToken;

    void nextChar();
    void acceptChar(char);
    void positionToNextLine();
    inline void consumeWhitespaces();

    Token createOp(TokenKind kind);

    Pos pos(int);
};

#endif /* LEXER_H_ */
