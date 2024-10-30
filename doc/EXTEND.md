Custom Extensions
=================


Overview
========

For starters, you can extend PCC by modifying <CODE>src/PccCore/Execute.cpp</CODE>. This is where you can add recognition of your own commands and call custom functions.

As you can see in the [Syntax Reference](doc/SYNTAX.md), all the standard commands are categorized into several groups, each corresponding to a separate file inside the <CODE>src/PccCore/Commands/</CODE> folder.

You should place your custom functions (like <CODE>MyModule.cpp</CODE> & <CODE>MyModule.hpp</CODE>) in a separate folder inside the <CODE>src/Modules/</CODE> folder.

The Makefile will automatically and recursively include all sources and monitor all headers inside the <CODE>src/</CODE> folder so you shouldn't have to modify it.

You should place your own header <CODE>\#includes</CODE> in <CODE>src/Global/Global.hpp</CODE> as this header is included by all other source modules. Useful global functions go in <CODE>src/Global/Global.cpp</CODE>.

The PCC core code should be relatively well documented and you should be able to jump right into developing all the additional features you need.

In case you want to fiddle with the general program logic, have a look at the rest of PccCore and check out main(), PccSession and PccParser.

**Just drop me a line if you need help or want to contribute to the PCC project.** :metal:


Process
=======

Here's what happens when PCC is started:

- main() creates a PccSession instance and either calls PccSession::StartScript() or PccSession::StartInteractive().
- After that, a PccParser instance is created, which in turn creates a PccCore instance.
- Now the PccParser is fed with either script lines from a file or interactive user input from the console.
- Each line is then tokenized and passed to the PccCore::Execute function, where commands are recognized and executed.
- The PccCore also has access to the PccParser so it can manipulate the parser state (i.e. perform jumps inside the script).

Because nested sessions (interactive or started scripts) need access to the variable space, the following procedure is implemented:

- PccCore::Execute will pass its own PccCore::variableSpace pointer to the newly spawned PccSession.
- The new PccSession then passes this pointer through to a new PccParser, which in turn passes it through to a new PccCore.
- The new PccCore then uses the given variable space instead of allocating and initializing an isolated new one by itself.
- This way any variable modification in the subsession is reflected in the original session. Global variables are thus effectively implemented.
- This is also exactly the way the <CODE>START</CODE> command works, where a new script session is spawned using the supplied filename.

Other more or less interesting stuff:

- Each nested session is enumerated using PccSession::sessionCounter and this variable is passed from PccSession through PccParser to PccCore.
- This way PccCore can then again spawn a new session with the incremented value of sessionCounter and so on. Each session is thus uniquely identified.

As you can see, the inner workings of PCC are somewhat interwoven yet clearly structured.


Variable space
==============

When writing code for PCC you will most likely also have to deal with variables. This is why the structure of the PccCore::variableSpace is explained here in detail.

The variable space is basically a vector of variable_t structures. It is divided into two parts, with predefined symbols (MODE_PREDEF_VAR & MODE_PREDEF_CONST) in the lower part and user variables (MODE_USER) in the upper part (starting at index PccCore::userVarBegin). Predefined variables (DATE, TIME, etc.) are updated dynamically everytime a PccCore::VariableFind is issued.

All data is stored as strings internally, regardless of data type. This includes floating point values which are stored as strings with 15 significant figures.

The following conversion functions are globally declard for your convenience:
- DoubleToStr(): Use this when converting floating point numbers to strings for storage in the variable space. (LOAD, INPUT, Mathematical operations, etc.)
- FormatAutoInt(): Use this when concatenating or outputting floating point strings to the console. It will return either the integer value or the floating point value rounded to six significant figures, depending on the fractional part of the number.
- Use stod() for all conversions from string to double.
