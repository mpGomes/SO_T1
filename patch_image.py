#!/usr/bin/python

IMAGE_FILE="linux-SO.img"
OFFSET="32256"

MNT_DIR="mounted"
FILES_DIR="modified_files_dir"


import os
import sys
import shutil
import subprocess
import time

def copy_to_image(path):
    src_path= os.path.join(FILES_DIR, path)
    dst_path= os.path.join(MNT_DIR, path)
    print path
    shutil.copy2(src_path, dst_path)


if not os.path.exists(MNT_DIR):
    os.mkdir(MNT_DIR)    
if not os.path.exists(IMAGE_FILE):
    print "could not find file on cwd: ",IMAGE_FILE
    exit(-1)
if not os.path.exists(FILES_DIR):
    print "could not find file on cwd: ",FILES_DIR
    exit(-1)

    
if subprocess.check_output(["whoami"])!="root\n":
    print "need sudo to run"
    exit(-1)

subprocess.check_call("mount -o loop,offset={OFFSET} {IMAGE_FILE} {MNT_DIR}".format(**locals()), shell=True)

os.chdir(FILES_DIR)
filelist= subprocess.check_output("find . -type f", shell=True).split("\n")[:-1]
os.chdir("..")
for f in filelist:
    copy_to_image(f)

time.sleep(1)
subprocess.check_call(["umount", MNT_DIR])
