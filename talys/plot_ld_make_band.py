import numpy as np
import matplotlib.pyplot as plt
import sys
import pandas as pd
import nums_from_string

def get_rho_shifted(data,cshift,pshift):
	data['E_shift']=data['Energy']-pshift
	ld_shift = []
	#data['ld_shift']=np.nan
#	print(data)
	for entry in range(len(data['Energy'])):
		temp=np.abs(np.array(data.Energy)-data.E_shift[entry]).argmin()
		ld_shift.append(data.loc[data['Energy']==data.Energy[temp],'Total l.d.'].iloc[0])

	data['ld_shift']=ld_shift		
	rho = np.exp(cshift*np.sqrt(data.E_shift))*data.ld_shift
	#print(data)
	return rho
	

# question about isotope to set folder name
if len(sys.argv) == 1:
    iso = input("Which nickel? ")
else:
    iso = sys.argv[1]
comp=int(iso)+1

# read in the first LD file to keep energy and ld in out output dataframe
data=pd.read_csv('ni'+str(iso)+'/ldcu'+str(comp)+'_1.tot',skiprows=12,sep=r'\s+',skipfooter=1,engine='python')
if (len(data.columns) == 6): # some files have less columns, since they come from table of data
	data.columns=['Energy','Level','N_cumulative','ld 1','a','Sigma']
	data=data.drop(columns=['Level','N_cumulative','a','Sigma'])
elif (len(data.columns) == 4):
	data.columns=['Energy','Level','N_cumulative','ld 1']
	data=data.drop(columns=['Level','N_cumulative'])
else:
	print('File '+'ldcu'+str(comp)+'_1.tot has weird number of columns.')
	sys.exit()

#print(data)	
ene=data['Energy']
ldtot=data['ld 1']

plt.plot(ene,ldtot,label='ld 1')


for i in range(2,7):
	ptable=0
	ctable=0
	if (i>3): # take into account ctable / ptable shifts
		with open('ni'+str(iso)+'/ldcu'+str(comp)+'_'+str(i)+'.tot','r') as fIn:
			for line in fIn:
				for char in line.split():
					if str(char)=='ptable':
						ptable=nums_from_string.get_nums(line)
#						print(i,line)
#						print(ptable)
					if str(char)=='ctable':
						ctable=nums_from_string.get_nums(line)
#						print(i,line)
#						print(ctable)


	add=pd.read_csv('ni'+str(iso)+'/ldcu'+str(comp)+'_'+str(i)+'.tot',skiprows=12,sep=r'\s+',skipfooter=1,engine='python')

	if (len(add.columns) == 6):
		add.columns=['Energy','Level','N_cumulative','Total l.d.','a','Sigma']
	elif (len(add.columns) == 4):
		add.columns=['Energy','Level','N_cumulative','Total l.d.']
	else:
		print('File '+'ldcu'+str(comp)+'_'+str(i)+'.tot has weird number of columns.')
		break	
	ene_new=add['Energy']
	ldtot_new=get_rho_shifted(add,ctable,ptable)
#	print('----------------------------------')
	data['ld '+str(i)]=ldtot_new # for the rest I only keep the LD value in my dataframe
	plt.plot(ene_new,ldtot_new,label='ld '+str(i))
#print(data)
# find min and max
min_ld=data[['ld 1', 'ld 2', 'ld 3', 'ld 4', 'ld 5', 'ld 6']].min(axis = 1)
data['min']=min_ld
max_ld=data[['ld 1', 'ld 2', 'ld 3', 'ld 4', 'ld 5', 'ld 6']].max(axis = 1)
data['max']=max_ld

# save all columns in one file
data[['Energy','ld 1', 'ld 2', 'ld 3', 'ld 4', 'ld 5', 'ld 6']].to_csv('ld_all_cu'+str(comp), sep = '\t', index=False, header=False)

# save in new file only the columns I want
data[['Energy', 'min', 'max']].to_csv('ld_band_cu'+str(comp), sep = '\t', index=False, header=False)


plt.title(str(comp)+"Cu Level Density")
plt.xlabel("Energy (MeV)")
plt.ylabel("Total LD")	
plt.yscale('log')
plt.legend()
plt.show()




