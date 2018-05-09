import sys
import platform

debug = True
optimizate = False
test = False

cc_path = ""
asm_path = ""

cc_path_option = False
asm_path_option = False

args = sys.argv[1:]
# if len(args) == 0:
#     args.append('-h')

for arg in args:
    if arg == "--test-ok":
        test = True
    elif arg == "--debug-ok":
        debug = True
        optimizate = False
    elif arg == "--nodebug-ok":
        debug = False
    elif arg == "--release-ok":
        debug = False
        optimizate = True
    elif arg in ["--help", "-h"]:
        print(
"""\
options:
  --test-ok: enable testing, disabled by default
  --debug-ok: enable debugging, enabled by default
  --nodebug-ok: disable debugging
  --release-ok: no debug and enable optimizations, disabled by default
  --help or -h: print this message
be good\
        """)
        exit(0)
    elif arg == "--cc-path":
        cc_path_option = True
    elif arg == "--asm-path":
        asm_path_option = True
    elif cc_path_option == True:
        cc_path_option = False
        cc_path = arg
    elif asm_path_option == True:
        asm_path_option  = False
        asm_path = arg

print('generating makefile...')
mk = open("makefile", "w")
options = ""
if debug:
    options += "-g"
if optimizate:
    options += "-O2"
if options != "":
    mk.write("options = " + options + "\n")

os = ""
if platform.machine().endswith("64"):
    if platform.system() == "Linux":
        os = "linux"
        mk.write("os = linux\n")
        mk.write("obj_ext = .o\n")
        mk.write("ASMFLAGS = -felf64 -d" + os + "\n")
        print('64 bit linux system detected')

    elif platform.system() == "Windows":
        os = "windwos"
        mk.write("os = windwos\n")
        mk.write("exe_ext = .exe\n")
        mk.write("obj_ext = .obj\n")
        mk.write("ASMFLAGS = -fwin64 -d" + os + "\n")
        print('64 bit windows system detected')

    else:
        print("Sorry, unsupported system")
        mk.close()
        exit(1)

if len(cc_path) != 0:
    if cc_path[-1] != "/":
        cc_path += "/"

if len(asm_path) != 0:
    if asm_path[-1] != "/":
        asm_path += "/"

mk.write("PATH_TO_CC = " + cc_path + "\n")
mk.write("PATH_TO_ASM = " + asm_path + "\n")

f = open("makefile.in", "r")
mk.write(f.read())

if os == 'linux':
    mk.write("""

clean:
\t@rm -f $(TEMPS)\n""")
elif os == 'windwos':
    mk.write("""

clean:
\t@del $(TEMPS) 2>nul\n""")

f.close()
mk.close()
print('makefile built')

pass
