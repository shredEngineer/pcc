Installing and Building
=======================

Install
=======

No installation required, just download PCC and use the pre-compiled binary inside the <CODE>bin</CODE> directory.

You might want to add the <CODE>bin</CODE> folder to your system's PATH variable in order to access it from any location.


Build
=====

You can build PCC yourself using GCC (g++) on Linux systems or MinGW on Windows systems.

Please note that your compiler must support the C++11 standard. I use g++ (GCC) 4.9.3.

Makefile targets
----------------

- Just type <CODE>make help</CODE> to see the list of all targets.
- To compile the project, just type <CODE>make clean build</CODE>
- To test the PCC binary with a simple example script, just type <CODE>make test</CODE>
- To start the PCC binary in interactive mode, just type <CODE>make user</CODE>
- Feel free to look inside the <CODE>Makefile</CODE> to adjust your settings.

Dependencies
------------

None.
