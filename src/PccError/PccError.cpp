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

#include "PccError.hpp"


PccError::PccError()
{
	// No error

	errorType = ERR_NONE;
	errorMessage = "";
}


PccError::PccError(errType_t error_type, string error_message)
{
	// Set error type~ & message

	errorType = error_type;
	errorMessage = error_message;
}


bool PccError::hasOccured()
{
	// Helper function

	return (errorType != ERR_NONE);
}


void PccError::ShowError()
{
	// Show error message (without any fancy extra information)

	ShowErrTypeStr();

	cout << errorMessage << endl;
}


void PccError::ShowError(PccParser *parser)
{
	// Show error message (in script session also filename & line number)

	ShowErrTypeStr();

	if (!parser->isInteractive)
	{
		cout << parser->scriptFilename << ": ";
		cout << "Line " << parser->lineCounter << ": ";
	}

	cout << errorMessage << endl;
}


void PccError::ShowError(PccParser *parser, string syntax_line)
{
	// Show error message & corresponding line
	// and in script session also filename & line number

	ShowErrTypeStr();

	if (!parser->isInteractive)
	{
		cout << parser->scriptFilename << ": ";
		cout << "Line " << parser->lineCounter << ": ";
	}

	cout << errorMessage << ": '" << syntax_line << "'" << endl;
}


void PccError::ShowErrTypeStr()
{
	// Helper function

	switch (errorType)
	{
		case ERR_NONE:
			// This should never occur
			cout << "[No error]: ";
		break;

		case ERR_INTERNAL:
			cout << "INTERNAL ERROR: ";
		break;

		case ERR_SYNTAX:
			cout << "SYNTAX ERROR: ";
		break;

		case ERR_RUNTIME:
			cout << "RUNTIME ERROR: ";
		break;

		default:
			// This should never occur
			cout << "[Undefined error] ";
		break;
	}
}
