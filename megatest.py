import subprocess

# a = [1234]
# b = [7654]
# resul = a[0] + b[0]
tests = 4

with open("test.in", "w") as fh:
    fh.write("var a 1234\n")
    fh.write("var b 7654\n")
    fh.write("eval a + b\n")
    fh.write("quit\n")

subprocess.call(["megaunit.exe"])

errors, passes, uninplementeds = 0, 0, 0
with open("test.out", "r") as fh:
    msgs = fh.readlines()
    for msg in msgs:
        if msg == "8888\n": #"str(resul)"
            passes += 1
        elif msg == "Error\n":
            errors += 1
        elif msg == "Uninplemented\n":
            uninplementeds += 1
        elif msg == "OK\n":
            passes += 1
        else:
            errors += 1
    
print("The program ran with:")
print(errors, "errors,")
print(passes, "passes,")
print(uninplementeds, "uninplemented tests")
print("Of", tests, "tests")
