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

#include "PccSession.hpp"


PccSession::PccSession(
	int					session_counter,
	vector<variable_t>	*variable_space,
	string				script_filename
)
{
	// Initialize script~ or interactive session

	sessionParser = new PccParser(
		session_counter,
		variable_space,
		script_filename
	);

	sessionCounter = session_counter;
	scriptFilename = script_filename;

	isInteractive = (script_filename == "");

	if (isInteractive)
	{
		cout << "Interactive session";

		if (sessionCounter > 0) cout << " (" << sessionCounter << ")";

		cout << ": Try 'help' or 'quit'" << endl;
	}
	else
	{
		scriptStream.open(scriptFilename);

		if (scriptStream)	// Suppress message if script did not load properly
		{
			cout << "Script session: " << scriptFilename << endl;
		}
	}
}


PccSession::~PccSession()
{
	// Close session & clean up resources

	if (isInteractive)
	{
		cout << "Interactive session";

		if (sessionCounter > 0)
		{
			cout << " (" << sessionCounter << ") done." << endl << endl;
		}
		else
		{
			if (scriptStream)	// Suppress message if script did not load properly
			{
				cout << " done." << endl;
			}
		}
	}
	else
	{
		cout << "Script session done." << endl;

		scriptStream.close();
	}

	delete sessionParser;
}


bool PccSession::StartInteractive()
{
	// Query and parse lines until quit

	while (sessionParser->isRunning)
	{
		string syntax_line;
		cout << "> ";
		getline(cin, syntax_line);

		sessionParser->Parse(syntax_line);
	}

	return sessionParser->isSuccessful;
}


bool PccSession::StartScript()
{
	// Read and parse lines until quit or EOF

	while (sessionParser->isRunning && scriptStream)
	{
		string syntax_line;
		getline(scriptStream, syntax_line);

		sessionParser->lineCounter++;
		sessionParser->Parse(syntax_line);
	}

	return sessionParser->isSuccessful;
}
