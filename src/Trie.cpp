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

#include "Trie.h"
#include <assert.h>
#include <iostream>

template <typename T>
TrieNode<T>::TrieNode(unsigned sessionID)
    : sessionID(sessionID), value(), parent(nullptr), children(), cTraces(0),
      state(-1) {}

template <typename T>
TrieNode<T>::TrieNode(unsigned sessionID, T value)
    : sessionID(sessionID), value(value), parent(nullptr), children(),
      cTraces(0), state(-1) {
    assert(sessionID != 0);
}

template <typename T> TrieNode<T>::~TrieNode() {
    for (auto c : children)
        delete c;
}

template <typename T>
TrieNode<T> *
TrieNode<T>::root() {
    if (isRoot())
        return this;
    return parent->root();
}

template <typename T>
std::vector<T>
TrieNode<T>::getPath() {
    if (isRoot()) {
        return std::vector<T>();
    }
    auto parentPath = parent->getPath();
    parentPath.push_back(value);
    return parentPath;
}

template <typename T>
unsigned
TrieNode<T>::depth() {
    if (isRoot()) {
        return 0;
    }
    return 1 + parent->depth();
}

template <typename T>
void
TrieNode<T>::removeSession(unsigned sessionID) {
    if (this->sessionID == sessionID) {
        parent->removeSession(sessionID);
        return;
    }
    for (auto it = children.begin(); it != children.end(); it++) {
        if ((*it)->sessionID == sessionID) {
            children.erase(it);
            break;
        }
    }
}

template <typename T>
TrieNode<T> *
TrieNode<T>::addValue(unsigned sessionID, T &value) {
    cTraces++;
    for (auto c : children) {
        if (c->value == value) {
            return c;
        }
    }
    auto new_child = new TrieNode(sessionID, value);
    new_child->parent = this;
    children.push_back(new_child);

    return new_child;
}

template <typename T>
size_t
TrieNode<T>::size() {
    size_t s = children.size();
    for (auto c : children)
        s += c->size();
    return s;
}

template <typename T>
void
TrieNode<T>::printChildCount(unsigned lvl) {
    if (lvl == 2)
        return;
    std::cerr << children.size() << std::endl;
    for (auto c : children)
        c->printChildCount(lvl + 1);
}

template struct TrieNode<Event>;
