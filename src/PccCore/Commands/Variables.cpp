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

#include "Variables.hpp"


PccError CmdVariablesSet(
	PccParser		*parser,
	PccCore			*core,
	vector<token_t>	&arg
)
{
	// Load variable with any IDVALs (one or more)

	if (arg[0].type != TOK_IDENT || arg[0].type != TOK_IDENTARR)
		// Syntax error
		return PccError(
			ERR_SYNTAX,
			"Identifier expected as 1. argument"
		);

	if (arg[1].type != TOK_EQUALS)
		// Syntax error
		return PccError(
			ERR_SYNTAX,
			"Expected '='"
		);

	string leftIdent = arg[0].value;

	// Destination/concatenation buffer
	string resultValue = "";
	varType_t resultType;

	// Check if multiple arguments are supplied
	if (arg.size() > 3)
	{
		// Concatenate multiple values as string
		resultType = VAR_STRING;
		for (
			size_t rightIndex = 2;
			rightIndex < arg.size();
			rightIndex++
		)
		{
			varType_t argType;	// Type of current argument
			string argValue;	// Value of current argument

			if (arg[rightIndex].type == TOK_IDENT)
			{
				// Dereference variable
				string argIdent = arg[rightIndex].value;
				variable_t* variable = core->VariableFind(argIdent);

				if (variable == NULL)
				{
					// Runtime error
					return PccError(
						ERR_RUNTIME,
						"Attempting to read undefined variable "	\
						"(" + argIdent + ")"
					);
				}

				argType = variable->type;
				argValue = variable->value;
			}
			else
			{
				// Get argument value & type
				argType =
					parser->TypecastTokToVar(arg[rightIndex].type);
				argValue = arg[rightIndex].value;
			}

			if (argType == VAR_NUMBER)
			{
				// Convert to auto-int formatted value
				argValue = FormatAutoInt(argValue);
			}

			// Concatenate this value
			resultValue += argValue;
		}
	}
	else
	{
		// Assign single value (do not concatenate anything)
		if (arg[2].type == TOK_IDENT || arg[2].type == TOK_IDENTARR)
		{
			// Dereference variable
			string rightIdent = arg[2].value;
			variable_t* variable = core->VariableFind(rightIdent);

			if (variable == NULL)
			{
				// Runtime error
				return PccError(
					ERR_RUNTIME,
					"Attempting to read undefined variable "	\
					"(" + rightIdent + ")"
				);
			}

			resultType = variable->type;
			resultValue = variable->value;
		}
		else
		{
			// Get argument value & type
			resultType = parser->TypecastTokToVar(arg[2].type);
			resultValue = arg[2].value;

			if (resultType == VAR_NUMBER)
			{
				// Assign converted value
				// Every number should have
				// 15 significant figures in the variable space
				resultValue = DoubleToStr(stod(resultValue));
			}
		}
	}

	// Save result
	variable_t *variable = core->VariableFind(leftIdent);

	if (variable == NULL)
	{
		// Create new variable
		core->VariableCreate(
			leftIdent,
			resultValue,
			resultType,
			MODE_USER
		);
	}
	else
	{
		if (variable->mode != MODE_USER)
			// Runtime error
			return PccError(
				ERR_RUNTIME,
				"Attempting to write non-user variable "	\
				"(" + leftIdent + ")"
			);

		// Modify variable
		core->VariableModify(
			variable,
			resultValue,
			resultType,
			MODE_USER
		);
	}

	return PccError();	// No error
}


PccError CmdVariablesDump(PccCore *core)
{
	// Show the list of all user defined variables

	// Skip predefined constants & predefined variables
	for (
		size_t i = core->userVarBegin;
		i < core->variableSpace->size();
		i++
	)
	{
		if ((*core->variableSpace)[i].mode != MODE_USER)
			// Internal error
			return PccError(
				ERR_INTERNAL,
				"Non-user variable encountered (DUMP)"
			);

		cout << "DUMP: ";
		switch ((*core->variableSpace)[i].type)
		{
			case VAR_STRING: cout << "STRING"; break;
			case VAR_NUMBER: cout << "NUMBER"; break;
			default:
				// Internal error
				return PccError(
					ERR_INTERNAL,
					"Unexpected/unhandled variable type (DUMP): Enum " +
					to_string((*core->variableSpace)[i].type)
				);
			break;
		}

		cout << " " << (*core->variableSpace)[i].ident << " = ";
		if ((*core->variableSpace)[i].type == VAR_STRING) cout << "\"";
		cout << (*core->variableSpace)[i].value;
		if ((*core->variableSpace)[i].type == VAR_STRING) cout << "\"";
		cout << endl;
	}

	return PccError();	// No error
}
