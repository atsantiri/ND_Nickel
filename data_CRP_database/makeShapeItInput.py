# 
import numpy as np
import sys

fOut = None

# Input file
if len(sys.argv) == 2:
    fIn = sys.argv[1]
elif len(sys.argv)==3:
    fIn = sys.argv[1]
    fOut = sys.argv[2]
else:
    print("Execute as: makeShapeItInput.py CRPfilename")
    print("Execute as: makeShapeItInput.py inputFilename outputFilename")

if not fIn.endswith(".dat"):
    print("Input file must be a .dat file from the CRP database.")
    sys.exit(1)
    
print(f"Reading file {fIn}")
E, f1, dE, df1 = np.loadtxt(fIn, comments='#').T

# ensure energies are in order
order = np.argsort(E)

E = E[order]
f1 = f1[order]
dE = dE[order]
df1 = df1[order]

# Output file
if (fOut == None):
    s = fIn.split("_")
    fOut = f"{int(s[3])}Cu_{s[5][:6]}.txt"

print(f'Making file {fOut}')


with open(f'../shapeIt_files/dataCRP/{fOut}', "w") as f:
    for e, f1, de, df1 in zip(E, f1, dE, df1):
        f.write(f"{e*1e3:.3e}\t{(f1+df1):.3e}\t{(f1-df1):.3e}\n")