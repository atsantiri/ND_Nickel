import pandas as pd
import matplotlib.pyplot as plt
import re
import warnings

warnings.simplefilter(action='ignore', category=FutureWarning)

import matplotlib.font_manager as fm
georgia_font_path = '/usr/share/fonts/truetype/msttcorefonts/Georgia.ttf' 
fm.fontManager.addfont(georgia_font_path)
plt.rcParams['font.family'] = 'Georgia' 
plt.rcParams['font.size'] = 20  
plt.rcParams['axes.labelsize'] = 20  
plt.rcParams['xtick.labelsize'] = 18 
plt.rcParams['ytick.labelsize'] = 18 
plt.rcParams['legend.fontsize'] = 20 
plt.rcParams['axes.titlesize'] = 20
plt.rcParams['mathtext.fontset'] = 'dejavuserif' 


cols=['Ene', 'J', 'P', 'pop', 'Ppop', 'tot']
df = pd.DataFrame(columns=cols)

fig, axs = plt.subplots(1, figsize=(10,8))

pattern = r"Compound nucleus decay of J= ([\d.]+) P= ?(-?1) Pop= ([\d.E(-?)]+) to bins of Z= 29 N= 34 \( 63Cu\), P= ?(-?1) via gamma    emission"

f=f'ni62/out_outdecay'
# f=f'test'
print('='*90)
previous_tot=0
with open(f, 'r') as file:
    for line in file:
        match = re.search(pattern, line)
        if match:
            j = float(match.group(1))
            p = int(match.group(2))
            pop = float(match.group(3))
            p2 = int(match.group(4))
            print(line)
            print(f"J={j}, P={p}, Pop={pop}, P2={p2}")
            next(file)
            total_match = re.search(r"Total:\s+([\d.E+-]+)", next(file))
            if total_match:
                # print(float(total_match.group(1)))
                this_tot = float(total_match.group(1))
                tot_subbed = this_tot - previous_tot
                print(f'previous = {previous_tot}, this tot = {this_tot}, tot_subbed = {tot_subbed}')
                if p2 == 1:
                    previous_tot = this_tot
                # df = df.append({'J': j, 'P': p, 'pop': pop, 'Ppop': p2, 'tot': float(total_match.group(1))}, ignore_index=True)
                # df = df.append({'J': j, 'P': p, 'pop': pop, 'Ppop': p2, 'tot': tot_subbed}, ignore_index=True)
                new_row = {'J': j, 'P': p, 'pop': pop, 'Ppop': p2, 'tot': tot_subbed}
                df = pd.concat([df, pd.DataFrame([new_row])], ignore_index=True)

df = df.groupby(['J', 'P', 'pop','tot']).agg({'Ppop': 'sum'}).reset_index()

df = df.sort_values(by=['J', 'P']).reset_index(drop=True)

df['J'] = df['J'].astype(float)
df['JP'] = df['J'].astype(str) + df['P'].apply(lambda x: '+' if x == 1 else '-')

print(df) 
unique_JP = ['0.5-', '0.5+', '1.5-', '1.5+', '2.5-', '2.5+', '3.5-', '3.5+', '4.5-', '4.5+', '5.5-', '5.5+', '6.5-', '6.5+']

df_filtered = df.set_index('JP').loc[unique_JP].reset_index()

df_filtered.loc[:, 'norm_tot'] = df_filtered['tot'] / df_filtered['tot'].sum()
df_filtered.loc[:, 'norm_pop'] = df_filtered['pop'] / df_filtered['pop'].sum()
print('='*90)
print(df_filtered)
# axs.plot(df_filtered['JP'], df_filtered['norm_tot'], label=f'norm_tot')
# axs.plot(df_filtered['JP'], df_filtered['norm_pop'], label=f'norm_pop')
axs.bar(df_filtered['JP'], df_filtered['norm_tot'],color='gray')


axs.set_ylabel('Normalized Probability')
# axs.legend(fontsize=15)

axs.tick_params(axis='x', rotation=45)
axs.set_xlabel(r'J$\pi$')

plt.tight_layout()
plt.savefig(f'jp_dist.png')
plt.show()

print('='*90)
js=df_filtered['J'].to_list()
pis_raw=df_filtered['P'].to_list()
def filter(i):
    if i<0:
        return 0
    else:
        return 1
pis=[filter(elem) for elem in pis_raw]
brs_raw=df_filtered['norm_tot'].to_list()
brs=[f'{elem:.6f}' for elem in brs_raw]

print(f"SpI = {', '.join(str(elem) for elem in js)}")
print(f"ParI = {', '.join(str(elem) for elem in pis)}")
print(f"BRI = {', '.join(str(elem) for elem in brs)}")
print(f'sum = {sum([float(n) for n in brs]):.6f}')