MetaANN file is configuration file used to compile ann files for Piklib/Bloomoo engine. It can be opened by notepad or any other simmilar text editor.

MANN structure is simple and hierarhical.
MANN always starts with ANN in header.
Then comes global ann options.
Then Events and frames.

Then images description.

I'll disect parts on sections and describe sections:

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

