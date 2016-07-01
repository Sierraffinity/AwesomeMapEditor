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
// Copyright (C) 2016 Diegoisawesome, Pokedude
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
#include <AME/System/Configuration.hpp>
#include <AME/Mapping/MappingErrors.hpp>
#include <AME/Mapping/MapNameTable.hpp>
#include <AME/Text/String.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   7/1/2016
    //
    ///////////////////////////////////////////////////////////
    MapNameTable::MapNameTable()
        : /*IUndoable(),*/
          m_Offset(0)
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   7/1/2016
    //
    ///////////////////////////////////////////////////////////
    MapNameTable::MapNameTable(const MapNameTable &rvalue)
        : /*IUndoable(),*/
          m_Offset(rvalue.m_Offset)
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   7/1/2016
    //
    ///////////////////////////////////////////////////////////
    MapNameTable &MapNameTable::operator=(const MapNameTable &rvalue)
    {
        m_Offset = rvalue.m_Offset;
        return *this;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Destructor
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   7/1/2016
    //
    ///////////////////////////////////////////////////////////
    MapNameTable::~MapNameTable()
    {
        foreach (MapName *name, m_Names)
            delete name;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   7/1/2016
    //
    ///////////////////////////////////////////////////////////
    bool MapNameTable::read(const qboy::Rom &rom, UInt32 offset)
    {
        if (!rom.seek(offset))
            AME_THROW(MBT_ERROR_OFFSET, rom.redirected());

        // Reads all the name index info
        for (int i = 0; i < (int)CONFIG(MapNameCount); i++)
        {
            MapName *name = new MapName();

            // Determines the map name position
            if (CONFIG(RomType) == RT_FRLG)
                rom.seek(offset + i*4);
            else
                rom.seek(offset + i*8 + 4);

            unsigned ptrName = rom.readPointerRef();
            if (!rom.checkOffset(ptrName))
                AME_THROW(MAP_ERROR_NAME, rom.redirected());

            // Reads the map name string
            name->name = String::read(rom, ptrName);

            m_Names.push_back(name);
        }


        // Loading successful
        m_Offset = offset;
        return true;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/15/2016
    //
    ///////////////////////////////////////////////////////////
    UInt32 MapNameTable::offset() const
    {
        return m_Offset;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/15/2016
    //
    ///////////////////////////////////////////////////////////
    const QList<MapName *> &MapNameTable::names() const
    {
        return m_Names;
    }
}
