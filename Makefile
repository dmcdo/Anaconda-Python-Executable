CC = g++

all:
	$(CC) python.cpp -o python.exe
	$(CC) python.cpp -D PYTHONW -o pythonw.exe

clean:
	rm *.exe
