import os
import subprocess

status = 0

if os.name == 'nt':
    status = os.system("build.bat")
else:
    status = os.system("./build.sh")

exit(status)
