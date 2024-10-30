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

#ifndef PccSession_hpp
#define PccSession_hpp


#include "Global.hpp"


#include "PccParser/PccParser.hpp"
class PccParser;	// Forward declaration


/**
 * @brief	PCC session management
 */
class PccSession
{

private:

	bool		isInteractive;	/**< Flag */
	int			sessionCounter;	/**< Counter */
	string		scriptFilename;	/**< Filename */

	PccParser	*sessionParser;	/** Instance pointer */


public:

	ifstream			scriptStream;	/**< Stream */
	vector<variable_t>	*variableSpace;	/**< Vector pointer */


	/**
	 * @brief	Class constructor
	 *
	 * Initialize script~ or interactive session
	 *
	 * @param	session_counter	Session counter
	 * @param	variable_space	Variable space pointer
	 * @param	script_filename	Script filename
	 */
	PccSession(
		int					session_counter,
		vector<variable_t>	*variable_space,
		string				script_filename
	);


	/**
	 * @brief	Class destructor
	 *
	 * Close session & clean up resources
	 */
	~PccSession();


	/**
	 * @brief	Start interactive session
	 *
	 * Query and parse lines until quit
	 *
	 * @return	isSuccessful flag
	 */
	bool StartInteractive();


	/**
	 * @brief	Start script session
	 *
	 * Read and parse lines until quit or EOF
	 *
	 * @return	isSuccessful flag
	 */
	bool StartScript();

};


#endif // PccSession_hpp
