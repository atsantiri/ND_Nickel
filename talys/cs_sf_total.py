import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

m0 = 57.935341
m1 = 1.00782503190
z0 = 28
z1 = 1


def two_pi_eta(E):
    c = 0.989534
    a = c*z0*z1*np.sqrt(m0*m1/(m0+m1)*1/E)
    return a

def s_factor(cs,E):
    c = two_pi_eta(E)
    s = E*cs*np.exp(c)
    return s
filename = 'total'

data = pd.read_table(filename+'.tot',skiprows=5,sep='\s+')
data.columns=['E','Non-elastic','Elastic','Total','Comp. el.','Shape el.','Reaction','Comp. nonel','Direct','Pre-equil.','Dir. Capt.']
ene = data['E']
comp_el = data['Comp. el.']
total = data['Reaction']
s_comp_el = s_factor(comp_el,ene)
s_tot = s_factor(total,ene)

data2 = {'Energy':ene,'Compound Elastic':s_comp_el,'Total':s_tot}
df = pd.DataFrame(data2)
print(df)
df.to_csv(filename+'_s.tot',sep='\t',index=None)

plt.figure(1)
plt.subplot(211)
plt.plot(ene,comp_el)
plt.plot(ene,total)
plt.yscale('log')
plt.title('Cross Section for Comp. El. and tot.')

plt.subplot(212)
plt.plot(ene,s_comp_el)
plt.plot(ene,s_tot)
plt.yscale('log')
plt.title('S factor')

#plt.show()

with open(filename+'_s.tot','r+') as f:
    file_data=f.read()
    f.seek(0,0)
    f.write('# '+ file_data)
