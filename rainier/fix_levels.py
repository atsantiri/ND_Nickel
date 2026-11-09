import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import sys

# question about isotope to set folder name
if len(sys.argv) == 1:
    iso = input("Which nickel? ")
else:
    iso = sys.argv[1]
  
comp=int(iso)+1


file = 'ni'+str(iso)+'/mylevels_z029_raw.dat'
content = []
new_content = []

test_lvl = True
t='1.000E-15'

with open (file,'r') as f:
	for line in f:
		content.append(line)
		data = line.split(' ')
f.close()		
prl = 1#previous level

#Find number of known levels
data=content[0].split(' ')
known_lvl = int(data[14])
print('# of known levels for ',data[1],' = ',known_lvl)
print(' Level   E   J   pi   half-life' )
# append first two lines as is	
new_content.append(content[0].split(' '))
new_content.append(content[1].split(' '))		

for i in range(2,len(content)):
	data=content[i].split(' ')
	
	if data[2].isdigit() or data[1].isdigit() or data[0].isdigit(): # if this is a level line
		# find level number index
		if data[2].isdigit(): # lvl 1-9
			l = 2
		elif data[1].isdigit(): # lvl 10-99
			l = 1
		elif data[0].isdigit(): # lvl 100-999
			l = 0
		else:
			print("I'm doing something wrong. I thought this is a level line, but it isn't.")	

		# check if previous level knows how to decay
		if (i-1== prl) and test_lvl and (int(data[l]) <= known_lvl):
			lvl = content[i-1].split(' ')
			print('Level ',lvl[l]," doesn't know how to decay")
		prl = i # update previous level index	
		
		# find energy value, index
		if data[l+1]!='':
			ene = l+1
#			print('Level ',data[l], ' ene ', data[ene], ene)
		elif data[l+2]!='':
			ene = l+2
#			print('Level ',data[l], ' ene ', data[ene], ene)
		elif data[l+3]!='':
			ene = l+3
#			print('Level ',data[l], ' ene ', data[ene], ene)
		else:
			print("Something is wrong. Can't find energy for level ", data[l])
			break

		# find J value, index
		if data[ene+1]!='':
			j = ene+1
#			print('Level ',data[l], ' J ', data[j], j)
		elif data[ene+2]!='':
			j = ene+2
#			print('Level ',data[l], ' J ', data[j], j)		
		elif data[ene+3]!='':
			j = ene+3
#			print('Level ',data[l], ' J ', data[j], j)		
		else:
			print("Something is wrong. Can't find J for level ", data[l])
			break

		# find pi value, index
		if data[j+1]!='':
			pi = j+1
#			print('Level ',data[l], ' pi ', data[pi], pi)
		elif data[j+2]!='':
			pi = j+2
#			print('Level ',data[l], ' pi ', data[pi], pi)
		elif data[j+3]!='':
			pi = j+3
#			print('Level ',data[l], ' pi ', data[pi], pi)
		else:
			print("Something is wrong. Can't find pi for level ", data[l])
			break
		
		# check if it has a half-life, if not add it
		if data[pi+1]!='':
			tau = pi+1
#			print("Found hl for level ",data[l],data[tau])		
		elif data[pi+2]!='':
			tau = pi+2
#			print("Found hl for level ",data[l],data[tau])			
		elif data[pi+3]!='':
			tau = pi+3
#			print("Found hl for level ",data[l],data[tau])		
		else:
#			print("didn't find half'life for level ",data[l])
			data[pi+2]=t
			tau = pi+2
			del data[(pi+3):(pi+12)] # remove spaces ahead to maintain file format
			
		print(data[l],data[ene],data[j],data[pi],data[tau],prl)
#		print(data)
		new_content.append(data)

	else: # this is a gamma line
		new_content.append(data)	

with open('ni'+str(iso)+'/mylevels_z029_fixed.dat','w') as new_file:
	for a in new_content: 
		a = ' '.join(str(item) for item in a)
#		print(a)
		new_file.write(str(a))
new_file.close()
