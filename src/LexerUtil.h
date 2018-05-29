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

#ifndef LEXERUTIL_H_
#define LEXERUTIL_H_

#include <string>
#include <unordered_map>

#include "TokenEnums.h"

static inline bool
isDigit(char c) {
    return c >= '0' && c <= '9';
}

static inline bool
isIdentifierNonDigit(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_';
}

static inline bool
isIdentifierChar(char c) {
    return isIdentifierNonDigit(c) || isDigit(c);
}

struct LexerUtil {
    static std::unordered_map<std::string, TokenKind> keyword2kind;
    static std::unordered_map<std::string, TokenKind>::const_iterator k2k_it;
};

static inline bool
isKeyword(std::string id) {
    LexerUtil::k2k_it = LexerUtil::keyword2kind.find(id);
    return LexerUtil::k2k_it != LexerUtil::keyword2kind.end();
}

// only call if keywordExists() returned true
static inline TokenKind
pickTokenKind() {
    return LexerUtil::k2k_it->second;
}

#endif /* LEXERUTIL_H_ */
