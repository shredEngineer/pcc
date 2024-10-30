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

#include "PccParser.hpp"


PccParser::PccParser(
	int					session_counter,
	vector<variable_t>	*variable_space,
	string				script_filename
)
{
	// Initialize parser for script~ or interactive session

	isInteractive = (script_filename == "");
	isRunning = true;
	isSuccessful = true;
	lineCounter = 0;
	sessionCounter = session_counter;	// Accessed by PccCore
	scriptFilename = script_filename;	// Displayed on syntax error
	Core = new PccCore(variable_space);	// Create PCC core instance
}


PccParser::~PccParser()
{
	// Clean up resources

	delete Core;
}


PccError PccParser::Tokenize(
	string			syntax_line,
	vector<token_t>	&token_vector
)
{
	// Trim whitespace/comment & extract tokens

	// Little trick to get last token more easily
	// (This also prevents the one-character lookahead from ever overflowing)
	syntax_line += " ";

	tokType_t fsmState = TOK_WHITE;

	bool isEscaping = false;
	bool isFloating = false;

	// Reset array bracket matching flag
	bool identHasIndex = false;
	// Reset temporary array index content
	string arrayIndexContent = "";

	string currentToken = "";
	for (size_t cursor = 0; cursor < syntax_line.length(); cursor++)
	{
		char currentChar = syntax_line[cursor];

		switch (fsmState) {


			case TOK_WHITE:
				if (isspace(currentChar))
				{
					// Ignore whitespace
				}

				else if (currentChar == '#')
				{
					// Ignore entire comment
					return PccError();	// No error
				}

				else if (
					(currentChar == '-' || isSingleOp(currentChar)) ||
					(isdigit(currentChar) || currentChar == '.')
				)
				{
					if (currentChar == '-' || isSingleOp(currentChar))
					{
						// Minus sign (-) can begin a number
						// OR can be a single character operator
						// Use a one-character lookahead
						// to decide how it is treated
						char lookahead = syntax_line[cursor + 1];
						if (isspace(lookahead))
						{
							// Expect whitespace after operator

							// Encountered single character operator
							token_t result;
							result.value = currentChar;
							result.type = TOK_SINGLEOP;
							token_vector.push_back(result);
							currentToken = "";	// (Note: Has not been modified)

							// Require at least one whitespace
							// character after token
							fsmState = TOK_INTER;
						}
						else
						{
							// No whitespace allowed after
							// minus sign of negative number

							// Beginning of number
							currentToken += currentChar;
							fsmState = TOK_NUMBER;
						}
					}
					else
					{
						// Beginning of number

						// Add a leading zero for
						// numbers without integer part
						if (currentChar == '.') currentToken += '0';

						currentToken += currentChar;
						fsmState = TOK_NUMBER;
					}
				}

				else if (currentChar == '=')
				{
					// Encountered equals character
					token_t result;
					result.value = currentChar;
					result.type = TOK_EQUALS;
					token_vector.push_back(result);
					currentToken = "";	// (Note: Has not been modified)

					// Require at least one whitespace
					// character after token
					fsmState = TOK_INTER;
				}

				else if (isalpha(currentChar) || currentChar == '_')
				{
					// Beginning of identifier
					currentToken += currentChar;
					fsmState = TOK_IDENT;
				}

				else if (currentChar == '\"')
				{
					// Beginning of string
					// Do not save double quote
					fsmState = TOK_STRING;
				}

				else
				{
					// Syntax error
					return PccError(
						ERR_SYNTAX,
						to_string(cursor) +
						": Unexpected character"
					);
				}
			break;


			case TOK_INTER:
				if (isspace(currentChar))
				{
					fsmState = TOK_WHITE;
				}
				else
				{
					// Syntax error
					return PccError(
						ERR_SYNTAX,
						to_string(cursor) +
						": Expecting whitespace after token"
					);
				}
			break;


			// **** Note: This block also handles  TOK_IDENTARR  distinction ****
			case TOK_IDENT:
				if (!identHasIndex)
				{
					if (isalnum(currentChar) || currentChar == '_')
					{
						// Next character of identifier
						currentToken += currentChar;
					}
					else if (currentChar == '[')
					{
						if (identHasIndex)
						{
							// Syntax error
							return PccError(
								ERR_SYNTAX,
								to_string(cursor) +
								": Unexpected opening array bracket"
							);
						}

						// Next character of identifier: Opening array bracket
						currentToken += currentChar;

						// Set array bracket matching flag
						identHasIndex = true;
					}
					else if (isspace(currentChar))
					{
						// End of identifier
						token_t result;
						result.value = currentToken;
						result.type = TOK_IDENT;
						token_vector.push_back(result);
						currentToken = "";	// Prepare for next token
						fsmState = TOK_WHITE;

						// Reset array bracket matching flag
						identHasIndex = false;
						// Reset temporary array index content
						string arrayIndexContent = "";
					}
					else if (currentChar == '\"')
					{
						// Syntax error
						return PccError(
							ERR_SYNTAX,
							to_string(cursor) +
							": Unexpected double quote in identifier"
						);
					}
					else
					{
						// Syntax error
						return PccError(
							ERR_SYNTAX,
							to_string(cursor) +
							": Identifier may only contain " +
							"alphanumeric characters"
						);
					}
				}
				else
				{
					if (currentChar == ']')
					{
						if (!identHasIndex)
						{
							// Syntax error
							return PccError(
								ERR_SYNTAX,
								to_string(cursor) +
								": Unexpected closing array bracket"
							);
						}

						// Last character of identifier: Closing array bracket
						currentToken += currentChar;

						// Evaluate array index content: Check for empty identifier
						if (arrayIndexContent.size() == 0) {
							// Syntax error
							return PccError(
								ERR_SYNTAX,
								to_string(cursor) +
								": Empty array index not allowed"
							);
						}

						// Closing array bracket enforces end of identifier
						token_t result;
						result.value = currentToken;
						result.type = TOK_IDENTARR;
						token_vector.push_back(result);
						currentToken = "";	// Prepare for next token
						fsmState = TOK_WHITE;

						// Reset array bracket matching flag
						identHasIndex = false;
						// Reset temporary array index content
						string arrayIndexContent = "";
					}
					else
					{
						// **** Note: Only array bracket matching is performed above ****
						// **** ...any other chars inbetween are not evaluated here! ****

						// Next character of identifier: Unfiltered character
						currentToken += currentChar;

						// Extract array index content for further evaluation
						arrayIndexContent += currentChar;
					}
				}
			break;


			case TOK_STRING:
				if (currentChar == '"')
				{
					if (!isEscaping)
					{
						// End of string
						// Do not save double quote
						token_t result;
						result.value = currentToken;
						result.type = TOK_STRING;
						token_vector.push_back(result);
						currentToken = "";	// Prepare for next token

						// Require at least one whitespace
						// character after token
						fsmState = TOK_INTER;
					}
					else
					{
						// Save escaped double quote
						currentToken += currentChar;
						isEscaping = false;	// Esc sequence ends
					}
				}
				else
				{
					if (!isEscaping)
					{
						// Check for escaping character
						if (currentChar == '\\')
						{
							// Do not save escaping character
							isEscaping = true;	// Esc sequence starts
						}
						else
						{
							// Save next character of string
							currentToken += currentChar;
						}
					}
					else
					{
						// Check for valid escaping sequence
						if (currentChar == '\\')
						{
							// Save escaped character
							currentToken += currentChar;
							isEscaping = false;	// Esc sequence ends
						}
						else
						{
							// Syntax error
							return PccError(
								ERR_SYNTAX,
								to_string(cursor) +
								": Undefined escaping sequence"
							);
						}
					}
				}
			break;


			case TOK_NUMBER:
				if (isdigit(currentChar))
				{
					// Next digit of number
					currentToken += currentChar;
				}
				else if (currentChar == '.')
				{
					if (!isFloating)
					{
						// Floating point of number
						currentToken += currentChar;
						isFloating = true;	// Float sequence starts
					}
					else
					{
						// Syntax error
						return PccError(
							ERR_SYNTAX,
							to_string(cursor) +
							": Unexpected multiple floating point"
						);
					}
				}
				else if (isspace(currentChar))
				{
					// End of number
					token_t result;

					result.value = currentToken;	// Save raw string
					result.type = TOK_NUMBER;
					token_vector.push_back(result);
					currentToken = "";	// Prepare for next token

					fsmState = TOK_WHITE;
					isFloating = false;	// Float sequence ends
				}
				else
				{
					// Syntax error
					return PccError(
						ERR_SYNTAX,
						to_string(cursor) +
						": Unexpected non-digit character in number"
					);
				}
			break;


			case TOK_SINGLEOP:
				// We never get here:
				// Single character operators are handled in TOK_WHITE
			break;


			case TOK_EQUALS:
				// We never get here:
				// Equals character is handled in TOK_WHITE
			break;


			default:
				// Internal error
				return PccError(
					ERR_INTERNAL,
					"Unhandled/unexpected token type (Tokenizer): "	\
					"Enum " + to_string(fsmState)
				);
			break;


		}
	}

	if (fsmState == TOK_STRING)
	{
		// Syntax error
		return PccError(
			ERR_SYNTAX,
			"Unterminated string detected"
		);
	}

	if (identHasIndex)
	{
		// Syntax error
		return PccError(
			ERR_SYNTAX,
			"Missing closing array bracket detected"
		);
	}

	return PccError();	// No error
}


void PccParser::Parse(string syntax_line)
{
	// Pass command and argument(s) to PCC core logic

	// Remove trailing CR/LF
	while (
		syntax_line[syntax_line.length() - 1] == '\n' ||
		syntax_line[syntax_line.length() - 1] == '\r'
	)
	{
		syntax_line = syntax_line.substr(0, syntax_line.length() - 1);
	}

	// Tokenize line (all token values are stored as strings)
	vector<token_t> tokenVector;
	PccError tokenizerError = Tokenize(syntax_line, tokenVector);

	if (tokenizerError.hasOccured())
	{
		// Show error message
		tokenizerError.ShowError(this, syntax_line);

		// Immediately exit in script mode
		if (!isInteractive)
		{
			isSuccessful = false;	// Invalidate parser state
			isRunning = false;		// Quit parser
			return;
		}
		else
		{
			// In interactive mode, just do not execute the invalid line
			return;
		}
	}

	// Ignore empty line
	int tokenCount = tokenVector.size();
	if (tokenCount == 0) return;

	// First token (command) must be identifier
	if (tokenVector[0].type != TOK_IDENT)
	{
		// Syntax error
		PccError error(ERR_SYNTAX, "Command expected");
		error.ShowError(this, syntax_line);

		// Immediately exit in script mode
		if (!isInteractive)
		{
			isSuccessful = false;	// Invalidate parser state
			isRunning = false;		// Quit parser
			return;
		}
	}

	// Debug helper function: List each token value and type
	//DbgListTokens(tokenVector);

	// Save command token
	string cmd = tokenVector[0].value;

	// Separate argument tokens
	vector<token_t> arg;
	if (tokenCount > 1)
	{
		tokenVector.erase(tokenVector.begin());
		arg = tokenVector;
	}

	// Commands are case-insensitive,
	// but PccCore::Execute uses UPPERCASE
	for (size_t i = 0; i < cmd.length(); i++)
		cmd[i] = toupper(cmd[i]);

	// Execute command & retrieve error status
	PccError executeError = Core->Execute(this, cmd, arg);

	if (executeError.hasOccured())
	{
		// Show error message
		executeError.ShowError(this, syntax_line);

		// Immediately exit in script mode
		if (!isInteractive)
		{
			isSuccessful = false;	// Invalidate parser state
			isRunning = false;		// Quit parser
			return;
		}
	}

	// Explicit for purpose of better understanding:
	isSuccessful = true;	// (Unchanged since init in constructor)
	return;
}


bool PccParser::isSingleOp(char character)
{
	// Check if single character is operator

	/// TODO: Keep this up-to-date with 'Commands/MathRpn._pp'

	return
		character == '+' ||
		character == '-' ||
		character == '*' ||
		character == '/';
}


varType_t PccParser::TypecastTokToVar(tokType_t token_type)
{
	// Typecast token type to variable type

	switch (token_type)
	{
		case TOK_STRING:
			return VAR_STRING;
		break;

		case TOK_NUMBER:
			return VAR_NUMBER;
		break;

		default:
			// This should NEVER happen
			PccError error(
				ERR_INTERNAL,
				"Unhandled token type (TypecastTokToVar): Enum " +
				to_string(token_type)
			);
			error.ShowError(this);
			return VAR_UNDEF;
		break;
	}
}


void PccParser::DbgListTokens(vector<token_t> token_vector)
{
	// Debug helper function: List each token value and type

	cout << scriptFilename << ": Line " << lineCounter << ":" << endl;
	for (unsigned int i = 0; i < token_vector.size(); i++)
	{
		cout << "Token '" << token_vector[i].value << "' is ";

		switch (token_vector[i].type)
		{
			case TOK_IDENT:
				cout << "IDENT";
			break;

			case TOK_STRING:
				cout << "STRING";
			break;

			case TOK_NUMBER:
				cout << "NUMBER";
			break;

			case TOK_EQUALS:
				cout << "EQUALS";
			break;

			default:
				// This should NEVER happen
				PccError error(
					ERR_INTERNAL,
					"Unhandled token type (DbgListTokens): Enum " +
					to_string(token_vector[i].type)
				);
				error.ShowError(this);
			break;
		}

		cout << endl;
	}
}
