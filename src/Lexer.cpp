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

#include "Lexer.h"

#include "LexerUtil.h"
#include "diagnostic.h"
#include <stdio.h>
#include <string.h>

#define TK TokenKind

Lexer::Lexer(const char *formula) {
    size_t size = strlen(formula);
    char *buffer = new char[size + 1];
    if (buffer == nullptr) {
        errorf("memory error");
        return;
    }
    this->buf = strcpy(buffer, formula);
    this->size = size;
    this->filename = "<string>";
    index = 0;
    after_dot_index = 0;
    after_dot = true;
    c = buf[0];
    line = 1;
    column = 1;
}

Lexer::Lexer(std::string filename) {
    FILE *pFile = fopen(filename.c_str(), "rb");
    if (pFile == nullptr) {
        errorf(Pos(filename.c_str()), "file error");
        return;
    }
    fseek(pFile, 0, SEEK_END);
    size_t size = ftell(pFile);
    rewind(pFile);
    char *buffer = new char[size];
    if (buffer == nullptr) {
        errorf(Pos(filename.c_str()), "memory error");
        return;
    }
    buffer = new char[size + 1];
    size_t result = fread(buffer, sizeof(char), size, pFile);
    if (result != size) {
        errorf(Pos(filename.c_str()), "reading error");
        return;
    }
    buffer[size] = 0;

    fclose(pFile);

    this->buf = buffer;
    this->size = size;
    this->filename = filename;
    index = 0;
    after_dot_index = 0;
    after_dot = true;
    c = buf[0];
    line = 1;
    column = 1;
}

Lexer::~Lexer() {}

void
Lexer::nextChar() {
    c = buf[++index];
    column++;
}

void
Lexer::acceptChar(char d) {
    if (c == d) {
        nextChar();
    }
}

void
Lexer::positionToNextLine() {
    line++;
    column = 1;
}

Token
Lexer::createOp(TokenKind kind) {
    nextChar();
    return Token(posToken, kind);
}

Token
Lexer::nextToken() {
    //    printf("c:%d\n",c);
    //    printf("index:%d\n",index);
    //    printf("size:%ld\n",size);
    consumeWhitespaces();
    posToken = pos(0);
    switch (c) {
    case 0:
        return Token(posToken, TK::END_OF_FILE);

    // punctuators
    case '.':
        nextChar();
        after_dot_index = index;
        after_dot = true;
        return Token(posToken, TK::DOT);

    default:
        if (isIdentifierNonDigit(c)) {
            const uint32_t id_start = index;
            do {
                nextChar();
            } while (isIdentifierChar(c));
            std::string id = std::string(&buf[id_start], index - id_start);
            if (isKeyword(id)) {
                after_dot = false;
                return Token(posToken, pickTokenKind());
            }
            if (after_dot) {
                std::string formula =
                    std::string(&buf[id_start], size - id_start);
                index = size - 1;
                nextChar();
                after_dot = false;
                return Token(posToken, TK::FORMULA, formula);
            }
            return Token(posToken, TK::IDENTIFIER, id);
        } else {
            std::string formula =
                std::string(&buf[after_dot_index], size - after_dot_index);
            index = size - 1;
            nextChar();
            after_dot = false;
            return Token(posToken, TK::FORMULA, formula);
        }
    }
}

void
Lexer::consumeWhitespaces() {
    while (true) {
        switch (c) {
        case ' ':
        case '\t':
            nextChar();
            continue;
        case '\n':
            nextChar();
            positionToNextLine();
            continue;
        case '\r':
            nextChar();
            acceptChar('\n');
            positionToNextLine();
            continue;
        default:
            return;
        }
    }
}

Pos
Lexer::pos(int offset) {
    return Pos(filename.c_str(), line, column + offset);
}

#undef TK
