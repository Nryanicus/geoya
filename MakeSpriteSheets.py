#!/usr/bin/python

import os, re, io
from subprocess import Popen, PIPE, call

from rectpack import newPacker

from offsets import OFFSETS

WRITE_DIR = "/home/nathan/Projects/GeoyaRedux/BlenderRender/Sprites/"
READ_DIR = "/home/nathan/Projects/GeoyaRedux/BlenderRender/Trimmed/"

RES = 4096

top_dimensions = []
front_dimensions = []

#get dimensions
for filename in os.listdir(READ_DIR):
    read_full_filename = os.path.join(READ_DIR, filename)
    p = Popen(["identify", "-format", "%w %h", read_full_filename], stdout=PIPE, encoding="UTF-8")
    stdout, _ = p.communicate()
    w, h = re.search("([0-9]*) ([0-9]*)", stdout).groups()
    sprite = filename[:-4]
    if sprite[:3] == "Top":
        top_dimensions.append( (sprite, (int(w), int(h)) ) )
    else:
        front_dimensions.append( (sprite, (int(w), int(h)) ) )

#pack rects and create spritesheets
with open("spritesheet_offsets.py", "w") as f:
    f.write("#sprite: x, y, w, h, x_off, y_off\n")
    for dirc, dim in (("Top", top_dimensions), ("TopFront", front_dimensions)):
        write_filename = os.path.join(WRITE_DIR, dirc+"SpriteSheet.png")
        f.write(dirc.upper()+"_SPRITESHEET = {\n")
        packer = newPacker(rotation=False)
        for (sprite, (w, h)) in dim:
            packer.add_rect(w, h, sprite)
        packer.add_bin(RES, RES)
        packer.pack()
        all_rects = packer.rect_list()
        call("convert -size "+str(RES)+"x"+str(RES)+" xc:wheat -alpha transparent "+write_filename, shell=True)
        i = 0
        for rect in all_rects:
            bin_id, x, y, w, h, sprite = rect
            i += 1
            print(i,"/",len(all_rects))
            xo, yo = OFFSETS[sprite]
            f.write("'"+sprite+"': ("+str(x)+", "+str(y)+", "+str(w)+", "+str(h)+", "+str(xo)+", "+str(yo)+"),\n")
            
            read_filename = os.path.join(READ_DIR, sprite+".png")
            call("composite -geometry +"+str(x)+"+"+str(y)+" -gravity northwest "+read_filename+" "+write_filename+" "+write_filename, shell=True)
        f.write("}\n")
