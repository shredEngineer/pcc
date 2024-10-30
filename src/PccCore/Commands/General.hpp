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

#ifndef General_hpp
#define General_hpp


#include "Global.hpp"


#include "PccSession/PccSession.hpp"
class PccSession;	// Forward declaration

#include "PccParser/PccParser.hpp"
class PccParser;	// Forward declaration

#include "PccCore/PccCore.hpp"
class PccCore;		// Forward declaration

#include "PccError/PccError.hpp"
class PccError;		// Forward declaration


// Forward declarations:

/**
 * @brief	Show the list of all commands
 */
void CmdGeneralHelp();


/**
 * @brief	Start nested interactive session (useful for debugging)
 *
 * @param	parser	PCC parser instance pointer
 * @param	core	PCC core instance pointer
 */
void CmdGeneralUser(
	PccParser *parser,
	PccCore *core
);


/**
 * @brief	Start script session from .pcc file
 *
 * @param	parser	PCC parser instance pointer
 * @param	core	PCC core instance pointer
 * @param	arg		Token vector
 * @return	PCC error class
 */
PccError CmdGeneralStart(
	PccParser *parser,
	PccCore *core,
	vector<token_t> &arg
);


#endif // General_hpp
