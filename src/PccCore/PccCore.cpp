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

#include "PccCore.hpp"


PccCore::PccCore(vector<variable_t> *variable_space)
{
	// Initialize PCC core functionality

	if (variable_space == NULL)
	{
		// Allocate own variable space
		variableSpace = new vector<variable_t>;
		isOwnVarSpace = true;

		// Predefined constants
		string newline;
		#ifdef _WIN32
		newline = "\r\n";
		#else
		newline = "\n";
		#endif
		VariableCreate("ENDL", newline, VAR_STRING, MODE_PREDEF_CONST);

		// Predefined variables (dynamically updated by VariableFind())
		VariableCreate(
			"TIME",
			"undefined",
			VAR_STRING,
			MODE_PREDEF_VAR
		);
		VariableCreate(
			"DATE",
			"undefined",
			VAR_STRING,
			MODE_PREDEF_VAR
		);
	}
	else
	{
		// Assign external variable space
		variableSpace = variable_space;
		isOwnVarSpace = false;
	}

	// Find beginning of user variables
	for (
		userVarBegin = 0;
		userVarBegin < variableSpace->size();
		userVarBegin++
	)
	{
		if ((*variableSpace)[userVarBegin].mode == MODE_USER) break;
	}
}


PccCore::~PccCore()
{
	// Clean up resources

	if (isOwnVarSpace)
		delete variableSpace;
}


// **** Variable names are case sensitive! ****

variable_t* PccCore::VariableFind(string ident)
{
	// Search variable in variable space

	for (size_t i = 0; i < variableSpace->size(); i++)
	{
		// Check if identifier exists
		if ((*variableSpace)[i].ident == ident)
		{
			if ((*variableSpace)[i].mode == MODE_PREDEF_VAR)
			{
				if (ident == "TIME")
				{
					(*variableSpace)[i].value = GetTime();
				}
				else if (ident == "DATE")
				{
					(*variableSpace)[i].value = GetDate();
				}
				else
				{
					// Internal error
					PccError error(
						ERR_INTERNAL,
						"No dynamic loader specified for identifier: " +
						ident
					);
					error.ShowError();
				}
			}

			return &(*variableSpace)[i];
		}
	}

	return NULL;	// Variable not existing
}


void PccCore::VariableCreate(
	string		ident,
	string		value,
	varType_t	type,
	varMode_t	mode
)
{
	// Create new variable in variable space
	// Note that this does NOT check if the variable already exists!
	// This increases performance but could, in principle and if used
	// incorrectly by the developer, lead to unintended behaviour due
	// to multiple variable instances with identical IDs. VariableFind
	// will, however, always use the variable with the lowest index.

	// Create new variable
	variable_t newVariable;
	newVariable.ident = ident;
	newVariable.value = value;
	newVariable.type = type;
	newVariable.mode = mode;
	variableSpace->push_back(newVariable);
}


void PccCore::VariableModify(
	variable_t	*variable,
	string		value,
	varType_t	type,
	varMode_t	mode
)
{
	// Modify variable in variable space; note that a
	// pointer to an ALREADY existing variable must be passed!

	if (variable == NULL)
	{
		PccError error(
			ERR_INTERNAL,
			"Attempting to modify NULL variable"
		);
		error.ShowError();
	}

	// Modify variable
	variable->value = value;
	variable->type = type;
	variable->mode = mode;
}
