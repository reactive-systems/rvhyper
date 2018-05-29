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

#include "Token.h"

#include "LexerUtil.h"
#include "diagnostic.h"

Token::Token() : pos(Pos("")), kind(TokenKind::ERROR) {}

Token::Token(Pos pos, TokenKind kind) : pos(pos), kind(kind) {}

Token::Token(Pos pos, TokenKind kind, std::string text)
    : pos(pos), kind(kind), text(text) {}

void
Token::print() {
    TokenType type = kind2type[int(kind)];
    char const *str_type = type2name[int(type)];
    switch (type) {
    case TokenType::CONSTANT:
    case TokenType::IDENTIFIER:
    case TokenType::FORMULA:
        printf("%s:%d:%d: %s %s\n", pos.name, pos.line, pos.column, str_type,
               text.c_str());
        break;
    case TokenType::KEYWORD:
    case TokenType::PUNCTUATOR:
        char const *name = kind2name[int(kind)];
        printf("%s:%d:%d: %s %s\n", pos.name, pos.line, pos.column, str_type,
               name);
        break;
    }
}

std::string
Token::toString() {
    switch (kind) {
    case TokenKind::IDENTIFIER:
    case TokenKind::FORMULA:
        return text;
    default:
        return kind2name[int(kind)];
    }
}
