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

#include "Global.hpp"


string DoubleToStr(double value)
{
	// Convert floating point number
	// to string with 15 significant figures

	stringstream ss;
	ss << fixed << setprecision(15) << value;
	return ss.str();
}


string FormatAutoInt(string value)
{
	// Detect integer number and format accordingly

	double number_double = stod(value);
	int number_int = number_double;
	double number_fract = number_double - number_int;

	// Avoiding abs() because MinGW 4.9.3 seems to be broken
	number_fract = number_fract < 0 ? -number_fract : number_fract;

	if (number_fract >= 0.000001)
	{
		stringstream ss;
		ss << fixed << setprecision(6) << number_double;
		return ss.str();
	}
	else
	{
		return to_string(number_int);
	}
}


string GetTime()
{
	time_t now = time(0);
	struct tm ts;
	char buffer[80];
	ts = *localtime(&now);
	strftime(buffer, sizeof(buffer), "%X", &ts);
	return buffer;
}


string GetDate()
{
	time_t now = time(0);
	struct tm ts;
	char buffer[80];
	ts = *localtime(&now);
	strftime(buffer, sizeof(buffer), "%Y-%m-%d", &ts);
	return buffer;
}
