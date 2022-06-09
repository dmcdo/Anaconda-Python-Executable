
The Makefile will create the files `python.exe` and `pythonw.exe`.

These executables will launch the Anaconda version of Python with the environment activated.

In order for the program to work, there must be an environment variable called `ANACONDAPATH` whose value is the full path to your Anaconda installation. Ex: `C:\Users\username\miniconda3`

The program will fail if the path is invalid or if it is surrounded by quotation marks.
