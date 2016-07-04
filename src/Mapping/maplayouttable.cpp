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
#include <AME/Mapping/MapLayoutTable.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude, Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    MapLayoutTable::MapLayoutTable()
        : /*IUndoable(),*/
          m_Offset(0),
          m_Count(-1)
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude, Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    MapLayoutTable::MapLayoutTable(const MapLayoutTable &rvalue)
        : /*IUndoable(),*/
          m_Offset(rvalue.m_Offset),
          m_Count(rvalue.m_Count)
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude, Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    MapLayoutTable &MapLayoutTable::operator=(const MapLayoutTable &rvalue)
    {
        m_Offset = rvalue.m_Offset;
        m_Count = rvalue.m_Count;
        return *this;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Destructor
    // Contributors:   Pokedude, Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    MapLayoutTable::~MapLayoutTable()
    {
        foreach (MapHeader *mapHeader, m_MapHeaders)
            delete mapHeader;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude, Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   7/3/2016
    //
    ///////////////////////////////////////////////////////////
    bool MapLayoutTable::read(const qboy::Rom &rom, UInt32 offset)
    {
        if (!rom.seek(offset))
            AME_THROW(LAY_ERROR_OFFSET, rom.redirected());


        // Checks for ptr validity within the layout header and
        // certain properties in tilesets

        // Retrieves the map count through various checks
        while (true)
        {
            if (!rom.seek(offset + (++m_Count) * 4))
                AME_THROW(LAY_ERROR_WHILE, offset + m_Count * 4);

            unsigned layout = rom.readPointer();
            if (layout == 0)
                continue;

            if (!rom.seek(layout))
                break;

            // Checks if the border, blocks, primary and secondary pointers are valid
            rom.readWord();
            rom.readWord();
            if (!rom.checkOffset(rom.readPointer()))
                break;

            if (!rom.checkOffset(rom.readPointer()))
                break;

            unsigned primary = rom.readPointer();
            if (!rom.checkOffset(primary))
                break;

            unsigned secondary = rom.readPointer();
            if (!rom.checkOffset(secondary))
                break;
/*
            // Checks the first two tileset settings
            rom.seek(primary);
            if (rom.readByte() >= 2) // compression can only be 0/1
                break;
            if (rom.readByte() != 0) // must be primary tileset!
                break;

            rom.seek(secondary);
            if (rom.readByte() >= 2) // compression can only be 0/1
                break;
            if (rom.readByte() != 1) // must be secondary tileset!
                break;*/
        }


        // Now attempts to read all the maps
        for (int i = 0; i < m_Count; i++)
        {
            rom.seek(offset + i * 4);

            // Retrieves the pointer to the map
            MapHeader *mapHeader = new MapHeader;
            UInt32 mapOff = rom.readPointerRef();

            // Attempts to read the map layout
            if (mapOff != 0 && !mapHeader->read(rom, mapOff))
            {
                delete mapHeader;
                return false;
            }

            m_MapHeaders.push_back(mapHeader);
        }


        // Loading successful
        m_Offset = offset;
        return true;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude, Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   7/4/2016
    //
    ///////////////////////////////////////////////////////////
    UInt32 MapLayoutTable::offset() const
    {
        return m_Offset;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude, Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   7/4/2016
    //
    ///////////////////////////////////////////////////////////
    const QList<MapHeader *> &MapLayoutTable::mapHeaders() const
    {
        return m_MapHeaders;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   7/4/2016
    //
    ///////////////////////////////////////////////////////////
    UInt32 MapLayoutTable::count() const
    {
        return m_Count;
    }
}
