import shutil
import os
import sys

build = "Build"
deps = "dependencies"

def systemRemoveDir(path):
    code = -1
    if os.name == 'nt':
        code = os.system("rm \"" + path + "\"")
    else:
        code = os.system("rm -rf \"" + path + "\"")
    if code == 0:
        print("Directory '%s' removed" % path)
    else:
        print("Return code: %d" % code)

def removeDir(path):
    if os.path.exists(path):
        try:
            shutil.rmtree(path)
            print("Directory '%s' removed" % path)
        except OSError as e:
            print("%s: '%s'" % (e.strerror, e.filename))
            print("Try to use shell command")
            systemRemoveDir(path)

    else:
        print("'%s' directory already removed" % path)

removeDir(build)

if len(sys.argv) > 1 and sys.argv[1] == "deps":
    removeDir(deps)

