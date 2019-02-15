_Connect Four is a project requirement taylored for [ICS-OS](https://github.com/srg-ics-uplb/ics-os) under CMSC 125._

# Connect Four


Connect Four is a simple strategy game that can be played with anyone, even young children. It is similar to Tic-Tac-Toe but instead of three, the players must aim to connect four pieces in a straight line (horizontally, vertically and diagonally). Be the first one to get four of your colored pieces in a straight line. Read rules [here](http://www.boardgamecapital.com/connect-four-rules.htm).


## How to Run
Run the following commands in the directory where you placed your ics-os:
```
$ cd ics-os/ics-os
$ cd contrib/proj
$ make
$ make install
$ cd ../..
$ make
$ sudo make install
$ make floppy
$ qemu-system-i386 -fda ics-os-floppy.img -boot a
  ```
  
## Contributors:

* Ana Marion Matawaran
* Paul Joshua Vargas
