OBJS	= main.o affichage.o fichiers.o fir.o iir.o integration.o lecture.o mesure.o
SOURCE	= main.c affichage.c fichiers.c fir.c iir.c integration.c lecture.c mesure.c
HEADER	= ftd2xx.h
OUT	= projet_c
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = -L./ -lftd2xx -lm

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.c
	$(CC) $(FLAGS) main.c -std=c99

affichage.o: affichage.c
	$(CC) $(FLAGS) affichage.c -std=c99

fichiers.o: fichiers.c
	$(CC) $(FLAGS) fichiers.c -std=c99

fir.o: fir.c
	$(CC) $(FLAGS) fir.c -std=c99

iir.o: iir.c
	$(CC) $(FLAGS) iir.c -std=c99

integration.o: integration.c
	$(CC) $(FLAGS) integration.c -std=c99

lecture.o: lecture.c
	$(CC) $(FLAGS) lecture.c -std=c99

mesure.o: mesure.c
	$(CC) $(FLAGS) mesure.c -std=c99


clean:
	rm -f $(OBJS) $(OUT)
