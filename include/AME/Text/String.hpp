///////////////////////////////////////////////////////////
//
// QBoy: GameboyAdvance library
// Copyright (C) 2015-2016 Pokedude
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
///////////////////////////////////////////////////////////


#ifndef __AME_STRING_HPP__
#define __AME_STRING_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QBoy/Core/Rom.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    /// \author Pokedude
    /// \date   04/05/2016
    /// \file   String.hpp
    /// \brief  Reads and writes Pokémon strings.
    ///
    /// Currently supports all known escape sequences, such as
    /// buffers or colors and also includes the japanese table.
    ///
    ///////////////////////////////////////////////////////////
    class String {
    public:

        ///////////////////////////////////////////////////////////
        /// \brief Reads a Pokémon string from the given rom.
        ///
        /// Reads at the given offset byte-for-byte and converts
        /// them to readable characters while considering escape
        /// sequences and newline characters.
        ///
        /// \param rom Rom instance to extract string from
        /// \param offset Location of the string to extract
        /// \returns a readable string representation.
        ///
        ///////////////////////////////////////////////////////////
        static const QString read(const qboy::Rom &rom, UInt32 offset);

        ///////////////////////////////////////////////////////////
        // * Write yet to be implemented! *                      //
        //                                                       //
        ///////////////////////////////////////////////////////////
    };
}


#endif  // __AME_STRING_HPP__
