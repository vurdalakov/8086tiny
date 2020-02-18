This git repository is a fork of 8086tiny project by Adrian Cable: https://github.com/adriancable/8086tiny

Goal of this fork is to provide a Visual Studio 2019 project to build 8086tiny on Windows.

8086tiny
========

8086tiny is a completely free ([MIT License](license.txt)) open source PC XT-compatible emulator/virtual machine written in C. It is, we believe, the smallest of its kind (the fully-commented source is under 25K). Despite its size, 8086tiny provides a highly accurate 8086 CPU emulation, together with support for PC peripherals including XT-style keyboard, floppy/hard disk, clock, audio, and Hercules/CGA graphics. 8086tiny is powerful enough to run software like AutoCAD, Windows 3.0, and legacy PC games: the 8086tiny distribution includes Alley Cat, the author's favorite PC game of all time.

8086tiny is highly portable and runs on practically any little endian machine, from simple 32-bit MCUs upwards. 8086tiny has successfully been deployed on 32-bit/64-bit Intel machines (Windows, Mac OS X and Linux), Nexus 4/ARM (Android), iPad 3 and iPhone 5S (iOS), and Raspberry Pi (Linux).

The philosophy of 8086tiny is to keep the code base as small as possible, and through the open source license encourage individual developers to tune and extend it as per their specific requirements, adding support, for example, for more complex instruction sets (e.g. Pentium) or peripherals (e.g. mouse). Forking this repository is highly encouraged!

Any questions, comments or suggestions are very welcome in our forum at [8086tiny.freeforums.net](http://8086tiny.freeforums.net/).

### How to compile 8086tiny with Visual Studio 2019

Visual Studio 2019 solution and C++ project are located in `vs2019` directory.

Below are steps you need to perform to make it yourself.

#### Visual Studio 2019 project

1. Download and install Visual Studio 2019 Community: https://visualstudio.microsoft.com/vs/community/
1. Start Visual Studio 2019 and click "File" / "New" / "Project".
1. Create a "Empty Project" (C++ / Windows / Console)
1. Add `8086tiny.c` file to project.
1. Modify project properties:
    1. "Debugging" / "Command Arguments" : "..\bios ..\fd.img"
    1. "C/C++" / "General" / "SDL checks" : "No (/sdl-)"
    1. "Linker" / "System" / "SubSystem" : "Console (/SUBSYSTEM:CONSOLE)"
    1. "Linker" / "Advanced" / "Entry Point" : ""
    
If you don't need graphics, add the `NO_GRAPHICS` define to "C/C++" / "Preprocessor" / "Preprocessor Definitions". Otherwise go through next section.

#### SDL/graphics support

1. Download SDL 1.2 development libraries (Visual C++) from here: https://www.libsdl.org/download-1.2.php
1. Unpack `SDL-devel-1.2.15-VC.zip` to `sdl` subdirectory of the project directory
1. Add `iob.c` file based on this Stack Overflow answer: https://stackoverflow.com/a/34655235/11196063
1. Modify project properties:
    1. "C/C++" / "General" / "Additional Include Directories" : "$(ProjectDir)sdl\include"
    1. "Linker" / "General" / "Additional Library Directories" : "$(ProjectDir)sdl\lib\x86"
    1. "Linker" / "Input" / "Additional Dependencies" : add "SDL.lib;SDLmain.lib;DbgHelp.lib;legacy_stdio_definitions.lib"
    1. "Build Events" / "Post-Build Event" / "Command Line" : "copy /y $(ProjectDir)sdl\lib\x86\SDL.dll $(OutDir)"

### Run from command line

If you don't want to run 8086tiny from Visual Studio, do it from the command line:

From a bootable floppy without hard disk:

```
8086tiny bios fd.img
```

From a bootable floppy with hard disk:

```
8086tiny bios fd.img hd.img
```

### Links

* An obfuscated version of 8086tiny (condensed into just 4043 bytes of C code) was a winner of the 2013 IOCCC contest: https://ioccc.org/2013/cable3/hint.html
* 8086tiny forum: http://8086tiny.freeforums.net/
* 8086 Tiny Plus, an enhanced version of 8086tiny (MCGA graphics, floppy disk swapping, serial port emulation, built in debugger): https://jaybertsoftware.weebly.com/8086-tiny-plus.html
