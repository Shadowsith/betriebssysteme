#--------------------------------------------------------------------
# Demo showing magic numbers and binary format loaders
# FABR: 02.04.2013
#--------------------------------------------------------------------

Magic numbers help identifiying file types.

By looking into files and matching against certain patterns (magic numbers)
the kernel finds out how some file, which has its executable bit set, should be
executed.

#-----------------------------
# Identification by the kernel
#-----------------------------

- Some well known formats (like ELF, COFF) are built into the kernel

$ find ./linux-source-*/fs -name '*binfmt*.c'
./linux-source-3.5.0/fs/binfmt_em86.c
./linux-source-3.5.0/fs/binfmt_elf_fdpic.c
./linux-source-3.5.0/fs/binfmt_script.c
./linux-source-3.5.0/fs/binfmt_misc.c
./linux-source-3.5.0/fs/binfmt_aout.c
./linux-source-3.5.0/fs/binfmt_flat.c
./linux-source-3.5.0/fs/binfmt_som.c
./linux-source-3.5.0/fs/compat_binfmt_elf.c
./linux-source-3.5.0/fs/binfmt_elf.c

 
- Additional custom formats may be added by the user (root of course)
  See <kernel-source>/Documentation/binfmt_misc.txt

- There is the generic mechanism using the Shebang '#!'-Notation

#--------------------------------------------
# Example for magic numbers used by binfmt_misc
# Python precompiled files
#

Create a tiny python script

$ echo 'print "Hello World"' > hello.py

$ cat hello.py

Run by calling the interpreter explicitly

$ python hello.py

Compile it into a .pyc file

$ python
Python 2.7.3 (default, Sep 26 2012, 21:51:14) 
[GCC 4.7.2] on linux2
Type "help", "copyright", "credits" or "license" for more information.
>>> import py_compile
>>> py_compile.compile("hello.py")
>>> ^D

The file generated is named: hello.pyc

We rename it, in order to demonstrate, that extensions play no role in linux

$ mv hello.pyc hello.compiled

Set executable bit

$ chmod u+x hello.compiled
$ ls -la hello.compiled

-rwxr--r-- 1 regensbu regensbu 116 Apr  2 16:58 hello.compiled

Execute:

$ ./hello.compiled 
Hello World

How does it work?

Enter: magic numbers

$ hexdump  -n32 -C ./hello.compiled 
00000000  03 f3 0d 0a a9 f1 5a 51  63 00 00 00 00 00 00 00  |......ZQc.......|
00000010  00 01 00 00 00 40 00 00  00 73 09 00 00 00 64 00  |.....@...s....d.|

The kernels checks executable files for certain magic numbers

$ ls /proc/sys/fs/binfmt_misc/
jar  python2.7  python3.2  register  status

$ cat /proc/sys/fs/binfmt_misc/python2.7

enabled
interpreter /usr/bin/python2.7
flags: 
offset 0
magic 03f30d0a

Our compiled file ./hello.compiled contains the magic numbers 03f30d0a at
offset 0
-> the kernel knows that it has to run the file via the interpreter
/usr/bin/python2.7

#--------------------------------------------
# '#!'-Shebang approach
#--------------------------------------------

The approach above is only feasible for compiled code.
For pure source code interpretation the '#!'-Shebang mechanism is used

Example: Shell scripts

Create a tiny shell script (technique: HERE-Style)

cat > hello.blablu <<HERE
#!/bin/bash

echo "Hello from a shell script!"
HERE

Run via explicit call to interpreter /bin/bash

$ /bin/bash ./hello.blablu

Set executable bits and rely on '#!'-Shebang

$ chmod u+x ./hello.blablu
$ ls -la ./hello.blablu

$ ./hello.blablu

Note:
In the shell script ./hello.blablu there is the magic number '2321' at offset 0
The 'magic number' is constituted by the characters '#' (Code 0x23) and
'!' (Code 0x21)

$ hexdump  -n32 -C ./hello.blablu 
00000000  23 21 2f 62 69 6e 2f 62  61 73 68 0a 0a 65 63 68  |#!/bin/bash..ech|
00000010  6f 20 22 48 65 6c 6c 6f  20 66 72 6f 6d 20 61 20  |o "Hello from a |

The recognition of the '#!'-pattern is usually built into the kernel via the
file linux-source-*/fs/binfmt_script.c

#-------------------------------------------------------------------------------
#-------------------------------------------------------------------------------
#-------------------------------------------------------------------------------

#----------------------------------------------------
# General mechanism of file type recognision
# Magic numbers and the /usr/share/misc/magic DB
#
#----------------------------------------------------

Manual pages:

man -s 1 file

See also:
http://www.itworld.com/operating-systems/344491/unix-under-spell-magic-numbers

Question:
Where is the source of the compiled version magic.msc ???

Answer:
You need to install the source package of packages 'file'

1) Add source code repo matching your distribution
For Mint 14 this is 
  deb-src http://archive.ubuntu.com/ubuntu/ quantal main restricted \
          universe multiverse

Update lists

apt-get update


2) Download the source code of package 'file' containing the 'file' utility

Note: the source code is downloaded to the working directory

mkdir -p /tmp/Downloads
cd /tmp/Downloads

apt-get source file

Hint: we put the downloaded and patched source into the directory 
CodeExamples/Magic/file*

#--------------------------------------------------------------------

Magic specification of ELF binaries:
See file-5.11/magic/Magdir/elf

0       string          \177ELF         ELF
>4      byte            0               invalid class
>4      byte            1               32-bit
>4      byte            2               64-bit

Inspect an ELF binary on your computer

$ hexdump  -n32 -C ~/Bs/CodeExamples/SysCalls/bin/lowlevel 
00000000  7f 45 4c 46 02 01 01 03  00 00 00 00 00 00 00 00  |.ELF............|
00000010  02 00 3e 00 01 00 00 00  58 10 40 00 00 00 00 00  |..>.....X.@.....|

or octal (better for interpreting the magic spec)
The pattern 0177 is octal and is the same as 0x7f

$ hexdump -b -n32 -C ~/Bs/CodeExamples/SysCalls/bin/lowlevel 
0000000 177 105 114 106 002 001 001 003 000 000 000 000 000 000 000 000
00000000  7f 45 4c 46 02 01 01 03  00 00 00 00 00 00 00 00  |.ELF............|
0000010 002 000 076 000 001 000 000 000 130 020 100 000 000 000 000 000
00000010  02 00 3e 00 01 00 00 00  58 10 40 00 00 00 00 00  |..>.....X.@.....|

#--------------------------------------------------------------------

Magic specification of a java class file 
See file-5.11/magic/Magdir/cafebabe

0       belong          0xcafebabe
!:mime  application/x-java-applet
>4      belong          >30             compiled Java class data,
>>6     beshort         x               version %d.
>>4     beshort         x               \b%d
# Which is which?
#>>4    belong          0x032d          (Java 1.0)
#>>4    belong          0x032d          (Java 1.1)
>>4     belong          0x002e          (Java 1.2)
>>4     belong          0x002f          (Java 1.3)
>>4     belong          0x0030          (Java 1.4)
>>4     belong          0x0031          (Java 1.5)
>>4     belong          0x0032          (Java 1.6)

Inspect a java class file

hexdump -n64 -C ~/GdPII/CodeExamples/Quicky/classes/GGT.class

00000000  ca fe ba be 00 00 00 33  00 32 07 00 02 01 00 03  |.......3.2......|
00000010  47 47 54 07 00 04 01 00  10 6a 61 76 61 2f 6c 61  |GGT......java/la|
00000020  6e 67 2f 4f 62 6a 65 63  74 01 00 06 3c 69 6e 69  |ng/Object...<ini|
00000030  74 3e 01 00 03 28 29 56  01 00 04 43 6f 64 65 0a  |t>...()V...Code.|
00000040

Offset 0: cafebabe      (big endian long)
Offset 4: 00 00 00 33   (big endian long)

--> Java version 1.7

According to the specification in the magic file the file derives
java version 51.0 (33 hex is 51 decimal)
>>6     beshort         x               version %d.

file  ~/GdPII/CodeExamples/Quicky/classes/GGT.class 
/export/home/regensbu/GdPII/CodeExamples/Quicky/classes/GGT.class:
        compiled Java class data, version 51.0

For a java 6 class file the output would be 50.0
