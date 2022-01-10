

import shutil

try:
    shutil.rmtree("Build")
except OSError as e:
    print("%s: '%s'" % (e.strerror, e.filename))
print("'Build' directory removed")

