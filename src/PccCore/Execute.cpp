/*
 * PCC - Paul's Code Collection
 * A powerful cross-platform script interpreter that is extensible.
 * Copyright (C) 2016-2017 by Paul Wilhelm <anfrage@paulwilhelm.de>
 *
 * PCC is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PCC is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Execute.hpp"


PccError PccCore::Execute(
	PccParser		*parser,
	string			cmd,
	vector<token_t>	arg
)
{
	// Process command/arguments & execute related logic

	size_t argc = arg.size();

	bool errUnknownCommand = false;		// Error flag
	bool errBadArgumentCount = false;	// Error flag

	// -----------------------------------------------------------------
	// General (Commands/General._pp)

	if (cmd == "QUIT")
	{
		// Stop script execution / leave interactive session

		if (argc == 0)
		{
			// No need to outsource this to General._pp
			parser->isRunning = false;
		}
		else errBadArgumentCount = true;
	}


	else if (cmd == "HELP")
	{
		// Show the list of all commands

		if (argc == 0)
		{
			CmdGeneralHelp();
		}
		else errBadArgumentCount = true;
	}


	else if (cmd == "USER")
	{
		// Start nested interactive session (useful for debugging)

		if (argc == 0)
		{
			CmdGeneralUser(parser, this);
		}
		else errBadArgumentCount = true;
	}


	else if (cmd == "START")
	{
		// Start script session from .pcc file

		if (argc == 1)
		{
			PccError error = CmdGeneralStart(parser, this, arg);
			if (error.hasOccured()) return error;
		}
		else errBadArgumentCount = true;
	}


	// -----------------------------------------------------------------
	// Variables (Commands/Variables._pp)

	else if (cmd == "SET")
	{
		// Load variable with any arguments (one or more)

		if (argc >= 3)
		{
			PccError error = CmdVariablesSet(parser, this, arg);
			if (error.hasOccured()) return error;
		}
		else errBadArgumentCount = true;
	}


	else if (cmd == "DUMP")
	{
		// Show the list of all user defined variables

		if (argc == 0)
		{
			PccError error = CmdVariablesDump(this);
			if (error.hasOccured()) return error;
		}
		else errBadArgumentCount = true;
	}


	// -----------------------------------------------------------------
	// Console (Commands/Console._pp)

	else if (cmd == "PRINT")
	{
		// Output any arguments to the console

		if (argc > 0)
		{
			PccError error = CmdConsolePrint(this, arg);
			if (error.hasOccured()) return error;
		}
		else errBadArgumentCount = true;
	}


	else if (cmd == "INPUT")
	{
		// Query user input from console & store in variable

		if (argc == 1)
		{
			PccError error = CmdConsoleInput(this, arg);
			if (error.hasOccured()) return error;
		}
		else errBadArgumentCount = true;
	}


	// -----------------------------------------------------------------
	// Operating System (Commands/OpSystem._pp)

	else if (cmd == "SYSTEM")
	{
		// Execute operating system-specific command

		if (argc == 1)
		{
			PccError error = CmdOpSystemSystem(this, arg);
			if (error.hasOccured()) return error;
		}
		else errBadArgumentCount = true;
	}


	// -----------------------------------------------------------------
	// Math (Commands/MathRpn._pp)

	else if (cmd == "MATH")
	{
		// Execute RPN calculation

		if (argc >= 3)
		{
			PccError error = CmdMathRpnSolve(this, arg);
			if (error.hasOccured()) return error;
		}
		else errBadArgumentCount = true;
	}


	// -----------------------------------------------------------------
	// <YourCategory>
	/*
	else if (command == "<YourCommand>")
	{
		// <YourDescription>

		// Check arguments & do stuff ...
	}
	*/

	else errUnknownCommand = true;


	// Return error messages
	if (errUnknownCommand)
	{
		return PccError(ERR_SYNTAX, "Unknown command");
	}
	else if (errBadArgumentCount)
	{
		return PccError(ERR_SYNTAX, "Bad argument count");
	}
	else
	{
		return PccError();	// No error
	}
}
