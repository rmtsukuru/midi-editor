midi.exe: src/*.c src/*.h main.c
	gcc -std=c99 src/*.c main.c -o midi.exe
