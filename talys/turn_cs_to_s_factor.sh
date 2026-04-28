echo 'Imma take some TALYS cross sections and run some Python to calculate s-factors.'
echo 'imma start with the total (binary) cross sections file'
python3 cs_sf_total.py
echo 'moving on to the 55Co CN from p,a'
python3 cs_to_sf.py
echo 'Then the 58Ni CN for pp'
sed -i s/rp027055/rp028058/ cs_to_sf.py
python3 cs_to_sf.py
echo 'now 58Cu for pn'
sed -i s/rp028058/rp029058/ cs_to_sf.py
python3 cs_to_sf.py
echo 'and finally our very own and loved 59Cu fpr p,gammaaaaaaa'
sed -i s/rp029058/rp029059/ cs_to_sf.py
python3 cs_to_sf.py
sed -i s/rp029059/rp027055/ cs_to_sf.py
#root -l -q decomposition.C
root -l -q decomposition_s_factor.C
echo 'done. out'
