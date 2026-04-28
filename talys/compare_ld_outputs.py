import numpy as np
import matplotlib.pyplot as plt
import sys
import pandas as pd

# question about isotope to set folder name
if len(sys.argv) == 1:
    iso = input("Which nickel? ")
else:
    iso = sys.argv[1]
comp=int(iso)+1

fig, axs = plt.subplots(2,3,sharex=True,sharey=True)

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


axs[0,0].plot(ene,ldtot,label='ld 1 per parity')


for i in range(2,7):

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

#	temp = pd.DataFrame()
#	temp['E1']=addtop['Energy']
#	temp["top"]=addtop['Total l.d.']
#	temp['E2']=addbot['Energy']
#	temp["bot"]=addbot['Total l.d.']
#	print(i)
#	print(temp)

	ene_new=addtop['Energy']
	ldtot_new=addtop['Total l.d.']+addbot['Total l.d.']
	if(i<=3):
		axs[0,i-1].plot(ene_new,ldtot_new,label='ld '+str(i)+' per parity')


	elif(i>3):
		axs[1,i-4].plot(ene_new,ldtot_new,label='ld '+str(i)+' per parity')


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

axs[0,0].plot(ene,ldtot,label='ld 1')
axs[0,0].set_ylim([1,1e6])
axs[0,0].set_xlim([0,15])
axs[0,0].set_yscale('log')
axs[0,0].legend()

for i in range(2,7):
	add=pd.read_csv('ni'+str(iso)+'/ldcu'+str(comp)+'_'+str(i)+'.tot',skiprows=12,sep=r'\s+',skipfooter=1,engine='python')
	if (len(add.columns) == 6):
		add.columns=['Energy','Level','N_cumulative','Total l.d.','a','Sigma']
	elif (len(add.columns) == 4):
		add.columns=['Energy','Level','N_cumulative','Total l.d.']
	else:
		print('File '+'ldcu'+str(comp)+'_'+str(i)+'.tot has weird number of columns.')
		break	
	ene_new=add['Energy']
	ldtot_new=add['Total l.d.']
	data['ld '+str(i)]=ldtot_new # for the rest I only keep the LD value in my dataframe
	if(i<=3):
		axs[0,i-1].plot(ene_new,ldtot_new,label='ld '+str(i))
		axs[0,i-1].set_ylim([1,1e6])
		axs[0,i-1].set_xlim([0,15])
		axs[0,i-1].set_yscale('log')
		axs[0,i-1].legend()
	else:
		axs[1,i-4].plot(ene_new,ldtot_new,label='ld '+str(i))
		axs[1,i-4].set_ylim([1,1e6])
		axs[1,i-4].set_xlim([0,15])
		axs[1,i-4].set_yscale('log')
		axs[1,i-4].legend()
#print(data)

fig.suptitle(str(comp)+'Cu')
plt.savefig(str(comp)+'Cu_ld_comparison.png')
plt.show()




