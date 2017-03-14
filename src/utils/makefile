# makefile for utils with TANK ARENA

GCC = gcc -O3 -Wall -Wunused -m486

passwd: /tank/utils/passwd.exe
	
draw: /tank/utils/draw.exe
	
dat: /tank/utils/dat.exe
	
bdraw: /tank/utils/bdraw.exe
	
xto1: /tank/utils/xto1.exe
	
1tox: /tank/utils/1tox.exe
	
link: /tank/utils/link.exe
	
bmp: /tank/utils/bmp2pic.exe /tank/utils/pic2bmp.exe

clean:
	del *.o

all: dat bdraw xto1 1tox link bmp draw
		
/tank/utils/draw.exe: draw.o 5x5font.o
	$(CC) -o $@ draw.o 5x5font.o -lemu

/tank/utils/%.exe: %.c
	$(GCC) -o $@ $< -lemu

%.o: %.cc
	$(GCC) -o $@ -c $<

%.o: %.c
	$(GCC) -o $@ -c $<

%.o: %.s
	$(GCC) -o $@ -c $<

