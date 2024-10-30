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

#include "MathRpn.hpp"


PccError CmdMathRpnSolve(PccCore *core, vector<token_t> &arg)
{
	// Execute RPN calculation

	if (arg[0].type != TOK_IDENT || arg[0].type == TOK_IDENTARR)
	{
		// Syntax error
		return PccError(
			ERR_SYNTAX,
			"Identifier expected as 1. argument"
		);
	}

	string leftIdent = arg[0].value;

	if (arg[1].type != TOK_EQUALS)
	{
		// Syntax error
		return PccError(
			ERR_SYNTAX,
			"Expected '='"
		);
	}

	// Parse RPN
	vector<string> stack;

	for (size_t i = 2; i < arg.size(); i++)
	{
		if (arg[i].type == TOK_NUMBER)
		{
			stack.push_back(arg[i].value);
		}
		else if (arg[i].type == TOK_IDENT || arg[i].type == TOK_IDENTARR)
		{
			// Dereference variable
			variable_t* variable = core->VariableFind(arg[i].value);

			if (variable == NULL)
			{
				// Runtime error
				return PccError(
					ERR_RUNTIME,
					"Attempting to read undefined variable "	\
					"(" + arg[i].value + ")"
				);
			}

			if (variable->type != VAR_NUMBER)
			{
				// Runtime error
				return PccError(
					ERR_RUNTIME,
					"Variable not of type number "	\
					"(" + arg[i].value + ")"
				);
			}

			stack.push_back(variable->value);
		}
		else if (arg[i].type == TOK_SINGLEOP)
		{
			/// TODO: Keep this up-to-date with 'PccParser::isSingleOp'

			if (stack.size() < 2)
			{
				return PccError(
					ERR_RUNTIME,
					"Not enough values on RPN stack "	\
					"(" + arg[i].value + " requires 2 operands, " + to_string(stack.size()) + " available)"
				);
			}

			double B = stod(stack.back());
			stack.pop_back();

			double A = stod(stack.back());
			stack.pop_back();

			if (arg[i].value == "+")
			{
				stack.push_back(DoubleToStr(A + B));
			}
			else if (arg[i].value == "-")
			{
				stack.push_back(DoubleToStr(A - B));
			}
			else if (arg[i].value == "*")
			{
				stack.push_back(DoubleToStr(A * B));
			}
			else if (arg[i].value == "/")
			{
				if (B == 0)
				{
					return PccError(
						ERR_RUNTIME,
						"Attempted division by zero"
					);
				}
				stack.push_back(DoubleToStr(A / B));
			}
			else
			{
				// This should NEVER happen
				return PccError(
					ERR_INTERNAL,
					"TOK_SINGLEOP not handled in CmdMathRpnSolve"
				);
			}
		}
		else
		{
			return PccError(
				ERR_RUNTIME,
				"REFNUM expected (number or variable of type number)"
			);
		}
	}

	if (stack.empty())
	{
		return PccError(
			ERR_RUNTIME,
			"Calculation failed - Empty RPN stack "
		);
	}

	if (stack.size() > 1)
	{
		string stackDump = "";
		for (size_t i = 1; i < stack.size(); i++)
		{
			stackDump += stack[i] + (i < stack.size() - 1 ? ", " : "");
		}

		return PccError(
			ERR_RUNTIME,
			"Calculation failed - Multiple values left over on RPN stack " \
			"(" + stackDump + ")"
		);
	}

	string resultValue = stack[0];
	varType_t resultType = VAR_NUMBER;

	// Save result
	variable_t *variable = core->VariableFind(leftIdent);

	if (variable == NULL)
	{
		// Create new variable
		core->VariableCreate(
			leftIdent,
			resultValue,
			resultType,
			MODE_USER
		);
	}
	else
	{
		if (variable->mode != MODE_USER)
			// Runtime error
			return PccError(
				ERR_RUNTIME,
				"Attempting to write non-user variable "	\
				"(" + leftIdent + ")"
			);

		// Modify variable
		core->VariableModify(
			variable,
			resultValue,
			resultType,
			MODE_USER
		);
	}

	return PccError();	// No error
}
