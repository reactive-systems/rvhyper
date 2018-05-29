# README - RVHyper

## 1. Build RVHyper

### 1.1. Dependencies
- [spot](https://spot.lrde.epita.fr/) 
- [EAHyper](https://github.com/reactive-systems/eahyper) (recommended):
    Provide as executable named *eahyper* in working directory when executing RVHyper.

### 1.2. Build
The given Makefile provides the functionality to build RVHyper.
A singe make command should do the job. The binary *rvhyper* gets placed into build/release/.
```
$ make
$ ls -lgoh --time-style=+ build/release/rvhyper
-rwxr-xr-x 1 170K  build/release/rvhyper
```

### 1.3. Demo
We also provide a short demo to check if the build was successful.
```
$ make demo
```

## 2. RVHyper Command-Line Interface

### 2.1. Specification
The specification to monitor for has to be a valid HyperLTL formula only containing forall-quantifiers.
The specification can be given to RVHyper either as a string parameter or contained in a file:
```
$ ./rvhyper -s 'forall x. forall y. (o_x <-> o_y) W ~(i_x <-> i_y)'
$ ./rvhyper -S observational_determinism.hltl
```

### 2.2. Online Monitoring
The typical way one wants to use RVHyper is with the option *\-\-stdin*, which is suited for online monitoring.
This reads commands as well as IO-events directly from the standard input.

#### 2.2.1. Commands
- print commands:
	- *print help*: list the available commands
	- *print specification*: print the specification
	- *print aps*: list the atomic propositions of the specification formula
	- *print stats*: print statistics of the running RVHyper instance
- session commands:
	- *session start*: start new monitoring session
	- *session end*: end active monitoring session
- *exit* / *quit*: exit RVHyper

#### 2.2.2. IO-Events
In order to provide one IO-event for the current monitoring session, one has to list the fulfilled input and output atomic propositions for the event. The format has to match the pattern
```
	[input (',' input)*] ';' [output (',' output)*]
```
This of course can only be used in an active session.

#### 2.2.3. Example
```
$ ./rvhyper -s 'forall x. forall y. (out_x <-> out_y) W !(in_x <-> in_y)' --stdin
Specification:
forall x.forall y. (out_x <-> out_y) W ~(in_x <-> in_y)
reflexive: true
symmetric: true
transitive: false

$ session start
$ in;out
$ in;
$ session end
$ session start
$ in;out
$ in;
$ in;
$ session end
$ print stats
seen traces: 2
trie size: 3
created instances: 1
done transitions: 2
$ session start
$ in;out
$ in;
$ in;out
 --> Monitor(2,3) detected violation at position 3
Counter Example:
Trace 2 | Trace 3
in;out | in;out
in | in
in | in;out
```

### 2.3. Offline Monitoring
RVHyper can also be used to monitor offline, where it has to be given recorded IO-event traces.

#### 2.3.1. Operation Modes
RVHyper currently knows two different modes of operation, which differ in how the IO-event traces (see below) get processed for monitoring.

- *\-\-parallel*: all traces get monitored in parallel
- *\-\-sequential*: traces get monitored one after another
- *\-\-sequential-debug*: Online Monitoring (see 2.2.) with given IO-event traces

#### 2.3.2. IO-Event Traces
IO-event traces have to be stored in files, where one file corresponds to one session and one line in the file corresponds to one IO-event. The lines have to match the pattern listed in 2.2.2., where only the fulfilled propositions must be listed.
Filesnames have to be listed as the last parameters for RVHyper:
```
$ ./rvhyper -S demo.hltl -- demo_a.tr demo_b.tr demo_c.tr
```

### 2.4. Verbosity
By default RVHyper outputs information about the formula and found violations to the specification.
Additionally there are options for even more verbose output, to suppress it or print extra statistics:

- *\-\-verbose*:
output extra information about the monitoring automaton, progress, IO-events, transitions, trace analysis
- *\-\-quiet*:
suppress most of the output
- *\-\-stats*:
output extra statistics such as computation time, traces seen, instances created, transistions taken
