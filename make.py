#!/usr/bin/python

import os
from subprocess import call

from spritesheet_offsets import *

WRITE_DIR = "/home/nathan/Projects/GeoyaRedux/BlenderRender/Sprites/"
READ_DIR = "/home/nathan/Projects/GeoyaRedux/BlenderRender/Trimmed/"

RES = 4096

# for dirc, sheet in (("Top", TOP_SPRITESHEET), ("TopFront", TOPFRONT_SPRITESHEET)):
dirc = "Top"
sheet = TOP_SPRITESHEET
write_filename = os.path.join(WRITE_DIR, dirc+"SpriteSheet.png")
call("convert -size "+str(RES)+"x"+str(RES)+" xc:wheat -alpha transparent "+write_filename, shell=True)
max_h = 0
for (sprite, (x, y, w, h, _, _)) in sheet.items():
#     if (y == 0 and h > max_h):
#         max_h = h
# for (sprite, (x, y, w, h, _, _)) in sheet.items():
#     y = y + h - max_h
    read_filename = os.path.join(READ_DIR, sprite+".png")
    call("composite -gravity southwest -geometry +"+str(x)+"+"+str(y)+" "+read_filename+" "+write_filename+" "+write_filename, shell=True)
