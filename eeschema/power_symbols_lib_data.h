/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2015 Chris Pavlina <pavlina.chris@gmail.com>
 * Copyright (C) 1992-2015 KiCad Developers, see AUTHORS.txt for contributors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

/**
 * @file power_symbols_lib_data.h
 * @brief Resource header for power_symbols.lib
 */

#ifndef POWER_SYMBOLS_LIB_DATA_H
#define POWER_SYMBOLS_LIB_DATA_H

#include <string>

/**
 * A library of power symbol styles, embedded into the object.
 *
 * The data is in power_symbols_lib_data.cpp; see that file for instructions on updating it.
 */
extern const std::string PowerSymbolLibData;

/**
 * The same library as a character array.
 */
extern const char RawPowerSymbolLibData[];

#endif // POWER_SYMBOLS_LIB_DATA_H
