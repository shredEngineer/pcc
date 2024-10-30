Paul's Code Collection
======================


About PCC
=========

**A powerful cross-platform script interpreter that is extensible.** Currently, Linux and Windows platforms are supported.

PCC is licensed under GPLv3, see [LICENSE.md](doc/LICENSE.md). Anyone interested in improving PCC is free to contribute to this project!

Copyright (C) 2016-2017 by Paul Wilhelm <<anfrage@paulwilhelm.de>>. (Please send bug reports to this address.)

Visit http://paulwilhelm.de/pcc for further information and other cool stuff.


Features
========

This project follows the *KISS* philosophy: *Keeping it small & simple!*

PCC just works. Many commonly used functions are supported. If not, you can easily implement your own modules.

PCC is non-verbose. You can get right down to coding the core functionality without the usual boilerplate overhead.

PCC is robust. Dynamic variables make your life easier without having to worry about data types or memory allocation.

*Yes, of course, I accept feature requests.* :-)


Usage
=====

- Start script: <CODE>pcc script.pcc</CODE>
- Start interactive session: <CODE>pcc</CODE>


Example
=======

This is what a very simple, hello-worldey PCC script might look like:

<PRE><SPAN style="color:DarkBlue">SET</SPAN> X = <SPAN style="color:DarkOrange">123.456789</SPAN>
<SPAN style="color:DarkBlue">PRINT</SPAN> <SPAN style="color:DarkOrange">"Hey there! Here is my number: "</SPAN> X <SPAN style="color:DarkBlue">ENDL</SPAN>
<SPAN style="color:DarkBlue">QUIT</SPAN> <SPAN style="color:DarkCyan"># Done! (We could omit this command BTW)</SPAN>
</PRE>

Check out [The Syntax Reference](doc/SYNTAX.md) for an exhaustive list of all commands and how they work!

See the <CODE>tutorials/</CODE> folder for example code to get an idea of how coding with PCC works.


Resources
=========

See the following pages for additional information on how to get the best out of PCC:

Page | Description
-----|------------
[Syntax Reference](doc/SYNTAX.md)			|	The comprehensive list of all commands and general syntax.
[Installing and Building](doc/INSTALL.md)	|	Instructions for installing and building PCC.
[Custom Extensions](doc/EXTEND.md)			|	Instructions for extending PCC with custom code.
[Future Improvements](doc/IMPROVE.md)		|	Some notes on future improvements of the PCC project.
[Changelog](doc/CHANGED.md)					|	The list of changes made to different versions of PCC.


Structure
=========

- <CODE>bin/</CODE> - Contains the PCC binary
- <CODE>doc/</CODE> - Doxygen documentation
- <CODE>geany/</CODE> - Contains the custom Geany PCC filetype
- <CODE>obj/</CODE> - Contains object files
- <CODE>src/</CODE> - All C++ source files
- <CODE>tutorial/</CODE> - Example PCC scripts
- <CODE>Makefile</CODE> - Try <CODE>make help</CODE>
- <CODE>README.md</CODE> - This file


PCC Syntax Highlighting
=======================

Enjoy your PCC source code in full color with the customized PCC syntax highlighting scheme for the fabulous [Geany Editor](https://www.geany.org/)!

The contents of the <CODE>geany/</CODE> folder need to be copied to your Geany installation folder. The exact subfolder location is, however, version and OS dependent.

This will also configure your <CODE>F5</CODE> key inside Geany to start the current PCC file. (Be sure that the <CODE>bin/</CODE> folder is inside your **PATH**.)

Motivation
==========

Last but not least: Why would I design my own programming language anyway?

1. I frequently need to write small applications where the overhead of traditional languages seems too high.

2. PCC provides a comfortable way to easily reuse valuable code which is needed over and over again.

3. Others might also benefit from these advantages and PCC could make their lives a little bit easier as well.

4. It's fun and I have gained many great experiences during the development process. :-)

**In case you feel the same way about this, you are invited to contribute to this project!**
