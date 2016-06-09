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
#include <AME/Structures/StructureErrors.hpp>
#include <AME/Structures/WildPokemonSubTable.hpp>


namespace ame
{
    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/3/2016
    //
    ///////////////////////////////////////////////////////////
    WildPokemonSubTable::WildPokemonSubTable()
        : m_Offset(0),
          m_Bank(0),
          m_Map(0),
          m_PtrGrass(0),
          m_PtrWater(0),
          m_PtrRock(0),
          m_PtrFish(0)
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/3/2016
    //
    ///////////////////////////////////////////////////////////
    WildPokemonSubTable::WildPokemonSubTable(const WildPokemonSubTable &rvalue)
        : m_Offset(rvalue.m_Offset),
          m_Bank(rvalue.m_Bank),
          m_Map(rvalue.m_Map),
          m_PtrGrass(rvalue.m_PtrGrass),
          m_PtrWater(rvalue.m_PtrWater),
          m_PtrRock(rvalue.m_PtrRock),
          m_PtrFish(rvalue.m_PtrFish)
    {
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Constructor
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/3/2016
    //
    ///////////////////////////////////////////////////////////
    WildPokemonSubTable &WildPokemonSubTable::operator=(const WildPokemonSubTable &rvalue)
    {
        m_Offset = rvalue.m_Offset;
        m_Bank = rvalue.m_Bank;
        m_Map = rvalue.m_Map;
        m_PtrGrass = rvalue.m_PtrGrass;
        m_PtrWater = rvalue.m_PtrWater;
        m_PtrRock = rvalue.m_PtrRock;
        m_PtrFish = rvalue.m_PtrFish;
        return *this;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/3/2016
    //
    ///////////////////////////////////////////////////////////
    bool WildPokemonSubTable::read(const qboy::Rom &rom, UInt32 offset)
    {
        if (!rom.seek(offset))
            AME_THROW(WPST_ERROR_OFFSET, rom.redirected());

        // Fetches the corresponding map index at a specific bank
        m_Bank = rom.readByte();
        m_Map = rom.readByte();
        rom.readHWord(); /* Padding */


        // Reads all pointers to each area
        m_PtrGrass = rom.readPointer();
        m_PtrWater = rom.readPointer();
        m_PtrRock = rom.readPointer();
        m_PtrFish = rom.readPointer();


        // Determines whether all read pointers are valid
        if (!rom.checkOffset(m_PtrGrass))
            AME_THROW(WPST_ERROR_GRASS, offset + 4);
        if (!rom.checkOffset(m_PtrWater))
            AME_THROW(WPST_ERROR_WATER, offset + 8);
        if (!rom.checkOffset(m_PtrRock))
            AME_THROW(WPST_ERROR_ROCK, offset + 12);
        if (!rom.checkOffset(m_PtrFish))
            AME_THROW(WPST_ERROR_FISH, offset + 16);


        // Defines objects for reading the encounter tables dynamically
        WildPokemonArea* areas[] { &m_AreaGrass, &m_AreaWater, &m_AreaRock, &m_AreaFish };
        UInt32 pointers[] { m_PtrGrass, m_PtrWater, m_PtrRock, m_PtrFish };
        UInt32 entrycnt[] { 12, 5, 5, 10 };


        // Reads each encounter table dynamically
        for (int i = 0; i < 4; i++)
        {
            WildPokemonArea *currentArea = areas[i];
            UInt32 currentOffset = pointers[i];
            UInt32 currentAmount = entrycnt[i];

            // If pointer is a NULL pointer, abort reading
            if (!currentOffset)
                continue;

            // Reads the current area
            if (!currentArea->read(rom, currentOffset, currentAmount))
                return false;
        }


        // Loading successful
        m_Offset = offset;
        return true;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/4/2016
    //
    ///////////////////////////////////////////////////////////
    QList<WriteEntry> WildPokemonSubTable::write()
    {
        QList<WriteEntry> entries;
        WriteEntry entry;
        entry.offset = m_Offset;


        // Adds bank, map and all the pointers to the entry
        entry.data.push_back(m_Bank);
        entry.data.push_back(m_Map);
        entry.addHWord(0x0000); // padding
        entry.addPointer(m_PtrGrass);
        entry.addPointer(m_PtrWater);
        entry.addPointer(m_PtrRock);
        entry.addPointer(m_PtrFish);
        entries.push_back(entry);

        // Writes each encounter array (if their offset is not NULL)
        if (m_PtrGrass)
            entries.append(m_AreaGrass.write(m_PtrGrass));
        if (m_PtrWater)
            entries.append(m_AreaWater.write(m_PtrWater));
        if (m_PtrRock)
            entries.append(m_AreaRock.write(m_PtrRock));
        if (m_PtrFish)
            entries.append(m_AreaFish.write(m_PtrFish));


        return entries;
    }


    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/3/2016
    //
    ///////////////////////////////////////////////////////////
    WildPokemonArea &WildPokemonSubTable::encounter(EncounterArea area)
    {
        switch (area)
        {
            case EA_AREA_GRASS: return m_AreaGrass;
            case EA_AREA_WATER: return m_AreaWater;
            case EA_AREA_ROCK:  return m_AreaRock;
            case EA_AREA_FISH:  return m_AreaFish;
            default:            return m_AreaGrass;
        }
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/4/2016
    //
    ///////////////////////////////////////////////////////////
    UInt32 WildPokemonSubTable::offset() const
    {
        return m_Offset;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/4/2016
    //
    ///////////////////////////////////////////////////////////
    UInt8 WildPokemonSubTable::bank() const
    {
        return m_Bank;
    }

    ///////////////////////////////////////////////////////////
    // Function type:  Getter
    // Contributors:   Pokedude
    // Last edit by:   Pokedude
    // Date of edit:   6/4/2016
    //
    ///////////////////////////////////////////////////////////
    UInt8 WildPokemonSubTable::map() const
    {
        return m_Map;
    }
}
