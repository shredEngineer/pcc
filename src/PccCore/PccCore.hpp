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

#ifndef PccCore_hpp
#define PccCore_hpp


#include "Global.hpp"


#include "PccCore/Execute.hpp"


#include "PccSession/PccSession.hpp"
class PccSession;	// Forward declaration

#include "PccParser/PccParser.hpp"
class PccParser;	// Forward declaration

#include "PccError/PccError.hpp"
class PccError;		// Forward declaration


/**
 * @brief	PCC core functionality
 */
class PccCore
{

private:


public:

	vector<variable_t>
		*variableSpace;		/**< Vector pointer */

	bool	isOwnVarSpace;	/**< Flag */

	size_t	userVarBegin;	/**< Offset */


	/**
	 * @brief	Class constructor
	 *
	 * Initialize PCC core functionality
	 *
	 * @param	variable_space	Variable space pointer
	 */
	PccCore(vector<variable_t> *variable_space);


	/**
	 * @brief	Class destructor
	 *
	 * Clean up resources
	 */
	~PccCore();


	/**
	 * @brief	Find PCC variable
	 *
	 * Search variable in variable space
	 *
	 * @param	ident	Variable identifier
	 * @return	Pointer to PCC variable structure (NULL if not existing)
	 */
	variable_t* VariableFind(string ident);


	/**
	 * @brief	Create PCC variable
	 *
	 * Create new variable in variable space
	 *
	 * @param	ident	Variable identifier
	 * @param	value	Variable value
	 * @param	type	Variable type
	 * @param	mode	Variable mode
	 */
	void VariableCreate(
		string		ident,
		string		value,
		varType_t	type,
		varMode_t	mode
	);


	/**
	 * @brief	Modify PCC variable
	 *
	 * Modify variable in variable space
	 *
	 * @param	variable	Variable pointer
	 * @param	value		New variable value
	 * @param	type		New variable type
	 * @param	mode		Variable mode
	 */
	void VariableModify(
		variable_t	*variable,
		string		value,
		varType_t	type,
		varMode_t mode
	);


	/**
	 * @brief	Execute single PCC command
	 *
	 * Process command/arguments & execute related logic.
	 * Note that this function is outsourced to Execute.cpp.
	 *
	 * @param	parser		Parser instance
	 * @param	cmd			Command
	 * @param	arg			Argument vector
	 * @return	PCC error class
	 */
	PccError Execute(
		PccParser *parser,
		string cmd,
		vector<token_t> arg
	);

};


#endif // PccCore_hpp
