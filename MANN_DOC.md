MetaANN(mann) and MetaIMG(mimg) files are configuration files used to compile ann or img files for Piklib/Bloomoo engine. They can be opened by notepad or any other simmilar text editor.

Metafile structure is simple and hierarchical.
MANN always starts with ANN - magic world followed by global ann options, events and frames, ended by images descriptions.

Here are descriptions of all MANN options. Some of which shouldn't be modified by user, MIMG uses same options as MANN in Image section, but should have IMG instead of ANN in header.

## HEADER
name|argument type|default value|description
----|-------------|-------------|-----------
filename | string | name of file | name of ann that will be created when passed to anndrzem (without extension)
author | string | "anon" | author of ann
transparency | int 0 - 255 | 255 | transparency of ann, generaly don't use, it's here for compatibility 
bpp | uint | 16 | bits per pixel, generaly don't use, it's here for compatybility
event | string | - | name of event, starts event section

## EVENT
name|argument type|default value|description
----|-------------|-------------|-----------
transparency | int 0-255 | 255 | transparency of event, generaly don't use, it's here for compatybility
loop | int 0-number of frames | 0 | number of frames to loop
frame | string - filename | - | image file of frame, starts frame arguments

## FRAME
name|argument type|default value|description
----|-------------|-------------|-----------
position_x | int | 0 | position of frame in x axis 
position_y | int | 0 | position of frame in y axis 
sfx_seed | int | - | number used for sfx deduction, generaly don't use, it's here for compatybility
sfx | string - filenames separated by ';' | - | paths to sound files that can play while playing this frame
check | string - 4 characters | \`}U. | just ignore it's here for compatybility
name | string | "none" | name of frame

## IMAGE
name|argument type|default value|description
----|-------------|-------------|-----------
image | string - filename | - | start of image section, filename of image 
position_x | int | 0 | position of image in x axis 
position_y | int | 0 | position of image in y axis 
compression | enum | 3 | compression type of image, just for experimentation 
width | int | input image width | width of image, just for experimentation 
height | int | input image height | height of image, just for experimentation 
bpp | int | bpp of ann, 16 | bits per pixel, generaly don't use
name | string | - | name of image

### Example
```
ANN

name=cpl_smok
author=Dove

Event=WCHODZI

	Frame=cpl_smok_0.png
		position_x=211
		position_y=5

	Frame=cpl_smok_0.png
		position_x=180
		position_y=4

	Frame=cpl_smok_0.png
		position_x=153
		position_y=3

	Frame=cpl_smok_1.png
		position_x=129
		position_y=3
		sfx=wavs/wchodzi1.wav;wavs/wchodzi2.wav

	Frame=cpl_smok_1.png
		position_x=108
		position_y=2

	Frame=cpl_smok_1.png
		position_x=89
		position_y=2

image=cpl_smok_0.png
	position_x=593
	position_y=106
	
image=cpl_smok_1.png
	position_x=593
	position_y=106
```
