ld=1
st=1
stm1=1
mid=_st
mid2=_M1st
while [ $ld -le 6 ]; do
	echo 'ld '$ld
	sed -i "s/ldmodel 1/ldmodel $ld/"  input.inp
	talys <input.inp> output.txt
	cp ld029061.tot ldcu61_$ld.tot
	cp nld029061.tab nldcu61_$ld.tab
	cp output.txt out_ld$ld$mid$st$mid2$stm1
	sed -i "s/ldmodel $ld/ldmodel 1/"  input.inp
	let ld=ld+1
done
ld=1
while [ $st -le 9 ]; do
	echo 'strength '$st
	sed -i "s/strength 1/strength $st/"  input.inp
	talys <input.inp> output.txt
	cp psf029061.E1 E1cu61_$st
	cp output.txt out_ld$ld$mid$st$mid2$stm1
	sed -i "s/strength $st/strength 1/"  input.inp
	let st=st+1
done
st=1
while [ $stm1 -le 3 ]; do
	echo 'strength M1 '$stm1
	sed -i "s/strengthM1 1/strengthM1 $stm1/"  input.inp
	talys <input.inp> output.txt
	cp psf029061.M1 M1cu61_$stm1
	cp output.txt out_ld$ld$mid$st$mid2$stm1
	sed -i "s/strengthM1 $stm1/strengthM1 1/"  input.inp
	let stm1=stm1+1
done
echo 'done. out'
