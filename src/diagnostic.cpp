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

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <errno.h>

#include "diagnostic.h"

void
panic(char const *const file, int const line, char const *const msg) {
    fprintf(stderr, "%s:%d: %s\n", file, line, msg);
    abort();
}

static void verrorf(Pos const *, char const *fmt, va_list);

static unsigned nErrors = 0;
static bool newErrors = false;

void
errorf(Pos const &pos, char const *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    verrorf(&pos, fmt, ap);
    va_end(ap);
}

void
errorf(char const *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    verrorf(nullptr, fmt, ap);
    va_end(ap);
}

bool
hasNewErrors() {
    auto const res = newErrors;
    newErrors = false;
    return res;
}

int
printDiagnosticSummary() {
    if (nErrors != 0) {
        fprintf(stderr, "%u error(s)\n", nErrors);
        return 1;
    }
    return 0;
}

static void
verrorf(Pos const *const pos, char const *fmt, va_list ap) {
    newErrors = true;
    ++nErrors;

    auto const out = stderr;

    if (pos) {
        fprintf(out, "%s:%u:%u: ", pos->name, pos->line, pos->column);
    }
    fputs("error: ", out);

    for (; auto f = strchr(fmt, '%'); fmt = f) {
        fwrite(fmt, 1, f - fmt, out);
        ++f; // Skip '%'.
        switch (*f++) {
        case '%':
            fputc('%', out);
            break;

        case 'c': {
            auto const c = (char)va_arg(ap, int);
            fputc(c, out);
            break;
        }

        case 's': {
            auto const s = va_arg(ap, char const *);
            fputs(s, out);
            break;
        }

        default:
            PANIC("invalid format specifier");
        }
    }
    fputs(fmt, out);
    fputc('\n', out);
    exit(1);
}
