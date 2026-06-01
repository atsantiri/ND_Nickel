import numpy as np
import matplotlib.pyplot as plt
import sys
import pandas as pd
import nums_from_string

# question about isotope to set folder name
if len(sys.argv) == 1:
    iso = input("Which nickel? ")
else:
    iso = sys.argv[1]
comp=int(iso)+1

oslo_file = '/home/artemis/Personal_Space/MSU/SuN_Stuff/ND_Nickel/oslo_analysis/ni'+str(iso)

if int(iso) == 64:
	oslo_file = str(oslo_file)+'/un80keV'
if int(iso) == 60:
	oslo_file = str(oslo_file)+'_all/ni60_take3_ann-cecilie_2' 
	
####################################################################
## TALYS FILE 
####################################################################
talys=pd.read_csv('/home/artemis/Personal_Space/MSU/SuN_Stuff/ND_Nickel/talys/gSF_band_cu'+str(comp)+'.txt',skiprows=0,sep=r'\s+',engine='python')
talys.columns = ['Energy','min','max']
plt.fill_between(talys['Energy'], talys['min'], talys['max'], label='TALYS',alpha=.4)

####################################################################
## ShapeIt FILE 
####################################################################

shape = pd.read_csv('/home/artemis/Personal_Space/MSU/SuN_Stuff/ND_Nickel/shapeIt_files/ni'+str(iso)+'_output.txt',sep=r'\s+',engine='python')
shape.columns = ['Energy','min','max']
plt.fill_between(shape['Energy'], shape['min'], shape['max'], label='ShapeIt',alpha=.4)

####################################################################
## OSLO FILE 
####################################################################
# find energy calibration params
a0=0
a1=0
with open(str(oslo_file)+'/strength.cpp','r') as fIn:
	for line in fIn:
		if line.startswith('   float a0 ='):
			a0=nums_from_string.get_nums(line)[1]
#			print(line, a0)
		if line.startswith('   float a1 ='):
			a1=nums_from_string.get_nums(line)[1]
#			print(line, a1)
# make energy array
ene = []
with open(str(oslo_file)+'/strength.nrm','r') as fene:
	num = len(fene.readlines())

pts = int((num+1)*0.5)	
for i in range(pts):
	e = a0 + a1*i
	ene.append(e*1e3)
#		print(i)
#print(ene)
#print(pts)

oslo = pd.DataFrame(ene,columns=['Energy'],dtype=float)
oslo_val= pd.read_csv(str(oslo_file)+'/strength.nrm',sep=r'\s+',skipfooter=pts,engine='python')
oslo_val.columns=['strength']
oslo_err= pd.read_csv(str(oslo_file)+'/strength.nrm',sep=r'\s+',skiprows=pts,engine='python')
oslo_err.columns=['err']
oslo['strength']=oslo_val['strength']
oslo['err']=oslo_err['err']
#print(oslo)
plt.errorbar(oslo['Energy'],oslo['strength'],oslo['err'],fmt='k.',label='Oslo')

plt.title(str(comp)+"Cu gSF")
plt.xlabel("Energy (keV)")
plt.ylabel("gSF")	
plt.yscale('log')
plt.legend()
plt.show()
