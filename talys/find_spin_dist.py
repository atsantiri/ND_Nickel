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
    enum = input("Which state?")
    denom = input('Over which state?')
elif len(sys.argv) == 2:
    iso = sys.argv[1]
    enum = input("Which state?")
    denom = input('Over which state?')
else:
    iso = sys.argv[1]
    enum = sys.argv[2]
    denom = sys.argv[3]
  
comp=int(iso)+1


fig, ax = plt.subplot_mosaic([['A', 'C'],['B', 'C']],)
fig.set_size_inches((10,10),forward=False)
#print('Arguments ',len(sys.argv))
ave=[]
for i in range(1,7):

	data = pd.DataFrame()

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
		top=pd.read_csv('ni'+str(iso)+'/nldcu'+str(comp)+'_'+str(i)+'.tab',skiprows=3,sep=r'\s+',skipfooter=60,engine='python')
	else:
		top=pd.read_csv('ni'+str(iso)+'/nldcu'+str(comp)+'_'+str(i)+'.tab',skiprows=3,sep=r'\s+',skipfooter=65,engine='python')
		
	top.columns=['Energy','Temp','N_cumulative','Total l.d.','State den','J=01/2','J=03/2','J=05/2','J=07/2','J=09/2','J=11/2','J=13/2','J=15/2','J=17/2','J=19/2','J=21/2','J=23/2', 'J=25/2' , 'J=27/2','J=29/2','J=31/2','J=33/2' ,'J=35/2','J=37/2','J=39/2','J=41/2','J=43/2', 'J=45/2','J=47/2','J=49/2','J=51/2','J=53/2','J=55/2','J=57/2' ,'J=59/2']

	if (i==4):
		bot=pd.read_csv('ni'+str(iso)+'/nldcu'+str(comp)+'_'+str(i)+'.tab',skiprows=63,sep=r'\s+',skipfooter=0,engine='python')
	else:
		bot=pd.read_csv('ni'+str(iso)+'/nldcu'+str(comp)+'_'+str(i)+'.tab',skiprows=68,sep=r'\s+',skipfooter=0,engine='python')

	bot.columns=['Energy','Temp','N_cumulative','Total l.d.','State den','J=01/2','J=03/2','J=05/2','J=07/2','J=09/2','J=11/2','J=13/2','J=15/2','J=17/2','J=19/2','J=21/2','J=23/2', 'J=25/2' , 'J=27/2','J=29/2','J=31/2','J=33/2' ,'J=35/2','J=37/2','J=39/2','J=41/2','J=43/2', 'J=45/2','J=47/2','J=49/2','J=51/2','J=53/2','J=55/2','J=57/2' ,'J=59/2']
	
	ene=top['Energy']
	data['Energy']=ene

	temp = pd.DataFrame()
	temp['Energy']= ene
	ld=top['Total l.d.']+bot['Total l.d.']	
#	ld=top['State den']+bot['State den']	
	temp['Total l.d.']=ld
	data['Total l.d.']=get_rho_shifted(temp,ptable,ctable)

	ax['A'].plot(ene,data['Total l.d.'],label='total ld '+str(i))
	
	tot=np.zeros(len(ene))
	data['My tot']=tot
	for j in range(1,60,2):
		temp = pd.DataFrame()
		temp['Energy']= ene
		if j<10:
			num = "0"+str(j)+'/2'
		else:
			num = str(j)+'/2'
		ld=top['J='+num]+bot['J='+num]	
		temp['Total l.d.']=ld
		data['J='+num]=get_rho_shifted(temp,ptable,ctable) # for the rest I only keep the LD value in my dataframe
		data['My tot']+=data['J='+num]
		
#		plt.plot(ene,data['J='+num],label='ld '+num)

	diff=abs(data['Total l.d.']-data['My tot'])/data['Total l.d.']*100
	
	#Now to get each Jπ contribution for a few energies and compare
	energies = [5.0,5.5,6.,6.5,7.,7.5,8.,8.5,9.,9.5,10.,11.,12.,13.]
#	energies = data.Energy
	dist = pd.DataFrame()
	dist['Energy']=energies
	j1 = []
	j3 = []
	j5 = []
	j7 = []
	j9 = []
	j11 = []
	j13 = []
	for j, e in enumerate(energies):
#		print(float(data.loc[data['Energy']==e,"Energy"]))		
		if float(data.loc[data['Energy']==e,'Energy'])==e:
#				print(j,' Energy: ',float(data.loc[data['Energy']==e,'Energy'] ), ' Jpi 1/2 = ', float(data.loc[data['Energy']==e,'J=01/2']))
				j1.append(float(data.loc[data['Energy']==e,'J=01/2']))
				j3.append(float(data.loc[data['Energy']==e,'J=03/2']))
				j5.append(float(data.loc[data['Energy']==e,'J=05/2']))
				j7.append(float(data.loc[data['Energy']==e,'J=07/2']))
				j9.append(float(data.loc[data['Energy']==e,'J=09/2']))
				j11.append(float(data.loc[data['Energy']==e,'J=11/2']))
				j13.append(float(data.loc[data['Energy']==e,'J=13/2']))

				
	dist['J=1/2']=j1
	dist['J=3/2']=j3
	dist['J=5/2']=j5
	dist['J=7/2']=j7
	dist['J=9/2']=j9
	dist['J=11/2']=j11
	dist['J=13/2']=j13

									
	ax['A'].plot(ene,data['My tot'],label='sum of all J '+str(i))
	ax['B'].plot(ene,diff,label=str(i))
	
	ratio = dist['J='+str(enum)+'/2']/dist['J='+str(denom)+'/2']
	ax['C'].plot(energies,ratio,label = str(enum)+'/2 over '+str(denom)+'/2, '+str(i))
	ave.append(ratio)

print('Average: ',np.average(ave))
ax['C'].axhline(y = np.average(ave), color = 'r', linestyle = '--',label='average = '+str(np.round(np.average(ave),3)))

ax['B'].get_shared_x_axes().join(ax['A'], ax['B'])
ax['A'].set_xticklabels([])	
ax['A'].set_title(str(comp)+"Cu Level Density")
ax['B'].set_xlabel("Energy (MeV)")
ax['A'].set_ylabel("Total LD")	
ax['B'].set_ylabel("Percentage")	
ax['A'].set_yscale('log')
#ax[1].set_yscale('log')
ax['B'].set_title("Diff b/w total and sum of all Js")
ax['C'].set_title("Ratio of J="+str(enum)+'/2 over J='+str(denom)+'/2')
ax['C'].set_xlabel('Energy (MeV)')
ax['C'].set_ylabel('Ratio')
ax['A'].legend()
ax['B'].legend()
ax['C'].legend()
plt.savefig('ratio_'+str(comp)+'Cu_'+str(enum)+'_over_'+str(denom))
plt.show()






