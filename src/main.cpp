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

#include <iostream>
#include <stdexcept>

#include <libgen.h>
#include <stdio.h>
#include <string.h>

#include "CachedTrace.h"
#include "Event.h"
#include "FileTrace.h"
#include "Lexer.h"
#include "LiveMonitoring.h"
#include "NoLiveMonitoring.h"
#include "Parser.h"
#include "RememberingSelectionHelper.h"
#include "SelectionHelper.h"
#include "diagnostic.h"
#include "flags.h"
#include "getopt.h"
#include "timer.h"

enum class Mode {
    PARALLEL,
    SEQUENTIAL_OFFLINE,
    SEQUENTIAL_ONLINE,
    SEQUENTIAL_ONLINE_DEBUG
};
enum class FormulaInputType { NONE, STRING, FILE };

static void
print_usage(const char *name) {
    printf(
        "usage: %s [OPTIONS] [IO-event files...]\n\n"
        "optional arguments:\n"
        "  -s specification       specification\n"
        "  -S specification_file  specification file\n"
        "  -h, --help             show this help message and exit\n"
        "  --parallel             parallel monitoring (default)\n"
        "  --sequential           sequential monitoring\n"
        "  --sequential-debug     sequential monitoring debug\n"
        "  --stdin                read from stdin (default: false)\n"
        "                         uses sequential monitoring\n"
        "  --verbose              print more information and progress "
        "(default: false)\n"
        "  --stats                print statistics at the end of the execution "
        "(default: false)\n"
        "  --quiet                suppress information output (default: "
        "false)\n",
        name);
}

static void
print_licence_notice() {
    printf(
        "Copyright (C) 2018  Marvin Stenger (Reactive Systems Group @ Saarland "
        "University)\n"
        "License GPLv3: GNU GPL version 3 <http://gnu.org/licenses/gpl.html>.\n"
        "This is free software, and you are welcome to\n"
        "redistribute it under certain conditions.\n"
        "This program comes with ABSOLUTELY NO WARRANTY.\n\n");
}

static void
print_version() {
    printf("RVHyper v0.2.3  (Jan 17, 2020)\n\n");

    print_licence_notice();

    printf("This software builds on top of the following projects:\n"
           "spot, <https://spot.lrde.epita.fr>\n"
           "libcperciva getopt, <https://github.com/Tarsnap/libcperciva>\n\n");
}

static void
print_commands() {
    fprintf(
        stderr,
        "Commands:\n"
        "- print commands:\n"
        "    print help: list the available commands\n"
        "    print specification: print the specification\n"
        "    print aps: list the atomic propositions of the specification "
        "formula\n"
        "    print stats: print statistics of the running RVHyper instance\n"
        "\n"
        "- session commands:\n"
        "    session start: signals start of a new monitoring session\n"
        "    session end: signals end of monitoring session\n"
        "\n"
        "- exit / quit: exit RVHyper\n"
        "\n"
        "- IO-event:\n"
        "    Provide one IO-event for the current monitoring session.\n"
        "    The format has to match the pattern "
        "[input(,input)*];[output(,output)*].\n");
}

int
main(int argc, char *const argv[]) {
    Mode mode = Mode::PARALLEL;
    FormulaInputType fit = FormulaInputType::NONE;
    std::string formula_param;
    std::vector<std::string> trace_files;

    // Handling of command line arguments
    // opterr = 0; // Disable error messages as we handle them ourself
    const char *ch;
    while ((ch = GETOPT(argc, argv)) != NULL) {
        GETOPT_SWITCH(ch) {
            GETOPT_OPT("-h") : print_usage(argv[0]);
            return 0;
            GETOPT_OPT("--help") : print_usage(argv[0]);
            return 0;
            GETOPT_OPT("--version") : print_version();
            return 0;

            GETOPT_OPT("--parallel") : mode = Mode::PARALLEL;
            break;
            GETOPT_OPT("--sequential") : mode = Mode::SEQUENTIAL_OFFLINE;
            break;
            GETOPT_OPT("--sequential-debug")
                : mode = Mode::SEQUENTIAL_ONLINE_DEBUG;
            break;
            GETOPT_OPT("--stdin") : mode = Mode::SEQUENTIAL_ONLINE;
            break;
            GETOPT_OPT("--verbose") : FLAG_VERBOSE = true;
            break;
            GETOPT_OPT("--stats") : FLAG_STATS = true;
            break;
            GETOPT_OPT("--quiet") : FLAG_QUIET = true;
            break;

            GETOPT_OPTARG("-s") : formula_param = std::string(optarg);
            fit = FormulaInputType::STRING;
            break;
            GETOPT_OPTARG("-S") : formula_param = std::string(optarg);
            fit = FormulaInputType::FILE;
            break;

        GETOPT_MISSING_ARG:
            printf("missing argument to %s\n", ch);
        /* FALLTHROUGH */
        GETOPT_DEFAULT:
            printf("unknown argument %s\n", ch);
            print_usage(argv[0]);
            return 1;
        }
    }

    for (; optind < argc; ++optind) {
        trace_files.push_back(std::string(argv[optind]));
    }

    if (hasNewErrors()) {
        return printDiagnosticSummary();
    }
    if (mode == Mode::SEQUENTIAL_ONLINE && !FLAG_QUIET) {
        print_licence_notice();
    }
    Lexer *lexer;
    switch (fit) {
    case FormulaInputType::STRING:
        lexer = new Lexer(formula_param.c_str());
        break;
    case FormulaInputType::FILE:
        lexer = new Lexer(formula_param);
        break;
    default:
        std::cerr << "no specification given" << std::endl;
        return 3;
    }

    if (hasNewErrors()) {
        return printDiagnosticSummary();
    }

    Parser parser = Parser(lexer);
    Formula formula = parser.parseFormula();
    if (hasNewErrors()) {
        return printDiagnosticSummary();
    }
    if (!formula.valid()) {
        std::cerr << "Specification not valid." << std::endl;
        return 2;
    }
    if (!FLAG_QUIET || FLAG_VERBOSE) {
        std::cout << "Specification:" << std::endl;
        formula.print(std::cout);
        std::cout << std::endl;
        if (!FLAG_QUIET || FLAG_VERBOSE) {
            std::cout << "reflexive: "
                      << (formula.reflexive() ? "true" : "false") << std::endl;
            std::cout << "symmetric: "
                      << (formula.symmetric() ? "true" : "false") << std::endl;
            std::cout << "transitive: "
                      << (formula.transitive() ? "true" : "false") << std::endl;
            std::cout << std::endl;
        }
    }

    int retVal = 0;

    switch (mode) {
    case Mode::PARALLEL: {
        NoLiveMonitoring m(formula);
        if (FLAG_VERBOSE) {
            m.printAutomatonCharacteristics(std::cout);
        }
        std::vector<Trace *> traces(trace_files.size());
        for (size_t i = 0; i < trace_files.size(); ++i) {
            traces[i] = new FileTrace(i + 1, trace_files[i]);
        }
        timer ti = timer();
        ti.Start();
        if (!m.monitor(traces)) {
            retVal = 1;
        }
        if (FLAG_STATS) {
            std::cout << "computation time: " << ti.GetTicks() / 1000 << "ms"
                      << std::endl;
            ti.Stop();
            std::cout << "seen traces: " << traces.size() << std::endl;
            m.printStats();
        }
        return retVal;
    }
    case Mode::SEQUENTIAL_OFFLINE: {
        NoLiveMonitoring m(formula);
        if (FLAG_VERBOSE) {
            m.printAutomatonCharacteristics(std::cout);
        }
        timer ti = timer();
        ti.Start();
        size_t i = 0;
        for (; i < trace_files.size(); ++i) {
            FileTrace t(i + 1, trace_files[i]);
            if (FLAG_VERBOSE) {
                std::cout << "process " << t.name() << "..." << std::endl;
            }
            if (!m.monitor(&t)) {
                retVal = 1;
                break;
            }
        }
        if (FLAG_STATS) {
            std::cout << "computation time: " << ti.GetTicks() / 1000 << "ms"
                      << std::endl;
            ti.Stop();
            std::cout << "seen traces: "
                      << (i < trace_files.size() ? i + 1 : trace_files.size())
                      << std::endl;
            m.printStats();
        }
        return retVal;
    }
    case Mode::SEQUENTIAL_ONLINE_DEBUG: {
        LiveMonitoring m(formula);
        if (FLAG_VERBOSE) {
            m.printAutomatonCharacteristics(std::cout);
        }
        timer ti = timer();
        ti.Start();
        for (size_t i = 0; i < trace_files.size(); i++) {
            FileTrace t(i + 1, trace_files[i]);
            if (FLAG_VERBOSE) {
                std::cout << "Process " << t.name() << std::endl;
            }
            m.initSession();
            while (t.next()) {
                if (!m.monitorEvent(t.currentEvent())) {
                    retVal = 1;
                    goto exit;
                }
            }
            m.terminateSession();
        }
    exit:
        if (FLAG_STATS) {
            std::cout << "computation time: " << ti.GetTicks() / 1000 << "ms"
                      << std::endl;
            ti.Stop();
            m.printStats();
        }
        return retVal;
    }
    case Mode::SEQUENTIAL_ONLINE: {
        LiveMonitoring m(formula);
        if (FLAG_VERBOSE) {
            m.printAutomatonCharacteristics(std::cout);
        }
        timer ti = timer();
        ti.Start();
        bool active = false;
        int i = 0;
        Row row;
        std::cout << (FLAG_QUIET ? "" : "$ ");
        for (std::string line; std::getline(std::cin, line);
             std::cout << (FLAG_QUIET ? "" : "$ ")) {
            if (line == "session start") {
                if (active) {
                    std::cout << "session already active" << std::endl;
                    continue;
                }
                // trace start
                m.initSession();
                active = true;
            } else if (line == "session end") {
                if (!active) {
                    std::cout << "no active session" << std::endl;
                    continue;
                }
                // trace end
                m.terminateSession();
                active = false;
                i++;
            } else if (line == "print aps") {
                m.printAPs();
            } else if (line == "print specification") {
                formula.print(std::cout);
                std::cout << std::endl;
            } else if (line == "print stats") {
                m.printStats();
            } else if (line == "print help") {
                print_commands();
            } else if (line == "exit" || line == "quit") {
                break;
            } else {
                // trace event
                if (active) {
                    row.readLine(line);
                    if (!m.monitorEvent(row.event)) {
                        retVal = 1;
                        break;
                    }
                } else {
                    std::cout << "invalid command/input \"" << line << "\""
                              << std::endl;
                    std::cout << "Type \"print help\" for command overview."
                              << std::endl;
                }
            }
        }
        if (!FLAG_QUIET)
            std::cout << std::endl;
        if (FLAG_STATS) {
            std::cout << "computation time: " << ti.GetTicks() / 1000 << "ms"
                      << std::endl;
            ti.Stop();
            m.printStats();
        }
        return retVal;
    }
    default:
        break;
    }
    return 0;
}
