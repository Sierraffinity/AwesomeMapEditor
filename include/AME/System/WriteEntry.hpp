//////////////////////////////////////////////////////////////////////////////////
//
//
//                     d88b         888b           d888  888888888888
//                    d8888b        8888b         d8888  888
//                   d88''88b       888'8b       d8'888  888
//                  d88'  '88b      888 '8b     d8' 888  8888888
//                 d88Y8888Y88b     888  '8b   d8'  888  888
//                d88""""""""88b    888   '8b d8'   888  888
//               d88'        '88b   888    '888'    888  888
//              d88'          '88b  888     '8'     888  888888888888
//
//
// AwesomeMapEditor: A map editor for GBA Pokémon games.
// Copyright (C) 2016 diegoisawesome, Pokedude
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
//////////////////////////////////////////////////////////////////////////////////


#ifndef __AME_WRITEENTRY_HPP__
#define __AME_WRITEENTRY_HPP__


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <QBoy/Config.hpp>
#include <QByteArray>


namespace ame
{
    ///////////////////////////////////////////////////////////
    /// \file    WriteEntry.hpp
    /// \author  Pokedude
    /// \version 1.0.0.0
    /// \date    6/2/2016
    /// \brief   Provides writable data for the ROM.
    ///
    /// Per map, a list of a list of write entries is stored.
    /// This enables easy undo- and redoing without changing
    /// the ROM's data constantly, thus preventing fragmentation.
    /// Write entries consist of an offset and the data itself.
    ///
    ///////////////////////////////////////////////////////////
    struct WriteEntry
    {
        ///////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// Initializes a new instance of ame::WriteEntry. Sets the
        /// data to null and the length to zero.
        ///
        ///////////////////////////////////////////////////////////
        WriteEntry();

        ///////////////////////////////////////////////////////////
        /// \brief Adds a half-word to the data array.
        ///
        ///////////////////////////////////////////////////////////
        void addHWord(UInt16 hword);

        ///////////////////////////////////////////////////////////
        /// \brief Adds a word to the data array.
        ///
        ///////////////////////////////////////////////////////////
        void addWord(UInt32 word);

        ///////////////////////////////////////////////////////////
        /// \brief Adds a pointer to the data array.
        ///
        ///////////////////////////////////////////////////////////
        void addPointer(UInt32 offset);


        ///////////////////////////////////////////////////////////
        /// \brief Defines the data for this write entry.
        ///
        /// The data can be anything - ranging from tilesets, to
        /// map data, to table data. They can either write data or
        /// clear data by writing a bunch of 0xFF's.
        ///
        ///////////////////////////////////////////////////////////
        QByteArray data;

        ///////////////////////////////////////////////////////////
        /// \brief Defines the offset to write the data to.
        ///
        /// The offset specifies where the 'data' should be written
        /// to, without changing offset in the process of writing.
        ///
        ///////////////////////////////////////////////////////////
        UInt32 offset;
    };
}


#endif // __AME_WRITEENTRY_HPP__
