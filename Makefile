# Compiler und Flags
CC = gcc
CFLAGS = -std=c11 -pedantic -Wall -Werror -D_XOPEN_SOURCE=700

# Standardziel
all: clash

# Hauptziel: clash aus Objektdateien bauen
clash: clash.o plist.o
	$(CC) $(CFLAGS) -o clash clash.o plist.o

# clash.o aus clash.c erzeugen
clash.o: clash.c plist.h
	$(CC) $(CFLAGS) -c clash.c -o clash.o

# plist.o aus plist.c erzeugen
plist.o: plist.c plist.h
	$(CC) $(CFLAGS) -c plist.c -o plist.o

# Aufräumen: alle erzeugten Dateien entfernen
clean:
	rm -f clash *.o
