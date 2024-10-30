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

#include "Main.hpp"


/**
 * @brief	Main function.
 *
 * @param   argc	Argument count
 * @param	argv	Argument values
 *
 * @return	EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char *argv[])
{
	// Show application header
	cout << "PCC " << VERSION;
	cout << " Copyright (C) 2016-2017 by Paul Wilhelm <anfrage@paulwilhelm.de>"			<< endl;
	cout << "This program comes with ABSOLUTELY NO WARRANTY. This is free software,"	<< endl;
	cout << "and you are welcome to redistribute it under certain conditions."			<< endl << endl;


	// Check CLI arguments
	bool isInteractive;
	if (argc == 1)
	{
		isInteractive = true;
	}
	else if (argc == 2)
	{
		isInteractive = false;
	}
	else
	{
		// Exit application with error
		cout << "Usage: pcc [script.pcc]" << endl;
		exit(EXIT_FAILURE);
	}


	// Initialize session
	int sessionCounter = 0;

	bool isSuccessful;

	if (isInteractive)
	{
		// Initialize interactive session
		PccSession Session(sessionCounter, NULL, "");

		// Start interactive session
		isSuccessful = Session.StartInteractive();
	}
	else
	{
		// Initialize script session
		string scriptFilename = argv[1];
		PccSession Session(sessionCounter, NULL, scriptFilename);

		// Check if script was loaded
		if (!Session.scriptStream)
		{
			// Exit application with error
			cout << "ERROR: Could not open file" << endl;
			exit(EXIT_FAILURE);
		}

		// Start script session
		isSuccessful = Session.StartScript();
	}


	// Exit application
	exit(isSuccessful ? EXIT_SUCCESS : EXIT_FAILURE);
}
