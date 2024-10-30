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

#include "Console.hpp"


PccError CmdConsolePrint(PccCore *core, vector<token_t> &arg)
{
	// Output any arguments to the console

	for (size_t i = 0; i < arg.size(); i++)
	{
		if (arg[i].type == TOK_STRING || arg[i].type == TOK_NUMBER)
		{
			if (arg[i].type == TOK_NUMBER)
			{
				cout << FormatAutoInt(arg[i].value);
			}
			else
			{
				cout << arg[i].value;
			}
		}
		else if (arg[i].type == TOK_IDENT || arg[i].type == TOK_IDENTARR) {
			// Dereference variable
			variable_t* variable = core->VariableFind(arg[i].value);

			if (variable == NULL)
			{
				// Runtime error
				return PccError(
					ERR_RUNTIME,
					"Attempting to read undefined variable (" +
					arg[i].value +
					")"
				);
			}

			if (variable->type == VAR_NUMBER)
			{
				cout << FormatAutoInt(variable->value);
			}
			else
			{
				cout << variable->value;
			}
		}
		else
		{
			// Internal error
			return PccError(
				ERR_INTERNAL,
				"Unexpected/unhandled token type (COUT): Enum " +
				to_string(arg[i].type)
			);
		}

	}

	return PccError();	// No error
}


PccError CmdConsoleInput(PccCore *core, vector<token_t> &arg)
{
	// Query user input from console & store in variable

	if (arg[0].type != TOK_IDENT || arg[0].type == TOK_IDENTARR)
	{
		return PccError(ERR_SYNTAX, "Identifier expected");
	}

	string ident = arg[0].value;

	variable_t* variable = core->VariableFind(ident);

	if (variable == NULL)
		// Runtime error
		return PccError(
			ERR_RUNTIME,
			"Attempting to INPUT into undefined variable "	\
			"(" + ident + ")"
		);

	if (variable->type == VAR_STRING)
	{
		string cin_string;
		getline(cin, cin_string);
		variable->value = cin_string;
	}
	else if (variable->type == VAR_NUMBER)
	{
		double cin_number;
		while (! (cin >> cin_number) )
		{
			cout << "Invalid input - Number required" << endl;
			cin.clear();	// Clear CIN error flags
			cin.sync();		// Clear CIN buffer (e.g. input after space)
		}
		cin.sync();			// Clear CIN buffer (e.g. input after space)

		// Assign converted value
		// Every number should have
		// 15 significant figures in the variable space
		variable->value = DoubleToStr(cin_number);
	}
	else
	{
		// Internal error
		return PccError(
			ERR_INTERNAL,
			"Unexpected/unhandled variable type (INPUT): Enum " +
			to_string(variable->type)
		);
	}

	return PccError();	// No error
}
