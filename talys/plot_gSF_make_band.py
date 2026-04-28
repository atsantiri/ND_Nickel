import numpy as np
import matplotlib.pyplot as plt
import sys
import pandas as pd
import os 

# question about isotope to set folder name
if len(sys.argv) == 1:
    iso = input("Which nickel? ")
else:
    iso = sys.argv[1]
comp=int(iso)+1

# Empty output dataframe
out = pd.DataFrame()
# read in E1 and keep energies column in output dataframe
e1=pd.read_csv('ni'+str(iso)+'/E1cu'+str(comp)+'_1',skiprows=1,sep=r'\s+',engine='python')
e1.columns=['Energy','gSF 1']
#print(data)	
ene=e1['Energy']*1e3
gsfe=e1['gSF 1']

out['Energy']=ene

f=plt.figure()
f.set_size_inches((10,10),forward=False)

# open different M1 output files and add eacj m1 to the e1 above. then add total m1+e1 in output dataframe
for i in range(1,4):
	m1=pd.read_csv('ni'+str(iso)+'/M1cu'+str(comp)+'_'+str(i),skiprows=1,sep=r'\s+',engine='python')
	m1.columns=['Energy','gsf M1']
	gsfm=m1['gsf M1']
	gsftot=gsfe+gsfm
	out['E1_1 M1_'+str(i)]=gsfe+gsfm
	plt.plot(ene,gsfe+gsfm,label='E1_1 M1_'+str(i))
	


for i in range(2,10):
	e1=pd.read_csv('ni'+str(iso)+'/E1cu'+str(comp)+'_'+str(i),skiprows=1,sep=r'\s+',engine='python')
	e1.columns=['Energy','E1']
	gsfe=e1['E1']
	for j in range(1,4):
		m1=pd.read_csv('ni'+str(iso)+'/M1cu'+str(comp)+'_'+str(j),skiprows=1,sep=r'\s+',engine='python')
		m1.columns=['Energy','gsf M1']
		gsfm=m1['gsf M1']
		gsftot=gsfe+gsfm
		out['E1_'+str(i)+' M1_'+str(j)]=gsfe+gsfm
		plt.plot(ene,gsfe+gsfm,label='E1_'+str(i)+' M1_'+str(j))

#print(out)

#find min and max
min_gsf=out[['E1_1 M1_1','E1_1 M1_2', 'E1_1 M1_3', 'E1_2 M1_1','E1_2 M1_2', 'E1_2 M1_3', 'E1_3 M1_1','E1_3 M1_2', 'E1_3 M1_3', 'E1_4 M1_1','E1_4 M1_2', 'E1_4 M1_3', 'E1_5 M1_1','E1_5 M1_2', 'E1_5 M1_3', 'E1_6 M1_1','E1_6 M1_2', 'E1_6 M1_3', 'E1_7 M1_1','E1_7 M1_2', 'E1_7 M1_3', 'E1_8 M1_1','E1_8 M1_2', 'E1_8 M1_3', 'E1_9 M1_1','E1_9 M1_2', 'E1_9 M1_3']].min(axis = 1)
out['min']=min_gsf
max_gsf=out[['E1_1 M1_1','E1_1 M1_2', 'E1_1 M1_3', 'E1_2 M1_1','E1_2 M1_2', 'E1_2 M1_3', 'E1_3 M1_1','E1_3 M1_2', 'E1_3 M1_3', 'E1_4 M1_1','E1_4 M1_2', 'E1_4 M1_3', 'E1_5 M1_1','E1_5 M1_2', 'E1_5 M1_3', 'E1_6 M1_1','E1_6 M1_2', 'E1_6 M1_3', 'E1_7 M1_1','E1_7 M1_2', 'E1_7 M1_3', 'E1_8 M1_1','E1_8 M1_2', 'E1_8 M1_3', 'E1_9 M1_1','E1_9 M1_2', 'E1_9 M1_3']].max(axis = 1)
out['max']=max_gsf


# save in new file only the columns I want
out[['Energy', 'min', 'max']].to_csv('gSF_band_cu'+str(comp)+'.txt', sep = '\t', index=False, header=False)
out[['Energy','E1_1 M1_1','E1_1 M1_2', 'E1_1 M1_3', 'E1_2 M1_1','E1_2 M1_2', 'E1_2 M1_3', 'E1_3 M1_1','E1_3 M1_2', 'E1_3 M1_3', 'E1_4 M1_1','E1_4 M1_2', 'E1_4 M1_3', 'E1_5 M1_1','E1_5 M1_2', 'E1_5 M1_3', 'E1_6 M1_1','E1_6 M1_2', 'E1_6 M1_3', 'E1_7 M1_1','E1_7 M1_2', 'E1_7 M1_3', 'E1_8 M1_1','E1_8 M1_2', 'E1_8 M1_3', 'E1_9 M1_1','E1_9 M1_2', 'E1_9 M1_3']].to_csv('gSF_all_cu'+str(comp)+'.txt', sep = '\t', index=False, header=True)

out['Ene_MeV'] = out['Energy']*1e-3
out[['Ene_MeV', 'min', 'max']].to_csv('gSF_band_cu'+str(comp)+'_oslo.txt', sep = '\t', index=False, header=False)


plt.title(str(comp)+"Cu gSF")
plt.xlabel("Energy (keV)")
plt.ylabel("gSF")	
plt.yscale('log')
plt.legend()
plt.savefig(str(comp)+"Cu_gSF.png")
plt.show()

out[['Ene_MeV', 'max']].to_csv('gSF_band_cu'+str(comp)+'_oslo_rev', sep = '\t', index=False, header=False)
out[['Ene_MeV', 'min']].to_csv('temp', sep = '\t', index=False, header=False)
fout = open('gSF_band_cu'+str(comp)+'_oslo_rev', "a")  
for line in reversed(list(open('temp'))):
	fout.write(str(line))
fout.close()
os.remove('temp')

