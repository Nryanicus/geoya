#!/usr/bin/python

import os, re, io
from subprocess import Popen, PIPE, call

import workerpool

class ProcessJob(workerpool.Job):

    def __init__(self, read_filename, write_filename):
        self.read_filename = read_filename
        self.write_filename = write_filename

    def run(self):
        call(["convert", self.read_filename, "-trim", self.write_filename])

READ_DIR = "/home/nathan/Projects/GeoyaRedux/BlenderRender/"
WRITE_DIR = "/home/nathan/Projects/GeoyaRedux/BlenderRender/Trimmed/"

pool = workerpool.WorkerPool(size=10)

with open("offsets.py", "w") as f:
    f.write("OFFSETS = {\n")
    for filename in os.listdir(READ_DIR):
        read_full_filename = os.path.join(READ_DIR, filename)
        # first find offsets
        p = Popen(["identify", "-format", "%@", read_full_filename], stdout=PIPE, encoding="UTF-8")
        stdout, _ = p.communicate()
        r = re.search("\+([0-9]*)\+([0-9]*)", stdout)
        x_offset, y_offset = "0","0"
        if r:
            x_offset, y_offset = r.groups()
        else:
            print( filename, stdout)

        sprite = filename[:-4]
        
        f.write('"'+sprite+'": ('+x_offset+", "+y_offset+"), ")
        # add conversion job
        write_full_filename = os.path.join(WRITE_DIR, filename)
        pool.put(ProcessJob(read_full_filename, write_full_filename))
        f.write("\n")
    f.write("}\n")

pool.shutdown()
pool.wait()