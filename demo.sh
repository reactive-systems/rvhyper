#!/bin/bash
echo -n 'forall x. forall y.
(
    (out_0_x <-> out_0_y)
    &(out_1_x <-> out_1_y)
)
W
~(
    (in_0_x <-> in_0_y)
    &(in_1_x <-> in_1_y)
)
' > demo.hltl

echo -n 'in_0,in_1;
;out_0
;out_1
;
;out_0,out_1
' > demo_a.tr

echo -n 'in_0,in_1;
;out_0
;out_1
;
;out_0,out_1
' > demo_b.tr

echo -n 'in_0,in_1;
;out_0
;out_0
;
;out_1
' > demo_c.tr

echo './build/release/rvhyper -S demo.hltl --parallel demo_a.tr demo_b.tr demo_c.tr'
./build/release/rvhyper -S demo.hltl demo_a.tr demo_b.tr demo_c.tr
echo
echo './build/release/rvhyper -S demo.hltl --sequential demo_a.tr demo_b.tr demo_c.tr'
./build/release/rvhyper --sequential -S demo.hltl demo_a.tr demo_b.tr demo_c.tr
echo './build/release/rvhyper -S demo.hltl --sequential-debug demo_a.tr demo_b.tr demo_c.tr'
./build/release/rvhyper -S demo.hltl --sequential-debug demo_a.tr demo_b.tr demo_c.tr

rm demo.hltl demo_a.tr demo_b.tr demo_c.tr
