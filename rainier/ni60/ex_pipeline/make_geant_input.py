# to make a geant input from a rainier output - Based on Hannah's better_root2geant
import numpy as np
import pandas as pd
import uproot
import os
import sys
###########################################################################################################################
#From our README file: The format for the input file is:
#		numberOfEventsToSimulate	numberOfCascades  numberOfGammaRaysPerCascade
#		intensityOfCascade1		energyGamma1		energyGamma2		energyGamma3.....
#		intensityOfCascade2		energyGamma1		energyGamma2		energyGamma3.....
#		intensityOfCascade3		energyGamma1		energyGamma2		energyGamma3.....
#		....									....				....						....
#	For exa0 the input file for simulating 100000 decays of Co-60 is
#			100000 1 2
#			100.0 1173 1332
###########################################################################################################################

# question about isotope to set folder name
if len(sys.argv) == 1:
    iso = input("Which nickel? ")
    ene = input("What Ex? ")
    real = input('How many to simulate? ')
elif len(sys.argv) == 2:
    iso = sys.argv[1]
    ene = input("What Ex? ")
    real = input('How many to simulate? ')
elif len(sys.argv) == 3:
    iso = sys.argv[1]
    ene = sys.argv[2]
    real = input('How many to simulate? ')
else:
    iso = sys.argv[1]
    ene = sys.argv[2]
    real = sys.argv[3]
  
comp=int(iso)+1

f = 'Run0001.root'
t = 'tree'
fOut_name = "ni"+str(iso)+"_Ex_"+str(ene)+"_mple.txt"

print("Reading tree {} in file {}".format(t, f))
data = uproot.open(f)[t]

names = data.keys()

#print("Tree has the following branches:")
#print("  [{}]".format(', '.join(names)))

data.arrays(names)
Egs = data["Egs"].array(library="np")
#print(type(Egs))
# some lines are full of zeros, so we want to remove that
# first find zero lines
zeros = []
for i in range(len(Egs)):
	if np.sum(Egs[i])<0.1:
		zeros.append(i)
		
print('Number of zero cascades ',len(zeros),' out of ',len(Egs),' simulated cascades')
# now remove these lines
if len(zeros)!=0:
	Egs = np.delete(Egs,zeros,axis=0)

#test to locate weird energies (larger or smaller by more than 100 keV)
lower_E = []
higher_E = []
for i in range(len(Egs)):
	if np.sum(Egs[i])>float(ene)+0.1:	
		higher_E.append(np.sum(Egs[i]))
	if np.sum(Egs[i])<float(ene)-0.1:	
		lower_E.append(np.sum(Egs[i]))
if len(lower_E)!=0:
	print("Some arrays have lower Ex")
	print(lower_E)
if len(higher_E)!=0:
	print("Some arrays have higher Ex")
	print(higher_E)		

events = len(Egs)
print('Events after removing zeros: ', events)
prob = 100/events
max_gammas = 20
total_to_simulate = int(real)

table = np.zeros((events,max_gammas+1))
table[:,0] = prob

for i in range(len(Egs)):
	n_gammas = len(Egs[i])
	for j in range(n_gammas):
		table[i][j+1]=np.round(Egs[i][j]*1e3,5)


with open(fOut_name,'w') as fOut:
	fOut.write(str(total_to_simulate)+' '+str(events)+' '+str(max_gammas)+'\n')
	fOut.write("\n".join(" ".join(map(str, x)) for x in table))
fOut.close()

