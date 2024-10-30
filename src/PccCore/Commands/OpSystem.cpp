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

#include "OpSystem.hpp"


PccError CmdOpSystemSystem(PccCore *core, vector<token_t> &arg)
{
	// Execute OS-specific command

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


		int status = system(refstr.c_str());
		if (status < 0)
		{
			cout << "SYS error: " << strerror(errno) << endl;
		}


	} else return PccError(ERR_SYNTAX, "REFSTR expected (string or variable of type string)");

	return PccError();	// No error
}
