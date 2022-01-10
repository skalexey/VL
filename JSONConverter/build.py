import os
import subprocess

if os.name == 'nt':
    os.system("build.bat")
else:
    os.system("./build.sh")

