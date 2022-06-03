CC = gcc

all:
	$(CC) python.c -o python.exe
	$(CC) python.c -D PYTHONW -o pythonw.exe