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

#ifndef SRC_TRIE_H_
#define SRC_TRIE_H_

#include "Event.h"
#include <set>
#include <vector>

template <typename T> struct TrieNode {
    //  private:
    //    void realTreeNow();

  public:
    unsigned sessionID;
    T value;
    TrieNode<T> *parent;
    std::vector<TrieNode<T> *> children;
    unsigned cTraces;
    int state;

    TrieNode(unsigned sessionID);
    TrieNode(unsigned sessionID, T value);
    virtual ~TrieNode();

    bool
    isRoot() {
        return parent == nullptr;
    }

    bool
    isLeaf() {
        return children.empty();
    }

    TrieNode<T> *addValue(unsigned sessionID, T &value);
    TrieNode<T> *root();
    std::vector<T> getPath();
    unsigned depth();
    size_t size();
    void removeSession(unsigned sessionID);
    void printChildCount(unsigned lvl);
};

typedef TrieNode<Event> Trie;

#endif /* SRC_TRIE_H_ */
