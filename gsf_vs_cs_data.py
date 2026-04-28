import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

hc=197.32698
hc2=hc**2

def get_cs(ene,gsf):
	a=(3*np.pi*np.pi*hc2) # 3*pi*hc**2 in (MeV*fm)^-2
	cs=gsf*ene*a*1.0E-13
	return cs

data= pd.read_csv('63Cu_gn_cs.csv',skiprows=1)
data.columns=['plaisir_ene','plaisir_cs','plaisir_cs_err','sund_ene','sund_cs','sund_cs_err','dzhil_ene','dzhil_cs','dzhil_cs_err']

gf= pd.read_table('ni60_take3_ann-cecilie_2/strength.nrm')
gf.columns=['val']

a0=-.7040
a1=0.24
ene_list=[]
gsf_list=[]
gsf_err_list=[]

for i in range(len(gf.val)+1):
	if (i<45 and gf.val[i]>0):
		gsf_list.append(gf.val[i])
		ene_list.append(a0+(i)*a1)
	elif (i>=45 and gf.val[i-1]>0):
		gsf_err_list.append(gf.val[i-1])
ene=np.array(ene_list)
gsf=np.array(gsf_list)
gsf_err=np.array(gsf_err_list)

cs=get_cs(ene,gsf)
cs_err=get_cs(ene,gsf_err)

#plt.errorbar(data.plaisir_ene,data.plaisir_cs,yerr=data.plaisir_cs_err,marker='.',ls='none')
#plt.errorbar(data.sund_ene,data.sund_cs,yerr=data.sund_cs_err,marker='.',ls='none')
#plt.errorbar(data.dzhil_ene,data.dzhil_cs,yerr=data.dzhil_cs_err,marker='.',ls='none')
plt.errorbar(ene,cs,yerr=cs_err,marker='.',ls='none')
plt.title("63Cu")
plt.xlabel("Energy (MeV)")
plt.ylabel("Cross Section (b)")

plt.show()

#plt.errorbar(ene,gsf,yerr=gsf_err,marker='.',ls='none')
#plt.yscale('log')
#plt.show()

