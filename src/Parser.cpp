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

#include <string>
#include <vector>

#include "Parser.h"

#include "diagnostic.h"

Parser::Parser(Lexer *lexer) : lexer(lexer) { nextToken(); }

Parser::~Parser() {}

Formula
Parser::parseFormula() {
    if (peek(TokenKind::END_OF_FILE)) {
        errorf(token.pos, "empty file");
    }
    std::vector<std::string> exists_vec;
    std::vector<std::string> forall_vec;
    std::string body;
    while (peek(TokenKind::EXISTS)) {
        nextToken();
        Token t = parseIdentifier();
        exists_vec.push_back(t.toString());
        expect(TokenKind::DOT);
    }
    while (peek(TokenKind::FORALL)) {
        nextToken();
        Token t = parseIdentifier();
        forall_vec.push_back(t.toString());
        expect(TokenKind::DOT);
    }
    if (peek(TokenKind::FORMULA)) {
        body = token.toString();
        nextToken();
    } else {
        errorf(token.pos, "expected formula, but got '%s'",
               kind2name[int(token.kind)]);
    }
    expect(TokenKind::END_OF_FILE);

    return Formula(exists_vec, forall_vec, body);
}

void
Parser::nextToken() {
    token = lexer->nextToken();
}

bool
Parser::peek(TokenKind t) {
    return (token.kind == t);
}

bool
Parser::accept(TokenKind t) {
    if (peek(t)) {
        nextToken();
        return true;
    } else {
        return false;
    }
}

void
Parser::expect(TokenKind t) {
    if (!accept(t)) {
        errorf(token.pos, "expected '%s', but got '%s'", kind2name[int(t)],
               token.toString().c_str());
    }
}

Token
Parser::parseIdentifier() {
    if (peek(TokenKind::IDENTIFIER)) {
        Token s = token;
        nextToken();
        return s;
    } else {
        errorf(token.pos, "expected identifier, but got '%s'",
               kind2name[int(token.kind)]);
        return Token(token.pos, TokenKind::IDENTIFIER, "<error>");
    }
}
