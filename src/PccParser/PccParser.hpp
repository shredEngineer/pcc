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

#ifndef PccParser_hpp
#define PccParser_hpp


#include "Global.hpp"


#include "PccSession/PccSession.hpp"
class PccSession;	// Forward declaration

#include "PccCore/PccCore.hpp"
class PccCore;		// Forward declaration

#include "PccError/PccError.hpp"
class PccError;		// Forward declaration


/**
 * @brief	PCC syntax parser
 */
class PccParser
{

private:

	PccCore	*Core;			/**< Instance pointer */


public:

	bool	isInteractive;	/**< Flag */
	bool	isRunning;		/**< Flag */
	bool	isSuccessful;	/**< Flag */
	int		lineCounter;	/**< Counter */
	int		sessionCounter;	/**< Counter (copy from PccSession) */
	string	scriptFilename;	/**< Filename (copy from PccSession) */


	/**
	 * @brief	Class constructor
	 *
	 * Initialize parser for script~ or interactive session
	 *
	 * @param	session_counter	Session counter
	 * @param	variable_space	Variable space pointer (or NULL)
	 * @param	script_filename	Script filename (or NULL)
	 */
	PccParser(
		int					session_counter,
		vector<variable_t>	*variable_space,
		string script_filename
	);


	/**
	 * @brief	Class destructor
	 *
	 * Clean up resources
	 */
	~PccParser();


	/**
	 * @brief	Tokenize single PCC syntax line
	 *
	 * Trim whitespace/comment & extract tokens
	 *
	 * @param	syntax_line		PCC syntax line
	 * @param	token_vector	Token vector
	 *
	 * @return	PCC error class
	 */
	PccError Tokenize(
		string			syntax_line,
		vector<token_t>	&token_vector
	);


	/**
	 * @brief	Parse single PCC line
	 *
	 * Pass command and arguments to PCC core for execution
	 *
	 * @param	syntax_line	PCC syntax line
	 */
	void Parse(string syntax_line);


	/**
	 * @brief	Check if single character is operator
	 *
	 * Used in Tokenizer
	 *
	 * @param	character	Character
	 * @return	true or false
	 */
	bool isSingleOp(char character);


	/**
	 * @brief	Typecast token type to variable type
	 *
	 * Note that this will only cout a message on error
	 * and further program execution may be undefined!
	 *
	 * @param	token_type	Token type
	 * @return	Variable type
	 */
	varType_t TypecastTokToVar(tokType_t token_type);


	/**
	 * @brief	Debug helper function
	 *
	 * List each token value and type
	 *
	 * @param	token_vector	Token vector
	 */
	void DbgListTokens(vector<token_t> token_vector);

};


#endif // PccParser_hpp
