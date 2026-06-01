#!/bin/bash
Js=(0.5 0.5 1.5 1.5 2.5 2.5 3.5)
Ps=(0 1 0 1 0 1 0)

for i in "${!Js[@]}"; do
	j=${Js[i]}
	pi=${Ps[i]}
	if [ "$pi" -eq 0 ]; then
		ext="${j}m"
	else
		ext="${j}p"
	fi
	echo $ext
	# sed -i "s|const double g_adSpI.*|const double g_adSpI[] = {$j};|" cu63/settings.h
	sed -i "s|const double g_dSpI =*|const double g_dSpI = $j;|" cu63/settings.h
	sed -i "s|const double g_dParI =*|const double g_dParI =$pi;|" cu63/settings.h
	# sed -i "s|const double g_anParI.*|const double g_anParI[] = {$pi};|" cu63/settings.h
	awk 'NR==210' cu63/settings.h
	awk 'NR==212' cu63/settings.h
	cd cu63
	./runRAINIER.sh
	cd ..
	python3 make_geant_input.py 10 $ext
	./runGeantInFishtank.sh $ext
done
