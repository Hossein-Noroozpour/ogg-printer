#!/bin/sh
clear
clear

clean_objs() {
	for f in *.obj
	do
		if [ -f "$f" ]
		then
			rm "$f";
			echo "$f have bean deleted."
		fi
	done
}

clean_objs

clang++ -c -std=c++14 ogg-file.cpp -o ogg-file.obj
clang++ -std=c++14 main.cpp *.obj -lvorbisfile -o main

clean_objs

./main
