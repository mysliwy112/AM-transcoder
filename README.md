### AM-transcoder
Is a CLI program named Anndrzem used for decoding and encoding graphical files (ann, img) from games developed in Piklib and Bloomoo - engines which were created by Aidem Media.

### Release
[Get it here](https://github.com/mysliwy112/ANN-decoder/releases)

### Instruction
Files can be decoded and encoded by dragging and dropping them on decoder (Anndrzem.exe).

###### Supported formats:
* .img => .png
* .ann => multiple .png and .mann if specified.
* .png => .img
* .mann and aditional .png => .ann


To get around complexity and additional data stored by ann, special MetaANN file for encoding ann was created. It should be readable and editable config file. You can create it by hand, or just by extracting ann file with -m flag. To get additional information and specification of mann file look into [MANN_DOC.md](MANN_DOC.md).

Because decoder is CLI application it can take additional arguments.
(To open CMD in folder, one should, while holding Shift, click on empty space in folder and choose option "Open command window here") later after typing the name of the program, it is possible to add parameters.

ex. `anndrzem -h` shows instruction page.

After arguments it is possible to add names of files to extract ex.:

```anndrzem -n -v  ".\Przygoda\Cat\buty.ann"```

Extracts images from buty, with additional debug info.

#Arguments:

Full names are specified only for better rememberance, please don't use them as flag, because that type of usage is not implemented.

-h(elp) Shows help message.
	
-d(irectory) Sets output directory, takes path as argument:
`anndrzem -f ./out/ kretes.ann`
Will create folder out in anndrzem directory and decode kretes.ann into it.
	
-f(ile's) Sets output to input file directory:
`anndrzem -f C:/kretes.ann`
Will decode to directory where file kretes.ann is located.

-c(reate) Creates directory named by input file's name:
`anndrzem -c kretes.ann`
Will create additional directory named kretes/ and decode there. Can be used with previos flags.

-m(eta) Creates MetaANN file and additional images, used for encoding anns.
`anndrzem -m kretes.ann`
Will create kretes.mann and additional png files representing images from ann. For mann documentation look into MANN_DOC.md.

-s(equence) Creates event sequence, can take one argument - event name:
Extracts specific animation from ann file, name of event to extract can be specified on runtime, after program list events' names. You can type : to get event by id.
'anndrzem -s GADA kretes.ann'
Will extract frames to files prefixed with GADA, representing talking Moles The Mole.

-v(erbose) Generates additional console output, used mainly for debbuging.

-p(ad) Pads numbers with zeros to specified number of digits can take one argument - maximal number of digits:
'anndrzem -p 3 kretes.ann'
Will creates files kretes_00001, kretes_00002 etc. When parameter is not specified program will take number of images to extract as maximal padding.
'anndrzem kretes.ann -p'
Will create kretes_01, kretes_02, ... , kretes_13.

-a(lign) Aligns images' sizes.
Images extracted with this option, are going to have additional transparent pixels, so they could be positioned in animation by left right corner.

-o(ffset) Adds transparent pixels to all sides of image, takes one argument - length of plane to add, when not specified default value is 10.

-i(gnore) Ignores errors while processing files.

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
Anndrzem is only compilable to 32-bit binaries, because it still uses piklib library.
I'll add dependencies later.
