from os import system
tekst = ""
print("RozOS build tols")
input("Press enter to continue")
while(True):
    print("1. build system 2. run build system 3. delete build date 4. rebuild")
    enter = (input("enter number:"))
    if enter == "1":
        system("make build")
    elif enter == "2":
        system("make run-build")
    elif enter == "3":
        system("make clean")
    elif enter == "4":
        system("make clean")
        system("make build")
        tekst = (input("run build[Y, n] :"))
        if tekst == "y" or tekst == "Y":
            print("Strting...")
            system("make run-build")
        else:
            print("ok run later!")
            exit()