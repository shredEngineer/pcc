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

#ifndef PccError_hpp
#define PccError_hpp


#include "Global.hpp"


#include "PccParser/PccParser.hpp"
class PccParser;	// Forward declaration


/**
 * @brief	PCC error type
 *
 * This enum is shared with PccCore & PccParser
 */
enum errType_t
{
	ERR_NONE		= 0,	/**< No error */
	ERR_INTERNAL	= 1,	/**< Internal error */
	ERR_SYNTAX		= 2,	/**< Syntax error */
	ERR_RUNTIME		= 3		/**< Runtime error */
};


/**
 * @brief	PCC error class
 */
class PccError
{

private:


public:

	errType_t	errorType;		/**< Error type */
	string		errorMessage;	/**< Error message */


	/**
	 * @brief	Class constructor
	 *
	 * No error
	 */
	PccError();


	/**
	 * @brief	Class constructor
	 *
	 * Set error type~ & message
	 *
	 * @param	error_type		Error type
	 * @param	error_message	Error message
	 */
	PccError(errType_t error_type, string error_message);


	/**
	 * @brief	Helper function
	 *
	 * @return	true if errorType != ERR_NONE
	 */
	bool hasOccured();


	/**
	 * @brief	Show error message
	 *
	 * Show error message (without any fancy extra information)
	 */
	void ShowError();


	/**
	 * @brief	Show error message
	 *
	 * Show error message (in script session also filename & line no.)
	 *
	 * @param	parser		PCC parser instance pointer
	 */
	void ShowError(PccParser *parser);


	/**
	 * @brief	Show error message
	 *
	 * Show error message & corresponding line
	 * (in script session also filename & line no.)
	 *
	 * @param	parser		PCC parser instance pointer
	 * @param	syntax_line	PCC syntax line
	 */
	void ShowError(PccParser *parser, string syntax_line);


	/**
	 * @brief	Helper function
	 *
	 * Show string corresponding to error type
	 */
	void ShowErrTypeStr();

};

#endif // PccError_hpp
