# official system development kit
### The set is made for LINUX, you can use it on Windows using WSL

# Setup
# On Linux, do not enter the command ```wsl --install``` and ```wsl`` go further!
## if you have Windows start here:
## On windows install wsl
```
wsl --install
```
### A restart will be required
### After restart enter in cmd:
```
wsl
```
### Fill in the login and password fields
### Next
# iif you have linux start here if you have windows read from::

### Enter in wsl linux command:
```
git clone https://github.com/ROZcloud/RozOS-system-creator-set.git
```
### Go to the downloaded folder with the command:
```
cd RozOS-system-creator-set
cd RozOS
```
### Next edit kernel.c
### Run ```make install``` once to install the required packages
### After editing, to get the iso, execute the command:
```
make build
```
### An iso file will be created, to run it, execute:
```
make run-build
```
### To run without building an iso do:
```
make run
```
### this will run ```kernel.c```
