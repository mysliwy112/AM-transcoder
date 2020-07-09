MetaANN file is configuration file used to compile ann files for Piklib/Bloomoo engine. It can be opened by notepad or any other simmilar text editor.

MANN structure is simple and hierarchical.
MANN always starts with ANN - magic world followed by global ann options, events and frames, ended by images descriptions.

Here are descriptions of all mann options. Some of which shouldn't be modified by user.

## HEADER
name|argument type|default value|description
----|-------------|-------------|-----------
name | string | "fire" | name of ann 
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
position_x | int | 0 | position in x axis of frame
position_y | int | 0 | position in y axis of frame
sfx | string - filenames separated by ';' | - | names of sounds that can play while playing this frame
name | string | "none" | name of frame

## IMAGE
name|argument type|default value|description
----|-------------|-------------|-----------
image | string - filename | - | start of image section, filename of image 
position_x | int | 0 | position in x axis of image
position_y | int | 0 | position in y axis of image
name | string | "magick" | name of image

### Example
```
ANN

name=cpl_smok
author=Piotr Maciejewski

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
