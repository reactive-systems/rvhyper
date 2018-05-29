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

#ifndef TOKENENUMS_H_
#define TOKENENUMS_H_

enum class TokenKind {
    // DO NOT CHANGE ORDER, or change in LexerUtil.cpp as well
    ERROR = 0,
    END_OF_FILE,

    IDENTIFIER,

    FORMULA,

    // keywords
    FORALL, // forall
    EXISTS, // exists

    // punctuators
    DOT // .

};

// see 6.4p1
enum class TokenType {
    // DO NOT CHANGE ORDER, or change in LexerUtil.cpp as well
    KEYWORD = 0,
    CONSTANT,
    IDENTIFIER,
    FORMULA,
    PUNCTUATOR
};

enum class TokenPrecedence {
    // DO NOT CHANGE ORDER, or change in LexerUtil.cpp as well
    NONE = 0
};

#define K TokenKind
#define T TokenType
#define P TokenPrecedence

static TokenType kind2type[] = {
    // DO NOT CHANGE ORDER
    T::CONSTANT, // ERROR
    T::CONSTANT, // END_OF_FILE

    T::IDENTIFIER, // IDENTIFIER

    T::FORMULA, // FORMULA

    T::KEYWORD, // FORALL
    T::KEYWORD, // EXISTS

    T::PUNCTUATOR, // DOT
};

/*
 * Maps every element of TokenKind to its string-representation. If there
 * is no direct representation, it maps the element to something like <...>.
 */
static char const *kind2name[] = {
    // DO NOT CHANGE ORDER
    "<error>",       // ERROR
    "<end_of_file>", // END_OF_FILE

    "<identifier>", // IDENTIFIER
    "<formula>",    // FORMULA

    "forall", // FORALL
    "exists", // EXISTS

    ".", // DOT
};

static char const *type2name[] = {
    // DO NOT CHANGE ORDER
    "keyword", "constant", "identifier", "formula", "punctuator"};

#undef K
#undef T
#undef P

#endif /* TOKENENUMS_H_ */
