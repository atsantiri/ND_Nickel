import numpy as np
import matplotlib.pyplot as plt
import sys
import pandas as pd
import nums_from_string

def get_rho_shifted(data,pshift,cshift):
	data['E_shift']=data['Energy']-pshift
	ld_shift = []
	#data['ld_shift']=np.nan
#	print(data)
	for entry in range(len(data['Energy'])):
		temp=np.abs(np.array(data.Energy)-data.E_shift[entry]).argmin()
		ld_shift.append(data.loc[data['Energy']==data.Energy[temp],'Total l.d.'].iloc[0])

	data['ld_shift']=ld_shift		
	rho = np.exp(cshift*np.sqrt(data.E_shift))*data.ld_shift
#	print(data)
	return rho
	

# question about isotope to set folder name
if len(sys.argv) == 1:
    iso = input("Which nickel? ")
else:
    iso = sys.argv[1]
comp=int(iso)+1

out = pd.DataFrame()

# read in the first LD file to keep energy and ld in out output dataframe for both positive parity (top of file) and negative parity (bottom of file)
datatop=pd.read_csv('ni'+str(iso)+'/nldcu'+str(comp)+'_1.tab',skiprows=3,sep=r'\s+',skipfooter=65,engine='python')
datatop.columns=['Energy','Temp','N_cumulative','ld 1','State den','J=01/2','J=03/2','J=05/2','J=07/2','J=09/2','J=11/2','J=13/2','J=15/2','J=17/2','J=19/2','J=21/2','J=23/2', 'J=25/2' , 'J=27/2','J=29/2','J=31/2','J=33/2' ,'J=35/2','J=37/2','J=39/2','J=41/2','J=43/2', 'J=45/2','J=47/2','J=49/2','J=51/2','J=53/2','J=55/2','J=57/2' ,'J=59/2']
datatop=datatop.drop(columns=['Temp','N_cumulative','State den','J=01/2','J=03/2','J=05/2','J=07/2','J=09/2','J=11/2','J=13/2','J=15/2','J=17/2','J=19/2','J=21/2','J=23/2', 'J=25/2' , 'J=27/2','J=29/2','J=31/2','J=33/2' ,'J=35/2','J=37/2','J=39/2','J=41/2','J=43/2', 'J=45/2','J=47/2','J=49/2','J=51/2','J=53/2','J=55/2','J=57/2' ,'J=59/2'])

databot=pd.read_csv('ni'+str(iso)+'/nldcu'+str(comp)+'_1.tab',skiprows=68,sep=r'\s+',skipfooter=0,engine='python')
databot.columns=['Energy','Temp','N_cumulative','ld 1','State den','J=01/2','J=03/2','J=05/2','J=07/2','J=09/2','J=11/2','J=13/2','J=15/2','J=17/2','J=19/2','J=21/2','J=23/2', 'J=25/2' , 'J=27/2','J=29/2','J=31/2','J=33/2' ,'J=35/2','J=37/2','J=39/2','J=41/2','J=43/2', 'J=45/2','J=47/2','J=49/2','J=51/2','J=53/2','J=55/2','J=57/2' ,'J=59/2']
databot=databot.drop(columns=['Temp','N_cumulative','State den','J=01/2','J=03/2','J=05/2','J=07/2','J=09/2','J=11/2','J=13/2','J=15/2','J=17/2','J=19/2','J=21/2','J=23/2', 'J=25/2' , 'J=27/2','J=29/2','J=31/2','J=33/2' ,'J=35/2','J=37/2','J=39/2','J=41/2','J=43/2', 'J=45/2','J=47/2','J=49/2','J=51/2','J=53/2','J=55/2','J=57/2' ,'J=59/2'])

#print(datatop,databot)	
ene=datatop['Energy']
ldtot=datatop['ld 1']+databot['ld 1']

out['Energy']=ene
out['ld 1']=ldtot
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

	if (i==4):
		addtop=pd.read_csv('ni'+str(iso)+'/nldcu'+str(comp)+'_'+str(i)+'.tab',skiprows=3,sep=r'\s+',skipfooter=60,engine='python')
	else:
		addtop=pd.read_csv('ni'+str(iso)+'/nldcu'+str(comp)+'_'+str(i)+'.tab',skiprows=3,sep=r'\s+',skipfooter=65,engine='python')
		
	addtop.columns=['Energy','Temp','N_cumulative','Total l.d.','State den','J=01/2','J=03/2','J=05/2','J=07/2','J=09/2','J=11/2','J=13/2','J=15/2','J=17/2','J=19/2','J=21/2','J=23/2', 'J=25/2' , 'J=27/2','J=29/2','J=31/2','J=33/2' ,'J=35/2','J=37/2','J=39/2','J=41/2','J=43/2', 'J=45/2','J=47/2','J=49/2','J=51/2','J=53/2','J=55/2','J=57/2' ,'J=59/2']

	if (i==4):
		addbot=pd.read_csv('ni'+str(iso)+'/nldcu'+str(comp)+'_'+str(i)+'.tab',skiprows=63,sep=r'\s+',skipfooter=0,engine='python')
	else:
		addbot=pd.read_csv('ni'+str(iso)+'/nldcu'+str(comp)+'_'+str(i)+'.tab',skiprows=68,sep=r'\s+',skipfooter=0,engine='python')

	addbot.columns=['Energy','Temp','N_cumulative','Total l.d.','State den','J=01/2','J=03/2','J=05/2','J=07/2','J=09/2','J=11/2','J=13/2','J=15/2','J=17/2','J=19/2','J=21/2','J=23/2', 'J=25/2' , 'J=27/2','J=29/2','J=31/2','J=33/2' ,'J=35/2','J=37/2','J=39/2','J=41/2','J=43/2', 'J=45/2','J=47/2','J=49/2','J=51/2','J=53/2','J=55/2','J=57/2' ,'J=59/2']
	
	ene_new=addtop['Energy']
	
	temp = pd.DataFrame()
	temp['Energy']= ene_new
	ldtot_new=addtop['Total l.d.']+addbot['Total l.d.']	
	temp['Total l.d.']=ldtot_new
	
	out['ld '+str(i)]=get_rho_shifted(temp,ptable,ctable) # for the rest I only keep the LD value in my dataframe
	
	plt.plot(ene_new,ldtot_new,label='ld '+str(i))

		
# find min and max
min_ld=out[['ld 1', 'ld 2', 'ld 3', 'ld 4', 'ld 5', 'ld 6']].min(axis = 1)
out['min']=min_ld
max_ld=out[['ld 1', 'ld 2', 'ld 3', 'ld 4', 'ld 5', 'ld 6']].max(axis = 1)
out['max']=max_ld

#print(out)
# save in new file only the columns I want
out[['Energy', 'min', 'max']].to_csv('ld_allparity_band_cu'+str(comp), sep = '\t', index=False, header=False)

# save all columns in one file
out[['Energy','ld 1', 'ld 2', 'ld 3', 'ld 4', 'ld 5', 'ld 6']].to_csv('ld_allparity_all_cu'+str(comp), sep = '\t', index=False, header=False)


plt.title(str(comp)+"Cu Level Density")
plt.xlabel("Energy (MeV)")
plt.ylabel("Total LD")	
plt.yscale('log')
plt.legend()
plt.show()




