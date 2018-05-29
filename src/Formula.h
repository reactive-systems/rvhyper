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

#ifndef FORMULA_H_
#define FORMULA_H_

#include <set>
#include <spot/tl/formula.hh>
#include <spot/twa/twagraph.hh>
#include <string>
#include <vector>

class Formula {
  private:
    std::vector<std::string> exists_vec;
    std::vector<std::string> forall_vec;
    spot::formula body;
    std::string body_str;
    bool ref, ref_done, symm, symm_done, trans, trans_done;

    bool valid_ap(const spot::formula &f, std::set<std::string> &tvar_set);

    bool hasProperty(std::string property);

  public:
    Formula(std::vector<std::string> &es, std::vector<std::string> &fs,
            std::string &body);

    void print(std::ostream &str);
    std::ostream &operator<<(std::ostream &str);
    bool valid();
    bool reflexive();
    bool symmetric();
    bool transitive();
    size_t tracevarsSize();
    std::vector<std::string> tracevars();
    spot::const_twa_graph_ptr translate2monitor();
};

#endif /* FORMULA_H_ */
