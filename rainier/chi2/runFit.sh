#!/bin/bash

expFile="exp_input_run_1205_multi2.root"
simFile="sim_hsts_Jp_T1.05_multi2.root"
outName="_Jp_T1.05_multi2"
root -l << EOF
.L fit_spectra.cpp
fit_spectra("$outName",3,7,"$simFile","$expFile","TAS","Seg","Mul","Minuit","Migrad",1E8,1E6,0.0001)
.q
EOF