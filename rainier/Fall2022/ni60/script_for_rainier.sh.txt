declare -a ene=(6740 7140 7950 8540 9330 9710 10290)
for i in "${ene[@]}"
do
	ex=$(echo "$i*0.001" |bc)
	echo '==========================================' 
	echo 'Energy ' $ex
	echo '==========================================' 
	sed -i 's/mple/'"$ex"'/' settings.h
	./runRAINIER.sh
	python3 make_geant_input.py 60 $ex 10
	sed -i 's/'"$ex"'/mple/' settings.h
done	
