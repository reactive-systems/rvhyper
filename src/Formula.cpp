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

#include "Formula.h"

#include "flags.h"
#include <fstream>
#include <spot/tl/parse.hh>
#include <spot/twaalgos/translate.hh>

Formula::Formula(std::vector<std::string> &es, std::vector<std::string> &fs,
                 std::string &body)
    : exists_vec(es), forall_vec(fs), body_str(body), ref(false),
      ref_done(false), symm(false), symm_done(false), trans(false),
      trans_done(false) {
    spot::parsed_formula pf = spot::parse_infix_psl(body);
    if (pf.format_errors(std::cerr))
        exit(2);
    this->body = pf.f;
};

void
Formula::print(std::ostream &out) {
    for (auto id : exists_vec) {
        out << "exists " << id << ".";
    }
    for (auto id : forall_vec) {
        out << "forall " << id << ".";
    }
    out << body_str;
}

std::ostream &
Formula::operator<<(std::ostream &out) {
    print(out);
    return out;
}

bool
Formula::valid() {
    if (!body.is_ltl_formula()) {
        return false;
    }
    std::set<std::string> tvar_set;
    std::pair<std::set<std::string>::iterator, bool> ret;
    for (auto e : exists_vec) {
        return false;
        ret = tvar_set.insert(e);
        if (ret.second == false) {
            // e used twice
            return false;
        }
    }
    for (auto f : forall_vec) {
        ret = tvar_set.insert(f);
        if (ret.second == false) {
            // f used twice
            return false;
        }
    }
    return valid_ap(body, tvar_set);
}

bool
Formula::valid_ap(const spot::formula &f, std::set<std::string> &tvar_set) {
    if (f.is(spot::op::ap)) {
        std::string ap_name = f.ap_name();
        size_t found = ap_name.find_last_of('_');
        if (found != std::string::npos) {
            if (found < ap_name.length() - 1) {
                std::string tvar = ap_name.substr(found + 1);
                if (tvar_set.find(tvar) == tvar_set.end()) {
                    // ap_id not in id_set
                    return false;
                }
                // ap_id in id_set
                return true;
            }
            // _ at end of ap
            return false;
        }
        // no ap_id
        return false;
    }

    for (auto cf = f.begin(); cf != f.end(); cf++) {
        if (!valid_ap(*cf, tvar_set)) {
            return false;
        }
    }
    return true;
}

size_t
Formula::tracevarsSize() {
    return forall_vec.size();
}

// source:
// http://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
// author: iFreilicht
template <typename... Args>
std::string
string_format(const std::string &format, Args... args) {
    size_t size = snprintf(nullptr, 0, format.c_str(), args...) +
                  1; // Extra space for '\0'
    std::unique_ptr<char[]> buf(new char[size]);
    snprintf(buf.get(), size, format.c_str(), args...);
    return std::string(buf.get(),
                       buf.get() + size - 1); // We don't want the '\0' inside
}

bool
Formula::hasProperty(std::string property) {
    bool p = false;
    std::string tmpn = std::tmpnam(nullptr);
    std::ofstream ofs(tmpn, std::ostream::out | std::ofstream::trunc);
    if (ofs.is_open()) {
        print(ofs);
        ofs << std::endl;
        ofs.close();
        std::string detect_path = "./eahyper";
        int ret =
            system(string_format("%s -f '%s' -%s|grep -e '^%s' >/dev/null",
                                 detect_path.c_str(), tmpn.c_str(),
                                 property.c_str(), property.c_str())
                       .c_str());
        p = WEXITSTATUS(ret) == 0;
        remove(tmpn.c_str());
    } else {
        // for now fall-back to false instead of exception
        //        std::cerr << "could not open tmpfile!" << std::endl;
        throw std::runtime_error("could not open tmpfile");
        p = false;
    }
    return p;
}

bool
Formula::reflexive() {
    if (!ref_done) {
        ref = hasProperty("r");
        ref_done = true;
    }
    return ref;
}

bool
Formula::symmetric() {
    if (!symm_done) {
        symm = hasProperty("s");
        symm_done = true;
    }
    return symm;
}

bool
Formula::transitive() {
    if (!trans_done) {
        trans = (this->forall_vec.size() == 2) && hasProperty("t");
        trans_done = true;
    }
    return trans;
}

std::vector<std::string>
Formula::tracevars() {
    std::vector<std::string> tracevars;
    tracevars.insert(tracevars.end(), exists_vec.begin(), exists_vec.end());
    tracevars.insert(tracevars.end(), forall_vec.begin(), forall_vec.end());
    return tracevars;
}

spot::const_twa_graph_ptr
Formula::translate2monitor() {
    spot::translator trans;
    trans.set_type(spot::postprocessor::Monitor);
    trans.set_pref(spot::postprocessor::Deterministic |
                   spot::postprocessor::Complete);
    spot::const_twa_graph_ptr aut = trans.run(body);
    return aut;
}
