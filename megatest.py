import subprocess

# a = [1234]
# b = [7654]
# resul = a[0] + b[0]
tests = 4

with open("test.in", "w") as fh:
    fh.write("var a 1234")
    fh.write("var b 7654")
    fh.write("eval a + b")
    fh.write("quit")

subprocess.call(["megaunit.exe"])

errors, passes, uninplementeds = 0, 0, 0
with open("test.out", "r") as fh:
    msgs = fh.readlines()
    for msg in msgs:
        if msg == "8888": #"str(resul)"
            passes++
        elif msg == "Error":
            errors++
        elif msg == "Uninplemented":
            uninplementeds++
        elif msg == "OK":
            passes++
        else:
            errors++
    
print("The program ran with:")
print(errors, "errors,")
print(passes, "passes,")
print(uninplemented, "uninplemented tests")
print("Of", tests, "tests")
