import os
import subprocess
import sys

args = ""
if (len(sys.argv) > 1):
    args = " " + sys.argv[1]

status = 0

if os.name == 'nt':
    status = os.system("build.bat" + args)
else:
    status = os.system("./build.sh" + args)

exit(status)
