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
#include <AME/Mapping/MapBank.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/15/2016
    //
    ///////////////////////////////////////////////////////////
    MapBank::MapBank()
        : /*IUndoable(),*/
          m_Offset(0),
          m_Count(-1)
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/15/2016
    //
    ///////////////////////////////////////////////////////////
    MapBank::MapBank(const MapBank &rvalue)
        : /*IUndoable(),*/
          m_Offset(rvalue.m_Offset),
          m_Count(rvalue.m_Count)
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/15/2016
    //
    ///////////////////////////////////////////////////////////
    MapBank &MapBank::operator=(const MapBank &rvalue)
    {
        m_Offset = rvalue.m_Offset;
        m_Count = rvalue.m_Count;
        return *this;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Destructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/15/2016
    //
    ///////////////////////////////////////////////////////////
    MapBank::~MapBank()
    {
        foreach (Map *map, m_Maps)
            delete map;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/15/2016
    //
    ///////////////////////////////////////////////////////////
    bool MapBank::read(const qboy::Rom &rom, UInt32 offset, UInt32 next)
    {
        if (!rom.seek(offset))
            AME_THROW(BNK_ERROR_OFFSET, rom.redirected());


        // TODO: Setting >> speed vs accuracy
        // User can choose whether to retrieve the map-count
        // on a fast or accurate way (horiz. slider)
        // Level 0: Checks for next bank and ptr validity +
        // Level 1: Checks for ptr validity within map +
        // Level 2: Checks for ptr validity within the header +
        // Level 3: Checks for ptr validity within the events +
        // Level 4: Checks certain properties in tilesets and events
        int accLevel = 4; // = CONFIG(MapAccuracy);

        // Retrieves the map count through various checks
        while (true)
        {
            if (!rom.seek(offset + (++m_Count) * 4))
                AME_THROW(BNK_ERROR_WHILE, offset + m_Count * 4);

            // =============  LEVEL 0  =============
            // Checks if current offset is start of bank-table
            if (rom.offset() == CONFIG(MapBanks))
                break;

            // Checks if current offset is next bank
            if (rom.offset() == next)
                break;

            // Checks if map pointer is valid
            unsigned map = rom.readPointer();
            if (!rom.seek(map))
                break;


            // =============  LEVEL 1  =============
            if (accLevel == 0)
                continue;

            // Checks if the footer and event pointers are valid
            unsigned footer = rom.readPointer();
            if (!rom.checkOffset(footer))
                break;

            unsigned events = rom.readPointer();
            if (!rom.checkOffset(events))
                break;


            // =============  LEVEL 2  =============
            if (accLevel == 1)
                continue;

            // Checks if the border, blocks, primary and secondary pointers are valid
            rom.seek(footer);
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


            // =============  LEVEL 3  =============
            if (accLevel == 2)
                continue;

            // Checks if the npc, warp, trigger and sign pointers are valid
            rom.seek(events);
            rom.readWord();
            if (!rom.checkOffset(rom.readPointer()))
                break;

            if (!rom.checkOffset(rom.readPointer()))
                break;

            if (!rom.checkOffset(rom.readPointer()))
                break;

            if (!rom.checkOffset(rom.readPointer()))
                break;


            // =============  LEVEL 4  =============
            if (accLevel == 3)
                continue;

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
                break;
        }


        // Now attempts to read all the maps
        for (unsigned i = 0; i < m_Count; i++)
        {
            rom.seek(offset + i * 4);

            // Retrieves the pointer to the map
            Map *map = new Map;
            UInt32 mapOff = rom.readPointerRef();

            // Attempts to read the map
            if (!map->read(rom, mapOff))
            {
                delete map;
                return false;
            }

            m_Maps.push_back(map);
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
    UInt32 MapBank::offset() const
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
    const QList<Map *> &MapBank::maps() const
    {
        return m_Maps;
    }
}
