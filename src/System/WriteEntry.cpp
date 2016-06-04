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


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <AME/System/WriteEntry.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/2/2016
    // Comment:
    //
    // Set offset to zero; data is null by default
    //
    ///////////////////////////////////////////////////////////
    WriteEntry::WriteEntry(UInt32 offset)
        : offset(offset)
    {
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/3/2016
    //
    ///////////////////////////////////////////////////////////
    void WriteEntry::addHWord(UInt16 hword)
    {
        data.push_back((Int8)(hword & 0xFF));
        data.push_back((Int8)(hword >> 0x8));
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/3/2016
    //
    ///////////////////////////////////////////////////////////
    void WriteEntry::addWord(UInt32 word)
    {
        data.push_back((Int8)(word & 0xFF));
        data.push_back((Int8)(word >> 0x08));
        data.push_back((Int8)(word >> 0x10));
        data.push_back((Int8)(word >> 0x18));
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Setter
    // Contributers:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/3/2016
    //
    ///////////////////////////////////////////////////////////
    void WriteEntry::addPointer(UInt32 offset)
    {
        if (offset == 0x00000000)
            addWord(offset);
        else
            addWord(offset + 0x08000000);
    }
}
