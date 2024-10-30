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

#ifndef Variables_hpp
#define Variables_hpp


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
 * @brief	Load variable with any IDVALs (one or more)
 *
 * Multiple IDVALs will be concatenated as a STRING
 *
 * @param	parser	PCC parser instance pointer
 * @param	core	PCC core instance pointer
 * @param	arg		Token vector
 * @return	PCC error class
 */
PccError CmdVariablesSet(
	PccParser		*parser,
	PccCore			*core,
	vector<token_t>	&arg
);


/**
 * @brief	Show the list of all user defined variables
 *
 * @param	core	PCC core instance pointer
 * @return	PCC error class
 */
PccError CmdVariablesDump(PccCore *core);


#endif // Variables_hpp
