### AM-transcoder
Is a CLI program named Anndrzem used for decoding and encoding graphical files (ann, img) from games developed in Piklib and Bloomoo - engines which were created by Aidem Media.

### Release
[Get it here](https://github.com/mysliwy112/ANN-decoder/releases)

### Instruction
Files can be decoded and encoded by dragging and dropping them on decoder (Anndrzem.exe).
ex. drag zegar.mimg on anndrzem.exe to convert it to zegar.img
or use console `anndrzem kretes.ann -m` to extract images and kretes.mann.

###### Supported formats:
* .img => .png
* .ann => multiple .png and .(j)mann if specified.
* .png/.mimg => .img
* .(j)mann and aditional .png => .ann


To get around complexity and additional data stored by ann, special MetaANN file for encoding ann was created. It should be readable and editable config file. You can create it by hand, or just by extracting ann file with -m flag. To get additional information and specification of mann file look into [MANN_DOC.md](MANN_DOC.md). Now anndrzem supports saving mann as json file.

Because decoder is CLI application it can take additional arguments.
(To open CMD in folder, one should, while holding Shift, click on empty space in folder and choose option "Open command window here") later after typing the name of the program, it is possible to add parameters.

ex. `anndrzem -h` shows instruction page.

After arguments it is possible to add names of files to extract ex.:

```anndrzem -n -v  ".\Przygoda\Cat\buty.ann"```

Extracts images from buty, with additional debug info.

#### Arguments:

Use - for short names and -- for full names. Parameters to specific flags can be added after "=" character.

-h(elp) Shows help message.
	
-d(irectory) Sets output directory, takes path as argument:
`anndrzem -f=./out/ kretes.ann`
Will create folder out in anndrzem directory and decode kretes.ann into it.
	
-f(ile) Sets output to input file directory:
`anndrzem -f C:/kretes.ann`
Will decode to directory where file kretes.ann is located.

-n(ame-off) Disable creation of directory named by input file's name:
`anndrzem kretes.ann`
Will create additional directory named kretes/ and decode there. While with -n it will not create kretes/. Can be used with previos flags.

-m(etafile) Creates MetaANN file and additional images, used for encoding anns.
`anndrzem -m kretes.ann`
Will create kretes.mann and additional png files representing images from ann. For mann documentation look into MANN_DOC.md.

-j(son) Creates MetaANN file in json format.
`anndrzem -j kretes.ann`
Works the same as -m but uses json syntax. Refer to MANN_DOC.md for semi informations.

-s(equence) Creates event sequence, can take one argument - event name:
Extracts specific animation from ann file, name of event to extract can be specified on runtime, after program list events' names. You can type : to get event by id.
'anndrzem -s=GADA kretes.ann'
Will extract frames to files prefixed with GADA, representing talking Moles The Mole.

-v(erbose) Generates additional console output, used mainly for debbuging.

-p(ad) Pads numbers with zeros to specified number of digits can take one argument - maximal number of digits:
'anndrzem -p=3 kretes.ann'
Will creates files kretes_00001, kretes_00002 etc. When parameter is not specified program will take number of images to extract as maximal padding.
'anndrzem kretes.ann -p'
Will create kretes_01, kretes_02, ... , kretes_13.

-a(lign) Aligns images' sizes.
Images extracted with this option, are going to have additional transparent pixels, so they could be positioned in animation by left right corner.

-o(ffset) Adds transparent pixels to all sides of image, takes one argument - length of plane to add, when not specified default value is 10.

-i(gnore) Ignores errors while processing files.

--images-off Don't extract images.
When used, image files like these with ".png" extension won't be created. Used mainly when you want to get information about ann. When used while encoding ann/img only headers will be created.

--full Fully transcripts ann to mann.
Writes all possible data to metafile. Metafile created in that way can be hard to read. But sometimes data used with this flag would me necessary for proper encoding.

--merge Merges to destination file (Experimental)
Merges decoded file to destination metafile. Mainly used with --full flag. It won't work sometimes, can break meta file. Use at your own risk, do backups.


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
