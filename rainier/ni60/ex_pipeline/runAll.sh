# script to modify settings.h and make_geant_input.py and run RAINIER for a few Ex for s and p wave p capture
# to set RAINIER_PATH, execute the following in terminal, or save in .bashrc/profile
# export RAINIER_PATH=/path/to/RAINIER
# define function
RunRainier () {
	cp ${RAINIER_PATH}/RAINIER.C RAINIER_copy.C
	root -l -q RAINIER_copy.C++ > out.txt
}

declare -a ex=("7.10" "7.15" "7.20")
declare -a j=(0.5 0.5 1.5)
declare -a pi=(1 0 0)
n=0
mid=_
end=.root

for i in "${ex[@]}"
do
	sed -i '215s/mple/'"$i"'/' settings.h
	
	for k in "${!j[@]}";
	do
		echo "==========================================="	
		echo "ex = $i, $k: ${j[$k]} ${pi[$k]}"	
		
		sed -i '216s/mple/'"${j[$k]}"'/' settings.h
		sed -i '217s/mple/'"${pi[$k]}"'/' settings.h
		RunRainier
		
		cp Run0001.root run_$i$mid$k$end
		
		num=$((n*3+k))
		echo $num
		sed -i '41s/mple/'"$num"'/' make_geant_input.py
		python3 make_geant_input.py 60 7.14 100000
		
		sed -i '41s/'"$num"'/mple/' make_geant_input.py
		sed -i '216s/'"${j[$k]}"'/mple/' settings.h
		sed -i '217s/'"${pi[$k]}"'/mple/' settings.h
				
	done

	let n=n+1
	sed -i '215s/'"$i"'/mple/' settings.h
done
