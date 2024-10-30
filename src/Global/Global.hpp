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

#ifndef Global_hpp
#define Global_hpp


#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>
#include <string>
#include <time.h>
#include <vector>

using namespace std;


// Add your custom headers Here



/**
 * @brief	Tokenizer FSM state / Token type
 */
enum tokType_t
{
	TOK_WHITE		= 0,	/**< Whites. or 1st char of current token */
	TOK_INTER		= 1,	/**< Whitespace between tokens */
	TOK_IDENT		= 2,	/**< Currently parsing identifier */
	TOK_IDENTARR	= 3,	/**< Currently parsing identifier with array index */
	TOK_STRING		= 4,	/**< Currently parsing string */
	TOK_NUMBER		= 5,	/**< Currently parsing number */
	TOK_SINGLEOP	= 6,	/**< Encountered single char operator */
	TOK_EQUALS		= 7		/**< Encountered equals character */
};


/**
 * @brief	PCC variable type
 */
enum varType_t
{
	VAR_UNDEF	= 0,	/**< Undefined type */
	VAR_STRING	= 1,	/**< String type */
	VAR_NUMBER	= 2		/**< Number type */
};


/**
 * @brief	PCC variable mode
 */
enum varMode_t
{
	MODE_PREDEF_CONST	= 0,	/**< Predefined constant */
	MODE_PREDEF_VAR		= 1,	/**< Predefined variable */
	MODE_USER			= 2		/**< User variable */
};


/**
 * @brief	PCC token structure
 */
struct token_t
{
	string value;
	tokType_t type;
};


/**
 * @brief	PCC variable structure
 */
struct variable_t
{
	string ident;	/**< Variable identifier */
	string value;	/**< Variable value */
	varType_t type;	/**< Variable type */
	varMode_t mode;	/**< Variable mode */
};


/**
 * @brief	Convert float number to string with 15 significant figures
 *
 * @param	value	Floating point value
 * @return	String representation
 */
string DoubleToStr(double value);


/**
 * @brief	Detect integer number and format accordingly
 *
 * @param	value	Number string
 * @return	Formatted string
 */
string FormatAutoInt(string value);


/**
 * @brief	Get current time
 *
 * @return	String
 */
string GetTime();


/**
 * @brief	Get current date
 *
 * @return	String
 */
string GetDate();


#endif // Global_hpp
