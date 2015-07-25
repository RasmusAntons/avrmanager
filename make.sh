#!/bin/bash
if [ -f plugin.o ] || [ -f avrdude.o ] || [ -f strlist.o ]
	then
		rm *.o
fi

gcc -c plugin.c avrdude.c strlist.c -fPIC `pkg-config --cflags geany`

if [ -f plugin.o ] && [ -f avrdude.o ] && [ -f strlist.o ]
	then
		gcc plugin.o avrdude.o strlist.o -o plugin.so -shared `pkg-config --libs geany`
		echo "success, copying to /usr/lib/x86_64-linux-gnu/geany with the power of root"
		sudo cp plugin.so /usr/lib/x86_64-linux-gnu/geany/avr.so
	else
		echo "shit."
fi
