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

#include "General.hpp"


void CmdGeneralHelp()
{
	// Show the list of all commands

	/// @todo	Keep both this list and SYNTAX.md up to date.

	cout << "Command  Arguments             Description" << endl;
	cout << "-------  --------------------  -------------------------------------------------" << endl;
	cout << "QUIT     none                  Stop script execution / leave interactive session" << endl;
	cout << "HELP     none                  Show the list of all commands" << endl;
	cout << "USER     none                  Start nested interactive session" << endl;
	cout << "START    REFSTR                Start script session from .pcc file" << endl;
	cout << "-------  --------------------  -------------------------------------------------" << endl;
	cout << "SET      IDENT(x) = ANY        Load variable (x) with any IDVALs (one or more)" << endl;
	cout << "                               Multiple IDVALs will be concatenated as a STRING" << endl;
	cout << "DUMP     none                  Show the list of all user defined variables" << endl;
	cout << "-------  --------------------  -------------------------------------------------" << endl;
	cout << "PRINT    ANY                   Output any arguments to the console" << endl;
	cout << "INPUT    IDENT                 Query user input from console & store in variable" << endl;
	cout << "-------  --------------------  -------------------------------------------------" << endl;
	cout << "SYS      REFSTR                Execute OS-specific command" << endl;
	cout << "-------  --------------------  -------------------------------------------------" << endl;
	cout << "MATH     IDENT(x) = RPN        Store result of RPN calculation in variable (x)" << endl;
	cout << "-------  --------------------  -------------------------------------------------" << endl;
	cout << "This is just a quick overview, see the documenation for a full syntax reference." << endl;
}


void CmdGeneralUser(PccParser *parser, PccCore *core)
{
	// Start nested interactive session (useful for debugging)

	cout << endl;

	// Initialize nested interactive session & pass variable space
	PccSession Session(
		parser->sessionCounter + 1,
		core->variableSpace,
		""
	);

	// Start nested interactive session (useful for debugging)
	bool isSuccessful = Session.StartInteractive();

	if (!isSuccessful)
	{
		// This should never occur in interactive mode, because
		// the flag isSuccessful is ONLY invalidated (i.e. false)
		// in non-interactive mode inside PccParser;
		// the flag isSuccessful is passed through
		// from PccParser directly to PccSession
	}
}


PccError CmdGeneralStart(PccParser *parser, PccCore *core, vector<token_t> &arg)
{
	// Start script session from .pcc file

	if (arg[0].type == TOK_STRING || arg[0].type == TOK_IDENT || arg[0].type == TOK_IDENTARR)
	{
		string refstr;
		if (arg[0].type == TOK_STRING)
		{
			refstr = arg[0].value;
		}
		else
		{
			// Dereference variable
			variable_t* variable = core->VariableFind(arg[0].value);

			if (variable == NULL)
			{
				// Runtime error
				return PccError(
					ERR_RUNTIME,
					"Attempting to read undefined variable "	\
					"(" + arg[0].value + ")"
				);
			}

			if (variable->type != VAR_STRING)
			{
				// Runtime error
				return PccError(
					ERR_RUNTIME,
					"Variable not of type string "	\
					"(" + arg[0].value + ")"
				);
			}

			refstr = variable->value;
		}


		// Initialize script session
		string scriptFilename = refstr;
		PccSession Session(
			parser->sessionCounter + 1,
			core->variableSpace,
			scriptFilename
		);

		// Check if script was loaded
		if (!Session.scriptStream)
		{
			// Exit application with error
			return PccError(
				ERR_RUNTIME,
				"File not found "	\
				"(" + refstr + ")"
			);
		}

		// Start script session
		bool isSuccessful = Session.StartScript();

		if (!isSuccessful) return PccError(ERR_RUNTIME, "Faulty script ended with errors.");



	} else return PccError(ERR_SYNTAX, "REFSTR expected (string or variable of type string)");

	return PccError();	// No error
}
