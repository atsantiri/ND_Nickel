import numpy as np
import matplotlib.pyplot as plt

m = np.loadtxt('ni60_Ex_7.13_0.txt', skiprows=1,usecols=range(21))
ex=[]
for i in range(len(m)):
	ex_i=0
	for j in range(1,len(m[i])):
		ex_i+=m[i][j]
	ex.append(ex_i)
plt.hist(ex,label='7.13')

m2 = np.loadtxt('ni60_Ex_7.12_0.txt', skiprows=1,usecols=range(21))
ex2=[]
for i in range(len(m2)):
	ex_i=0
	for j in range(1,len(m[i])):
		ex_i+=m[i][j]
	ex2.append(ex_i)


m3 = np.loadtxt('ni60_Ex_7.14_0.txt', skiprows=1,usecols=range(21))
ex3=[]
for i in range(len(m3)):
	ex_i=0
	for j in range(1,len(m[i])):
		ex_i+=m[i][j]
	ex3.append(ex_i)
plt.hist(ex3,label='7.14')
plt.hist(ex2,label='7.12')
plt.legend()
plt.show()
