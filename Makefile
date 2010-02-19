all: jogo.exe 

obj.o:
	windres -O coff -o obj.o icon.rc

intro.o: intro.c
	gcc -c intro.c

jogo.o: jogo.c
	gcc -c jogo.c

almp3.o:
	gcc -c almp3.c

dct64_i386.o:
	gcc -c dct64_i386.c

interface.o:
	gcc -c interface.c

layer2.o:
	gcc -c layer2.c

layer3.o:
	gcc -c layer3.c

common.o:
	gcc -c common.c

decode_i386.o:
	gcc -c decode_i386.c

tabinit.o:
	gcc -c tabinit.c

jogo.exe: obj.o intro.o jogo.o almp3.o dct64_i386.o interface.o layer2.o layer3.o common.o decode_i386.o tabinit.o
	gcc -o jogo.exe obj.o intro.o jogo.o almp3.o dct64_i386.o interface.o layer2.o layer3.o common.o decode_i386.o tabinit.o -Wall -lalleg -O2

clean:
	del jogo.exe obj.o intro.o jogo.o almp3.o dct64_i386.o interface.o layer2.o layer3.o common.o decode_i386.o tabinit.o
