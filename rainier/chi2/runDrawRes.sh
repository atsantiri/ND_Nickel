#!/bin/bash

simFile="sim_hsts_Jp_noLEE_multi2.root"
outName="_Jp_noLEE_multi2"
root -l << EOF
.L draw_results.C
draw_results("$outName","$simFile")
.q
EOF