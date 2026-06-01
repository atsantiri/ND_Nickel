#!/bin/bash

fname="cu63_Ex_10MeV_$1"
echo $fname
fIn="cu63/$fname.txt"
fOut="geant_outputs/$fname.root"
geantInput="/evtdata/spyrou-sim/tsantiri/ND_Ni/GEANT_SuN/input"
geantOutput="/evtdata/spyrou-sim/tsantiri/ND_Ni/GEANT_SuN/rootFiles/trial.root"

echo "Copying file on fishtank: $fIn"
scp $fIn tsantiri@nsclgw1.nscl.msu.edu:$geantInput
echo "==============================================================================="
echo "Starting GEANT on fishtank"
ssh tsantiri@nsclgw1.nscl.msu.edu <<EOF
ssh fishtank << INNER
cd /evtdata/spyrou-sim/tsantiri/ND_Ni/GEANT_SuN/
echo "running Geant on fishtank"
~/geant4_workdir/bin/Linux-g++/sunShare >> logfile
cp rootFiles/trial.root rootFiles/May2026/$fname.root
INNER
EOF
echo "==============================================================================="
echo "Copying to local file $fOut"
scp tsantiri@nsclgw1.nscl.msu.edu:$geantOutput $fOut
