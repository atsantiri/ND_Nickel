import numpy as np
import sys
import matplotlib.pyplot as plt
import pandas as pd

hc=197.32698
hc2=hc**2

def get_gsf(ene,cs):
	a=(np.pi*np.pi*hc2) # 3*pi*hc**2 in (MeV*fm)^-2
	gsf=(cs)/(3*ene*a*100)
	return gsf

if len(sys.argv) == 1:
    print("You can also give filename as a command line argument")
    filename = input("Enter .csv file (3 columns - energy, cross section and error): ")
else:
    filename = sys.argv[1]

data= pd.read_csv(str(filename)+'.csv',skiprows=1)
data.columns=['ene','cs','cs_err']
cs1=np.array(data.cs)
ene1=np.array(data.ene)
cserr1=np.array(data.cs_err)


gsf1=get_gsf(ene1,cs1)
gsferr1=get_gsf(ene1,cserr1)

data1 = {'Energy':ene1,'gSF':gsf1,'gSF err':gsferr1}
df = pd.DataFrame(data1)
print(df)
df.to_csv(str(filename)+'_gsf.csv',sep='\t',index=None)


plt.errorbar(ene1,gsf1,yerr=gsferr1,marker='.',ls='none')
plt.xlabel("Energy (MeV)")
plt.ylabel("gSF")
plt.yscale('log')
plt.show()


