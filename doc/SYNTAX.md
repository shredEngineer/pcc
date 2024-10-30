Syntax Reference
================


General
=======

The PCC syntax looks like a mixture of Assembler, BASIC and C(++); that's why I like to call it an *ABC-Syntax*.

The syntax is strictly command-based (*mnemonics*) but often allows for a variable number of arguments.

- Every line of a PCC script consists of a single command.
- Commands are case insensitive, although I prefer to use all UPPERCASE.
- Variable names and predefined identifiers, however, *are* case sensitive.
- Arguments to commands are separated by a single whitespace character.
- Additional whitespace (space and tab characters) is ignored.
- Nested commands are not supported. (Always store intermediate results.)
- Every argument has a data type which can dynamically change at runtime.

Use hashtag (#) comments to document your code. (Block comments are not supported.) E.g.:

<PRE><SPAN style="color:DarkCyan"># This is a one-line comment.</SPAN>
<SPAN style="color:DarkBlue">PRINT</SPAN> <SPAN style="color:DarkOrange">"Hi!"</SPAN> <SPAN style="color:DarkCyan"># This is an end-of-line comment.</SPAN>
</PRE>


- - -


Data types
==========

Every argument has one of the following basic data types. Type abbreviations are introduced for convenience.


### Basic types

Type | Description
-----|------------
<CODE>IDENT</CODE>	|	A variable name or label starts with a letter and may contain letters (a..z, A..Z), digits (0..9) and underscores (<CODE>\_</CODE>)
<CODE>IDENTARR</CODE>	|	A variable name followed by an additional pair of square brackets containing an arbitrary string, i.e. <CODE>MyVariable\[0123Hello\]</CODE>
<CODE>STRING</CODE>	|	A constant character string is enclosed in double quotes (<CODE>&quot;Hi!&quot;</CODE>). To include a double quote inside the string itself, it must be escaped using a backslash (<CODE>\\&quot;</CODE>). To include a backslash inside the string, it must also be escaped using another backslash (<CODE>\\\\</CODE>).
<CODE>NUMBER</CODE>	|	A constant number consists of digits and may contain a single dot for floating point values and a minus sign for negative values.


### Type abbreviations

General types:

- <CODE>VALUE</CODE> = <CODE>NUMBER</CODE> or <CODE>STRING</CODE>
- <CODE>IDVAL</CODE> = <CODE>IDENT</CODE> or <CODE>IDENTARR</CODE> or <CODE>VALUE</CODE>
- <CODE>ANY</CODE> = Arbitrary list of space-separated <CODE>IDVAL</CODE> type arguments

Literal types:

- <CODE>REFSTR</CODE> = <CODE>STRING</CODE> or <CODE>IDENT</CODE> or <CODE>IDENTARR</CODE>, where the dereferenced variable (<CODE>IDENT</CODE>) must have type <CODE>STRING</CODE>.

Mathematical types:

- <CODE>REFNUM</CODE> = <CODE>NUMBER</CODE> or <CODE>IDENT</CODE> or <CODE>IDENTARR</CODE>, where the dereferenced variable (<CODE>IDENT</CODE> or <CODE>IDENTARR</CODE>) must have type <CODE>NUMBER</CODE>.
- <CODE>OPERATOR</CODE> = <CODE>+</CODE> or <CODE>-</CODE> or <CODE>\*</CODE> or <CODE>/</CODE>


*If a dereferenced variable (<CODE>REFSTR</CODE> or <CODE>REFNUM</CODE>) does not match the expected type, a runtime error will be thrown.*

- - -


Command reference
=================

The standard commands are categorized into several groups.

The notation of command-specific arguments introduces references (x) for easier documentation, like so: <CODE>\<ARGTYPE\></CODE>(x)


General
-------

Command	|	Arguments	|	Description
--------|---------------|--------------
<CODE>QUIT</CODE>	|	*none*		|	Stop script execution / leave interactive session
<CODE>HELP</CODE>	|	*none*		|	Show the list of all commands
<CODE>USER</CODE>	|	*none*		|	Start nested interactive session. Useful for debugging. **User variables will be shared between sessions.**
<CODE>START</CODE>	|	<CODE>REFSTR</CODE>		|	Start script session using the supplied filename.


Variables
---------

Command	| Arguments	| Description
--------|-----------|------------
<CODE>SET</CODE>	|	<CODE>IDENT</CODE>(x)	=	<CODE>ANY</CODE>	|	Load variable (x) with any IDVALs (1 or more). If just one <CODE>IDVAL</CODE> is supplied, the variable will assume its type. Multiple <CODE>IDVAL</CODE>s will be concatenated as a <CODE>STRING</CODE>.
<CODE>DUMP</CODE>	|	*none*											|	Show the list of all user defined variables.


Console
-------

Command	|	Arguments				|	Description
--------|---------------------------|--------------
<CODE>PRINT</CODE>	|	<CODE>ANY</CODE>		|	Output any arguments to the console. Integer numbers are displayed as such while floating point numbers are displayed with six significant figures.
<CODE>INPUT</CODE>	|	<CODE>IDENT</CODE>(x)	|	Query user input from console & store in variable (x).
The input format must match the current data type of variable (x). On mismatch, the query will be repeated.


Operating System
----------------

Command	| Arguments	| Description
--------|-----------|------------
<CODE>SYSTEM</CODE>	|	<CODE>REFSTR</CODE>	|	Execute OS-specific command.


Mathematics
-----------

Mathematical operations are carried out in reverse polish notation (RPN) because it is easy to implement and it doesn't depend on braces or on operator precedence.

Operands must be either numbers or variables. Supplying a variable with a type other than <CODE>NUMBER</CODE> will result in a runtime error.

Command	| Operands | Description
--------|----------|------------
<CODE>MATH</CODE>	|	<CODE>IDENT</CODE>(x)	<CODE>=</CODE>	<CODE>RPN</CODE>	|	See table below

A simple <CODE>RPN</CODE> term consists of an operator followed by one or more operands. See the table below for a full list of supported operators and the required number of operands.

<CODE>RPN</CODE> terms may be nested arbitrarily, e.g. <CODE>+ * 4 5 3</CODE> is valid and is equivalent to <CODE>((4 * 5) + 3)</CODE> in infix (\"normal\") notation.

The result of all (nested) calculations will then be stored in the variable (x).

Operator | Operands | Description
---------|----------|--------------
<CODE>+</CODE>	|	<CODE>REFNUM</CODE>(a)	<CODE>REFNUM</CODE>(b)	|	Add (a) to (b)
<CODE>-</CODE>	|	<CODE>REFNUM</CODE>(a)	<CODE>REFNUM</CODE>(b)	|	Subtract from (a) the value (b)
<CODE>\*</CODE>	|	<CODE>REFNUM</CODE>(a)	<CODE>REFNUM</CODE>(b)	|	Multiply (a) times (b)
<CODE>/</CODE>	|	<CODE>REFNUM</CODE>(a)	<CODE>REFNUM</CODE>(b)	|	Divide (a) by (b)

*You are hereby invited to also implement an infix notation parser, if that's what you like better.* :-)


- - -


User variables
==============

Variables consist of an identifier (<CODE>IDENT</CODE>) and a value (<CODE>VALUE</CODE>).
All variables are global, i.e. they can always be accessed from any section of the program.

PCC variables have dynamic data types, i.e. variables will adopt the data type of the data assigned to it using the <CODE>SET</CODE> command.

Variables do not have to be declared explicitly. A variable (<CODE>IDENT</CODE>/<CODE>VALUE</CODE> pair) is created as soon as a value is assigned to it.

Please note that you have to assign a value to a variable first before you can read its value. Accessing an undefined variable will result in a runtime error.


- - -


Predefined identifiers
======================

The following identifieres are read-only. Writing to these identifiers will result in a runtime error.


Constants
---------

These constants provide quick access to commonly used symbols:

Identifier | Description
-----------|------------
<CODE>ENDL</CODE>	|	End-of-line character (LF on Linux systems & CRLF on Windows systems).


Variables
---------

These variables provide quick access to commonly used data:

Identifier | Description
-----------|------------
<CODE>TIME</CODE>	|	Returns the current system time formatted as mm:hh:ss.
<CODE>DATE</CODE>	|	Returns the current system data formatted as YYYY-MM-DD.


- - -


Code examples
=============

See the <CODE>tutorials/</CODE> folder to get an idea of how coding for PCC works.
