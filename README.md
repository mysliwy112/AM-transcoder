### ANN-decoder
Is a CLI program for decoding .ann files from Piklib and Bloomoo games developed by Aidem Media

### Release
[Get it here](https://github.com/mysliwy112/ANN-decoder/releases)

### Instruction
Files can be decoded by dragging and dropping them on decoder (Anndrzem.exe).
Because decoder is CLI application it can take additional arguments.
(To open CMD in folder, one should, while holding Shift, click on empty space in folder and choose option "Open command window here") later after typing the name of the program, it is possible to add parameters.

ex. ```anndrzem -h``` shows instruction page.

#Arguments:
    -h Shows help message.
    -f Sets output directory.
    -l Some random console output.
    -o Don't create new directory for images.
    -s Creates event sequence (type \"id:\" to name by id)(events names are going to be listed and can be chosen from on runtime).
    -e Extract raw decompressed bitmap in RGBA(8,8,8,8).
    -n Align image sizes (useful for creating animations).
	
After arguments it is possible to add names of files to extract ex.:

```anndrzem -n -s PLAY ".\Przygoda\Cat\buty.ann```

Extracts to folder buty images that formed animation PLAY in file buty.ann, allowing to recreate animation in normal file format.

### Warning
I've stolen this section from sister project [dePIKczer](https://github.com/Dove6/dePIKczer/raw/master/Release/dePIKczer.exe) for decoding .img files.

Using this program or making changes to any of Aidem Media's products is contrary to licence agreement according to point number 2:
>Nabywca nie ma prawa do wynajmowania, dzierżawy, wypożyczania i rozpowszechniania programu, a także do przenoszenia licencji na osoby trzecie bez pisemnej zgody Producenta. Niedozwolone jest kopiowanie i modyfikacja fragmentów lub całości programu jak i wykorzystanie go w publikacjach i innych programach.

Mentioned in last sentence:
Copying or modifying of elements or program as a whole and using it in publications and other programs is prohibited.

You're accepting the consequences writen down in point number seven:
>W przypadku naruszenia przez nabywcę umowy licencyjnej Producent będzie dochodził swoich praw i roszczeń, z wypowiedzeniem umowy włącznie. W razie sporów sądowych właściwy będzie Sąd siedziby Producenta.

In case of breaching a contract, producer is going to exercise the right and claims with termination of the agreement. In the events of 
litigation processes will take place in Producer's local Court.

You've been warned.

### Compilation
Program was created in codeblocks, so it uses it's project system.

To decode infinitely big files, define BIGFILE at compilation.
